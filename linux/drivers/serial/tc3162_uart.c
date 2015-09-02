/*
 *	Serial driver for TC3162 SoC
 */

#include <linux/autoconf.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/console.h>

#include <asm/tc3162/tc3162.h>

#define TC3162_NR_PORTS				1

#define TC3162_UART_SIZE			0x30

#define PORT_TC3162					3162
#ifdef CONFIG_TC3162_ADSL
void (*send_uart_msg)(char* msg, int len);
EXPORT_SYMBOL(send_uart_msg);
static char tuart_buf[1024];
#endif

static void tc3162ser_stop_tx(struct uart_port *port)
{
	VPchar(CR_UART_IER) &= ~IER_THRE_INTERRUPT_ENABLE;	
}

static void tc3162ser_irq_rx(struct uart_port *port)
{
	struct tty_struct *tty = port->info->tty;
	unsigned int ch, flg;

	while (LSR_INDICATOR & LSR_RECEIVED_DATA_READY) {
		/* 
		 * We need to read rds before reading the 
		 * character from the fifo
		 */
		ch = VPchar(CR_UART_RBR);
		port->icount.rx++;

		if (tty->low_latency)
			tty_flip_buffer_push(tty);

		flg = TTY_NORMAL;

		tty_insert_flip_char(tty, ch, flg);
	}
	tty_flip_buffer_push(tty);
}

static void tc3162ser_irq_tx(struct uart_port *port)
{
	struct circ_buf *xmit = &port->info->xmit;
	int count;

#ifdef CONFIG_TC3162_ADSL
	int len=0;
	memset(tuart_buf, 0, sizeof(tuart_buf));
#endif
	if (port->x_char) {
		VPchar(CR_UART_THR) = port->x_char;
		port->icount.tx++;
		port->x_char = 0;
		return;
	}
	if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
		tc3162ser_stop_tx(port);
		return;
	}

	count = port->fifosize;
	do {
		VPchar(CR_UART_THR) = 
		    xmit->buf[xmit->tail];
#ifdef CONFIG_TC3162_ADSL
		if((void *)send_uart_msg){
			tuart_buf[len] = xmit->buf[xmit->tail];
			len++;
		}
#endif
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
		if (uart_circ_empty(xmit))
			break;
	} while (--count > 0);
#ifdef CONFIG_TC3162_ADSL
	if((void *)send_uart_msg){
		send_uart_msg(tuart_buf, len);
	}
#endif

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

	if (uart_circ_empty(xmit))
		tc3162ser_stop_tx(port);
}

static irqreturn_t tc3162ser_irq(int irq, void *dev_id)
{
	struct uart_port *port = dev_id;
	uint8 iir = IIR_INDICATOR;

	if (((iir & IIR_RECEIVED_DATA_AVAILABLE) == IIR_RECEIVED_DATA_AVAILABLE) ||
		((iir & IIR_RECEIVER_IDLE_TRIGGER) == IIR_RECEIVER_IDLE_TRIGGER)) 
		tc3162ser_irq_rx(port);
	if ((iir & IIR_TRANSMITTED_REGISTER_EMPTY) == IIR_TRANSMITTED_REGISTER_EMPTY) 
		tc3162ser_irq_tx(port);

	return IRQ_HANDLED;
}

static unsigned int tc3162ser_tx_empty(struct uart_port *port)
{
	unsigned int ret;

	ret = (LSR_INDICATOR & LSR_THRE) ? TIOCSER_TEMT : 0;
	return ret;
}

static void tc3162ser_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
}

static unsigned int tc3162ser_get_mctrl(struct uart_port *port)
{
	unsigned int result = 0;
	return result;
}

static void tc3162ser_start_tx(struct uart_port *port)
{
	VPchar(CR_UART_IER) |= IER_THRE_INTERRUPT_ENABLE;	
}

static void tc3162ser_stop_rx(struct uart_port *port)
{
	VPchar(CR_UART_IER) &= ~IER_RECEIVED_DATA_INTERRUPT_ENABLE;	
}

static void tc3162ser_enable_ms(struct uart_port *port)
{
}

static void tc3162ser_break_ctl(struct uart_port *port, int break_state)
{
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	spin_unlock_irqrestore(&port->lock, flags);
}

#ifdef CONFIG_MIPS_TC3262
static void tc3162ser_irq_dispatch(void)
{
	do_IRQ(UART_INT);
}	
#endif

static int tc3162ser_startup(struct uart_port *port)
{
	int ret;

#ifdef CONFIG_MIPS_TC3262
	if (cpu_has_vint)
		set_vi_handler(port->irq, tc3162ser_irq_dispatch);
#endif
	ret = request_irq(port->irq, tc3162ser_irq, 0, "TC3162 UART", port);
	if (ret) {
		printk(KERN_ERR "Couldn't get irq %d ret=%d\n", port->irq, ret);
		return ret;
	}
	VPchar(CR_UART_IER) |= IER_RECEIVED_DATA_INTERRUPT_ENABLE;	
	return 0;
}

static void tc3162ser_shutdown(struct uart_port *port)
{
	VPchar(CR_UART_IER) &= ~IER_RECEIVED_DATA_INTERRUPT_ENABLE;	
	free_irq(port->irq, port);
}

static void tc3162ser_set_termios(struct uart_port *port,
    struct ktermios *termios, struct ktermios *old)
{
	unsigned int baud, quot;
	unsigned long flags;

	termios->c_cflag |= CREAD;

	baud = 115200;
	quot = uart_get_divisor(port, baud);

	spin_lock_irqsave(&port->lock, flags);

	/*
	 * Update the per-port timeout.
	 */
	uart_update_timeout(port, termios->c_cflag, baud);

	/*
	 * Characters to ignore
	 */
	port->ignore_status_mask = 0;

	spin_unlock_irqrestore(&port->lock, flags);
}

static const char *tc3162ser_type(struct uart_port *port)
{
	return port->type == PORT_TC3162 ? "TC3162" : NULL;
}

static void tc3162ser_config_port(struct uart_port *port, int flags)
{
	if (flags & UART_CONFIG_TYPE)
		port->type = PORT_TC3162;
}

static void tc3162ser_release_port(struct uart_port *port)
{
	release_mem_region(port->iobase, TC3162_UART_SIZE);
}

static int tc3162ser_request_port(struct uart_port *port)
{
	return request_mem_region(port->iobase, TC3162_UART_SIZE,
	    "tc3162-uart") != NULL ? 0 : -EBUSY; 
}

static struct uart_ops tc3162ser_ops = {
	.tx_empty =		tc3162ser_tx_empty,
	.set_mctrl =	tc3162ser_set_mctrl,
	.get_mctrl =	tc3162ser_get_mctrl,
	.stop_tx =		tc3162ser_stop_tx,
	.start_tx =		tc3162ser_start_tx,
	.stop_rx =		tc3162ser_stop_rx,
	.enable_ms =	tc3162ser_enable_ms,
	.break_ctl =	tc3162ser_break_ctl,
	.startup =		tc3162ser_startup,
	.shutdown =		tc3162ser_shutdown,
	.set_termios =	tc3162ser_set_termios,
	.type =			tc3162ser_type,
	.config_port =	tc3162ser_config_port,
	.release_port =	tc3162ser_release_port,
	.request_port =	tc3162ser_request_port,
};

static void tc3162_console_put(const char c)
{
	while (!(LSR_INDICATOR & LSR_THRE))
		;
	VPchar(CR_UART_THR) = c; 
}

static void tc3162_console_write(struct console *con, const char *s,
    unsigned int count)
{
#ifdef CONFIG_TC3162_ADSL
	/*The prink message is hook this funcion.*/
	if((void *)send_uart_msg){
		send_uart_msg((char *)s, count);
	}
#endif
	while (count--) {
		if (*s == '\n')
			tc3162_console_put('\r');
		tc3162_console_put(*s);
		s++;
	}
}

static int tc3162_console_setup(struct console *con, char *options)
{
	return 0;
}

static struct uart_driver tc3162ser_reg;

static struct console tc3162_serconsole = {
	.name =		"ttyS",
	.write =	tc3162_console_write,
	.device =	uart_console_device,
	.setup =	tc3162_console_setup,
	.flags =	CON_PRINTBUFFER,
	.cflag =	B115200 | CS8 | CREAD,
	.index =	-1,
	.data =		&tc3162ser_reg,
};

static int __init tc3162_console_init(void)
{
	register_console(&tc3162_serconsole);
	return 0;
}

console_initcall(tc3162_console_init);

static struct uart_port tc3162ser_ports[] = {
	{
		.iobase =	0xbfbf0003,
		.irq =		UART_INT,
		.uartclk =	115200,
		.fifosize =	1,
		.ops =		&tc3162ser_ops,
		.line =		0,
		.flags =	ASYNC_BOOT_AUTOCONF,
	}
};

static struct uart_driver tc3162ser_reg = {
	.owner	=		THIS_MODULE,
	.driver_name =	"ttyS",
	.dev_name =		"ttyS",
	.major =		TTY_MAJOR,
	.minor =		64,
	.nr =			1,
};

static int __init tc3162ser_init(void)
{
	int ret, i;

	ret = uart_register_driver(&tc3162ser_reg);
	if (!ret) {
		for (i = 0; i < TC3162_NR_PORTS; i++)
			uart_add_one_port(&tc3162ser_reg, &tc3162ser_ports[i]);
	}

	return ret;
}

#ifdef CONFIG_TC3162_ADSL
/*_____________________________________________________________________________
**      function name: tcconsole_cmd
**      descriptions:
**             This function is used the send command to uart drivers that is used
**		tcconsole utility.
**
**      parameters:
**             cmd: 	Specify the command line strings.
**             len: 	The length of command.
**
**      global:
**             tc3162ser_ports
**
**      return:
**             none
**
**      call:
**      	tty_flip_buffer_push
**      	tty_insert_flip_char
**
**      revision:
**      1. Here 2010/9/23
**____________________________________________________________________________
*/
void 
tcconsole_cmd(char* cmd, int len){
	struct uart_port *port= &tc3162ser_ports[0];
	struct tty_struct *tty = port->info->tty;
	unsigned int ch, flg;
	int i;
	
	/*Ignore the line  feed character*/	
	for(i=0; i<len-1; i++){
		ch = cmd[i];
		port->icount.rx++;

		if (tty->low_latency)
			tty_flip_buffer_push(tty);

		flg = TTY_NORMAL;

		tty_insert_flip_char(tty, ch, flg);
	}
	tty_flip_buffer_push(tty);
}/*end tcconsole_cmd*/
EXPORT_SYMBOL(tcconsole_cmd);
#endif 

__initcall(tc3162ser_init);


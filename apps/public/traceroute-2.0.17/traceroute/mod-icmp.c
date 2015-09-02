/*
    Copyright (c)  2006, 2007		Dmitry Butskoy
					<buc@citadel.stu.neva.ru>
    License:  GPL v2 or any later

    See COPYING for the status of this software.
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/icmp6.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>

#include "traceroute.h"


static sockaddr_any dest_addr = {{ 0, }, };
static u_int16_t seq = 1;
static u_int16_t ident = 0;

static char *data;
static size_t *length_p;

static int icmp_sk = -1;
static int last_ttl = 0;


static int icmp_init (const sockaddr_any *dest,
			    unsigned int port_seq, size_t *packet_len_p) {
	int i;
	int af = dest->sa.sa_family;

	dest_addr = *dest;
	dest_addr.sin.sin_port = 0;

	if (port_seq)  seq = port_seq;

	length_p = packet_len_p;
	if (*length_p < sizeof (struct icmphdr))
		*length_p = sizeof (struct icmphdr);

	data = malloc (*length_p);
	if (!data)  error ("malloc");

        for (i = sizeof (struct icmphdr); i < *length_p; i++)
                data[i] = 0x40 + (i & 0x3f);


	icmp_sk = socket (af, SOCK_RAW, (af == AF_INET) ? IPPROTO_ICMP
							: IPPROTO_ICMPV6);
	if (icmp_sk < 0)
		error_or_perm ("socket");

	tune_socket (icmp_sk);

	/*  Don't want to catch packets from another hosts   */
	if (raw_can_connect () &&
	    connect (icmp_sk, &dest_addr.sa, sizeof (dest_addr)) < 0
	)  error ("connect");

	use_recverr (icmp_sk);


	add_poll (icmp_sk, POLLIN | POLLERR);

	ident = getpid () & 0xffff;
 
	return 0;
}


static void icmp_send_probe (probe *pb, int ttl) {
	int af = dest_addr.sa.sa_family;


	if (ttl != last_ttl) {

	    set_ttl (icmp_sk, ttl);

	    last_ttl = ttl;
	}


	if (af == AF_INET) {
	    struct icmp *icmp = (struct icmp *) data;

	    icmp->icmp_type = ICMP_ECHO;
	    icmp->icmp_code = 0;
	    icmp->icmp_cksum = 0;
	    icmp->icmp_id = htons (ident);
	    icmp->icmp_seq = htons (seq);

	    icmp->icmp_cksum = in_csum (data, *length_p);
	}
	else if (af == AF_INET6) {
	    struct icmp6_hdr *icmp6 = (struct icmp6_hdr *) data;

	    icmp6->icmp6_type = ICMP6_ECHO_REQUEST;
	    icmp6->icmp6_code = 0;
	    icmp6->icmp6_cksum = 0;
	    icmp6->icmp6_id = htons (ident);
	    icmp6->icmp6_seq = htons(seq);

	    /*  icmp6->icmp6_cksum always computed by kernel internally   */
	}


	pb->send_time = get_time ();

	if (do_send (icmp_sk, data, *length_p, &dest_addr) < 0) {
	    pb->send_time = 0;
	    return;
	}


	pb->seq = seq;

	seq++;

	return;
}


static probe *icmp_check_reply (int sk, int err, sockaddr_any *from,
						    char *buf, size_t len) {
	int af = dest_addr.sa.sa_family;
	int type;
	u_int16_t recv_id, recv_seq;
	probe *pb;


	if (len < sizeof (struct icmphdr))
		return NULL;


	if (af == AF_INET) {
	    struct icmp *icmp = (struct icmp *) buf;

	    type = icmp->icmp_type;

	    recv_id = ntohs (icmp->icmp_id);
	    recv_seq = ntohs (icmp->icmp_seq);

	}
	else {	    /*  AF_INET6   */
	    struct icmp6_hdr *icmp6 = (struct icmp6_hdr *) buf;

	    type = icmp6->icmp6_type;

	    recv_id = ntohs (icmp6->icmp6_id);
	    recv_seq = ntohs (icmp6->icmp6_seq);
	}


	if (recv_id != ident)
		return NULL;

	pb = probe_by_seq (recv_seq);
	if (!pb)  return NULL;


	if (!err) {

	    if (!(af == AF_INET && type == ICMP_ECHOREPLY) &&
		!(af == AF_INET6 && type == ICMP6_ECHO_REPLY)
	    )  return NULL;

	    pb->final = 1;
	}

	return pb;
}


static void icmp_recv_probe (int sk, int revents) {

	if (!(revents & (POLLIN | POLLERR)))
		return;

	recv_reply (sk, !!(revents & POLLERR), icmp_check_reply);
}


static void icmp_expire_probe (probe *pb) {

	probe_done (pb);
}


static tr_module icmp_ops = {
	.name = "icmp",
	.init = icmp_init,
	.send_probe = icmp_send_probe,
	.recv_probe = icmp_recv_probe,
	.expire_probe = icmp_expire_probe,
};

TR_MODULE (icmp_ops);

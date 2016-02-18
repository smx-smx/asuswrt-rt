#ifndef _APPS_H
#define _APPS_H

#include "cfg_manager.h"

int 
vram_init(void);
int
usb_init(void);
int
usb_boot(mxml_node_t *top);
int
usbmodem_init(void);
int
usbmodem_boot(mxml_node_t *top);
int
usbmodem_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
apps_init(void);
int
apps_boot(mxml_node_t *top);
int
aicloud_init(void);
int
aicloud_boot(mxml_node_t *top);
int
aicloud_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#endif

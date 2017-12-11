#ifndef _BWDPI_H
#define _BWDPI_H

int AiProtection_boot(mxml_node_t *top);
int AiProtection_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int AiProtection_init();
#endif

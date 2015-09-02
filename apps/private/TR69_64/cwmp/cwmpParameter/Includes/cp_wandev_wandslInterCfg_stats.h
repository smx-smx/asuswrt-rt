#ifndef CP_WANDEV_WANDSLINTERCFG_STATS_H_
#define CP_WANDEV_WANDSLINTERCFG_STATS_H_

#include "cp_core.h"

/*----------------------stats showtime function begin----------------------*/
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getReceiveBlock_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getReceiveBlock_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReceiveBlock_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTransmitBlock_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTransmitBlock_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTransmitBlock_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCellDelin_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCellDelin_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCellDelin_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLinkRetrain_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLinkRetrain_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLinkRetrain_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInitErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInitErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInitErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInitTimeouts_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInitTimeouts_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInitTimeouts_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLossOfFraming_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLossOfFraming_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLossOfFraming_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLOF_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLOF_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLOF_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getErroredSecs_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getErroredSecs_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setErroredSecs_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSeveErrSecs_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSeveErrSecs_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSeveErrSecs_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFecErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFecErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFecErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCFECErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCFECErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCFECErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHECErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHECErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHECErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCRCErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCRCErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCRCErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCCRCErrors_stValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCCRCErrors_stAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCCRCErrors_stAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFecErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFecErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFecErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCFECErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCFECErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCFECErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
/*----------------------stats showtime function end----------------------*/

/*----------------------stats total function begin----------------------*/
int8 getReceiveBlock_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getReceiveBlock_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReceiveBlock_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTransmitBlock_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTransmitBlock_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTransmitBlock_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCellDelin_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCellDelin_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCellDelin_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLinkRetrain_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLinkRetrain_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLinkRetrain_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInitErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInitErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInitErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInitTimeouts_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInitTimeouts_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInitTimeouts_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLossOfFraming_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLossOfFraming_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLossOfFraming_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getErroredSecs_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getErroredSecs_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setErroredSecs_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSeveErrSecs_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSeveErrSecs_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSeveErrSecs_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHECErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHECErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHECErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCRCErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCRCErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCRCErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCCRCErrors_totalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCCRCErrors_totalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCCRCErrors_totalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
/*----------------------stats total function end----------------------*/

/*----------------------stats lastShowtime function begin----------------------*/
int8 getReceiveBlock_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getReceiveBlock_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReceiveBlock_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTransmitBlock_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTransmitBlock_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTransmitBlock_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCellDelin_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCellDelin_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCellDelin_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLinkRetrain_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLinkRetrain_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLinkRetrain_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInitErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInitErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInitErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInitTimeouts_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInitTimeouts_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInitTimeouts_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLossOfFraming_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLossOfFraming_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLossOfFraming_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getErroredSecs_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getErroredSecs_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setErroredSecs_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSeveErrSecs_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSeveErrSecs_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSeveErrSecs_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFecErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFecErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFecErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCFECErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCFECErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCFECErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHECErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHECErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHECErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCRCErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCRCErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCRCErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCCRCErrors_lstValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCCRCErrors_lstAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCCRCErrors_lstAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
/*----------------------stats lastShowtime function end----------------------*/

/*----------------------stats currentDay function begin----------------------*/
int8 getReceiveBlock_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getReceiveBlock_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReceiveBlock_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTransmitBlock_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTransmitBlock_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTransmitBlock_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCellDelin_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCellDelin_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCellDelin_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLinkRetrain_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLinkRetrain_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLinkRetrain_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInitErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInitErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInitErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInitTimeouts_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInitTimeouts_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInitTimeouts_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLossOfFraming_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLossOfFraming_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLossOfFraming_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getErroredSecs_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getErroredSecs_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setErroredSecs_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSeveErrSecs_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSeveErrSecs_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSeveErrSecs_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFecErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFecErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFecErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCFECErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCFECErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCFECErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHECErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHECErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHECErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCRCErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCRCErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCRCErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCCRCErrors_cdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCCRCErrors_cdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCCRCErrors_cdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
/*----------------------stats currentDay function end----------------------*/

/*----------------------stats quarterHour function begin----------------------*/
int8 getReceiveBlock_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getReceiveBlock_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReceiveBlock_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTransmitBlock_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTransmitBlock_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTransmitBlock_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCellDelin_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCellDelin_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCellDelin_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLinkRetrain_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLinkRetrain_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLinkRetrain_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInitErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInitErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInitErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInitTimeouts_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInitTimeouts_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInitTimeouts_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLossOfFraming_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLossOfFraming_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLossOfFraming_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getErroredSecs_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getErroredSecs_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setErroredSecs_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSeveErrSecs_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSeveErrSecs_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSeveErrSecs_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFecErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFecErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFecErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCFECErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCFECErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCFECErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHECErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHECErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHECErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCRCErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCRCErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCRCErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getATUCCRCErrors_qhValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getATUCCRCErrors_qhAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setATUCCRCErrors_qhAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
/*----------------------stats quarterHour function end----------------------*/
uint32 atolu(const char* nptr);
#endif


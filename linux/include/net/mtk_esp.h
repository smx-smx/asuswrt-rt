#ifndef MTK_ESP_H
#define MTK_ESP_H

#include <linux/skbuff.h>
#include <linux/dma-mapping.h>
#include <linux/kernel.h>

#define HASH_DIGEST_OUT			0
#define HASH_DIGEST_IN			1
#define CRYPTO_ENCRYPTION		1
#define CRYPTO_DECRYPTION		2


/************************************************************************
*      E X T E R N E L    S T R U C T U R E    D E F I N I T I O N
*************************************************************************
*/
typedef union
{
	struct
	{
		unsigned int padCrtlStat	: 8;
		unsigned int errStatus		: 8;
		unsigned int padValue		: 8;
		unsigned int prngMode		: 2;
		unsigned int haltMode		: 1;
		unsigned int hashFinal		: 1;
		unsigned int initArc4		: 1;
		unsigned int reserved		: 1;
		unsigned int peReady		: 1;
		unsigned int hostReady		: 1;
	} bits;
	unsigned int word;
		
} peCrtlStat_t;

typedef union
{
	struct
	{
		unsigned int byPass			: 8;
		unsigned int peReady		: 1;
		unsigned int hostReady		: 1;
		unsigned int reserved		: 2;
		unsigned int length			: 20;
	} bits;	
	unsigned int word;
		
} peLength_t;

typedef struct addrHandler_s
{
	unsigned int addr;
	dma_addr_t	 phyAddr;

} addrHandler_t;

typedef struct eip93DescpHandler_s
{
	peCrtlStat_t	peCrtlStat;
	addrHandler_t	srcAddr;
	addrHandler_t	dstAddr;
	addrHandler_t	saAddr;
	addrHandler_t	stateAddr;
	addrHandler_t	arc4Addr;
	unsigned int	userId;
	peLength_t		peLength;

} eip93DescpHandler_t;

typedef struct addrsDigestPreCompute_s
{
	unsigned int 		*hashKeyTank;
	addrHandler_t 		ipadHandler;
	addrHandler_t 		opadHandler;
	unsigned int 		blkSize;
	eip93DescpHandler_t *cmdHandler;
	addrHandler_t 		saHandler;
	addrHandler_t 		stateHandler;
	addrHandler_t 		stateHandler2;
	unsigned int 		digestWord;
	unsigned int 		*pIDigest;
	unsigned int 		*pODigest;

} addrsDigestPreCompute_t;

typedef struct ipsecEip93Adapter_s
{
	unsigned int 				spi; //every ipsec flow has a unique spi
	struct xfrm_state 			*x; //the SA
	unsigned int 				isHashPreCompute; //0:pre-compute init, 1:inner digest done, 2:inner digest done, 3:pre-compute done
	unsigned int 				isEncryptOrDecrypt; //1:encrypt, 2:decrypt
	struct sk_buff_head 		skbQueue;
	addrsDigestPreCompute_t		*addrsPreCompute; //for hash pre-compute
	eip93DescpHandler_t 		*cmdHandler; //for encrypt/decrypt
	spinlock_t 					lock;
	unsigned int 				addedLen; //refer to ssh_hwaccel_alloc_combined() in safenet_la.c

} ipsecEip93Adapter_t;

/************************************************************************
*      E X T E R N E L     F U N C T I O N    D E C L A R A T I O N
*************************************************************************
*/
extern int 
(*ipsec_packet_put)(
	eip93DescpHandler_t *descpHandler, struct sk_buff *skb
);
extern int 
(*ipsec_packet_get)(
	eip93DescpHandler_t *descpHandler
);
extern bool 
(*ipsec_eip93CmdResCnt_check)(
	void
);
extern int 
(*ipsec_preComputeIn_cmdDescp_set)(
	ipsecEip93Adapter_t *currAdapterPtr, 
	unsigned int direction
);
extern int 
(*ipsec_preComputeOut_cmdDescp_set)(
	ipsecEip93Adapter_t *currAdapterPtr, 
	unsigned int direction
);
extern int 
(*ipsec_cmdHandler_cmdDescp_set)(
	ipsecEip93Adapter_t *currAdapterPtr, 
	unsigned int direction,
	unsigned int cipherAlg, 
	unsigned int hashAlg, 
	unsigned int cipherMode, 
	unsigned int enHmac, 
	unsigned int aesKeyLen, 
	unsigned int *cipherKey, 
	unsigned int keyLen, 
	unsigned int spi, 
	unsigned int padCrtlStat
);
extern void 
(*ipsec_espNextHeader_set)(
	eip93DescpHandler_t *cmdHandler, 
	unsigned char protocol	
);
extern unsigned char 
(*ipsec_espNextHeader_get)(
	eip93DescpHandler_t *resHandler
);
extern unsigned int 
(*ipsec_pktLength_get)(
	eip93DescpHandler_t *resHandler
);
extern unsigned int 
(*ipsec_eip93HashFinal_get)(
	eip93DescpHandler_t *resHandler
);
extern unsigned int 
(*ipsec_eip93UserId_get)(
	eip93DescpHandler_t *resHandler
);

extern void 
(*ipsec_addrsDigestPreCompute_free)(
	ipsecEip93Adapter_t *currAdapterPtr
);

extern void 
(*ipsec_cmdHandler_free)(
	eip93DescpHandler_t *cmdHandler
);

extern void 
(*ipsec_hashDigests_get)(
	ipsecEip93Adapter_t *currAdapterPtr
);
extern void 
(*ipsec_hashDigests_set)(
	ipsecEip93Adapter_t *currAdapterPtr,
	unsigned int isInOrOut
);
	
extern void 
ipsec_eip93_adapters_init(
	void
);
extern void 
ipsec_cryptoLock_init(
	void
);
extern void 
ipsec_BH_handler_resultGet(
	void
);
#endif


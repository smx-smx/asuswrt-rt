/************************************************************************
 *
 *	Copyright (C) 2012 MediaTek Technologies, Corp.
 *	All Rights Reserved.
 *
 * MediaTek Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of MediaTek Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * MediaTek Technologeis, Co.
 *
 *************************************************************************/


#include <linux/err.h>
#include <linux/module.h>
#include <net/ip.h>
#include <net/xfrm.h>
#include <net/esp.h>
#include <asm/scatterlist.h>
#include <linux/crypto.h>
#include <linux/kernel.h>
#include <linux/pfkeyv2.h>
#include <linux/random.h>
#include <net/icmp.h>
#include <net/protocol.h>
#include <net/udp.h>

#include <net/mtk_esp.h>
#include <linux/netfilter_ipv4.h>


/************************************************************************
*                          C O N S T A N T S
*************************************************************************
*/
#define IPESC_EIP93_ADAPTERS	16
#define HASH_MD5_HMAC			"hmac(md5)"
#define HASH_SHA1_HMAC			"hmac(sha1)"
#define HASH_SHA256_HMAC		"hmac(sha256)"
#define HASH_NULL_HMAC 			"hmac(digest_null)"
#define HASH_IPAD				0x36363636
#define HASH_OPAD				0x5c5c5c5c
#define CIPHER_DES_CBC			"cbc(des)"
#define CIPHER_3DES_CBC			"cbc(des3_ede)"
#define CIPHER_AES_CBC			"cbc(aes)"
#define CIPHER_NULL_ECB			"ecb(cipher_null)"
#define SKB_QUEUE_MAX_SIZE		100

/************************************************************************
*      P R I V A T E    S T R U C T U R E    D E F I N I T I O N
*************************************************************************
*/


/************************************************************************
*              P R I V A T E     D A T A
*************************************************************************
*/
static ipsecEip93Adapter_t 	*ipsecEip93AdapterListOut[IPESC_EIP93_ADAPTERS];
static ipsecEip93Adapter_t 	*ipsecEip93AdapterListIn[IPESC_EIP93_ADAPTERS];
static spinlock_t 			cryptoLock;
static eip93DescpHandler_t 	resDescpHandler;

/************************************************************************
*              E X T E R N A L     D A T A
*************************************************************************
*/
int 
(*ipsec_packet_put)(
	eip93DescpHandler_t *descpHandler, 
	struct sk_buff *skb
);
int 
(*ipsec_packet_get)(
	eip93DescpHandler_t *descpHandler
);
bool 
(*ipsec_eip93CmdResCnt_check)(
	void
);
int 
(*ipsec_preComputeIn_cmdDescp_set)(
	ipsecEip93Adapter_t *currAdapterPtr, 
	unsigned int direction
);
int 
(*ipsec_preComputeOut_cmdDescp_set)(
	ipsecEip93Adapter_t *currAdapterPtr, 
	unsigned int direction
);
int 
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
void 
(*ipsec_espNextHeader_set)(
	eip93DescpHandler_t *cmdHandler, 
	unsigned char protocol	
);
unsigned char 
(*ipsec_espNextHeader_get)(
	eip93DescpHandler_t *resHandler
);
unsigned int 
(*ipsec_pktLength_get)(
	eip93DescpHandler_t *resHandler
);
unsigned int 
(*ipsec_eip93HashFinal_get)(
	eip93DescpHandler_t *resHandler
);
unsigned int 
(*ipsec_eip93UserId_get)(
	eip93DescpHandler_t *resHandler
);

void 
(*ipsec_addrsDigestPreCompute_free)(
	ipsecEip93Adapter_t *currAdapterPtr
);

void 
(*ipsec_cmdHandler_free)(
	eip93DescpHandler_t *cmdHandler
);

void 
(*ipsec_hashDigests_get)(
	ipsecEip93Adapter_t *currAdapterPtr
);

void 
(*ipsec_hashDigests_set)(
	ipsecEip93Adapter_t *currAdapterPtr,
	unsigned int isInOrOut
);



EXPORT_SYMBOL(ipsec_packet_put);
EXPORT_SYMBOL(ipsec_packet_get);
EXPORT_SYMBOL(ipsec_eip93CmdResCnt_check);
EXPORT_SYMBOL(ipsec_preComputeIn_cmdDescp_set);
EXPORT_SYMBOL(ipsec_preComputeOut_cmdDescp_set);
EXPORT_SYMBOL(ipsec_cmdHandler_cmdDescp_set);
EXPORT_SYMBOL(ipsec_espNextHeader_set);
EXPORT_SYMBOL(ipsec_espNextHeader_get);
EXPORT_SYMBOL(ipsec_pktLength_get);
EXPORT_SYMBOL(ipsec_eip93HashFinal_get);
EXPORT_SYMBOL(ipsec_eip93UserId_get);
EXPORT_SYMBOL(ipsec_addrsDigestPreCompute_free);
EXPORT_SYMBOL(ipsec_cmdHandler_free);
EXPORT_SYMBOL(ipsec_hashDigests_get);
EXPORT_SYMBOL(ipsec_hashDigests_set);


/************************************************************************
*              P R I V A T E     F U N C T I O N S
*************************************************************************
*/
/*_______________________________________________________________________
**function name: ipsec_hashDigest_preCompute
**
**description:
*   EIP93 can only use Hash Digests (not Hash keys) to do authentication!
*	This funtion is to use EIP93 to generate Hash Digests from Hash keys!
*	Only the first packet for a IPSec flow need to do this!
**parameters:
*   x -- point to the structure that stores IPSec SA information
*	currAdapterPtr -- point to the structure that stores needed info
*		for Hash Digest Pre-Compute. After Pre-Compute is done,
*		currAdapterPtr->addrsPreCompute is used to free resource.
*	digestPreComputeDir -- indicate direction for encryption or
*		decryption.
**global:
*   none
**return:
*   -EPERM, -ENOMEM -- failed: the pakcet will be dropped!
*	1 -- success
**call:
*   none
**revision:
*   1.Trey 20120209
**_______________________________________________________________________*/
static int 
ipsec_hashDigest_preCompute(
	struct xfrm_state *x, 
	ipsecEip93Adapter_t *currAdapterPtr, 
	unsigned int digestPreComputeDir
)
{
	struct esp_data *esp = x->data;
	char hashKeyName[32];
	unsigned int blkSize, blkWord, digestWord, hashKeyLen, hashKeyWord;
	unsigned int *ipad, *opad, *hashKey, *hashKeyTank;
	dma_addr_t	ipadPhyAddr, opadPhyAddr;
	unsigned int *pIDigest, *pODigest;
	unsigned int i, j;
	int errVal;
	addrsDigestPreCompute_t* addrsPreCompute;
	

	strcpy(hashKeyName, esp->auth.tfm->base.__crt_alg->cra_name);
	hashKeyLen = esp->auth.key_len;
	hashKeyWord = hashKeyLen >> 2;

	if (strcmp(hashKeyName, HASH_MD5_HMAC) == 0)
	{
		blkSize = 64; //bytes
		digestWord = 4; //words
	}
	else if (strcmp(hashKeyName, HASH_SHA1_HMAC) == 0)
	{
		blkSize = 64; //bytes
		digestWord = 5; //words	
	}
	else if (strcmp(hashKeyName, HASH_SHA256_HMAC) == 0)
	{
		blkSize = 64; //bytes
		digestWord = 8; //words	
	}
	else
	{
		printk("\n !Unsupported Hash Algorithm by EIP93: %s! \n", hashKeyName);
		return -EPERM;
	}

	
	addrsPreCompute = (addrsDigestPreCompute_t *) kzalloc(sizeof(addrsDigestPreCompute_t), GFP_KERNEL);
	if (unlikely(addrsPreCompute == NULL))
	{
		printk("\n\n !!kmalloc for addrsPreCompute failed!! \n\n");
		return -ENOMEM;
	}
	currAdapterPtr->addrsPreCompute = addrsPreCompute;
	
	hashKeyTank = (unsigned int *) kzalloc(blkSize, GFP_KERNEL);
	if (unlikely(hashKeyTank == NULL))
	{
		printk("\n\n !!kmalloc for hashKeyTank failed!! \n\n");
		errVal = -ENOMEM;
		goto free_addrsPreCompute;
	}
	addrsPreCompute->hashKeyTank = hashKeyTank;
	
	ipad = (unsigned int *) dma_alloc_coherent(NULL, blkSize, &ipadPhyAddr, GFP_KERNEL);
	if (unlikely(ipad == NULL))
	{
		printk("\n\n !!dma_alloc for ipad failed!! \n\n");
		errVal = -ENOMEM;
		goto free_hashKeyTank;
	}
	addrsPreCompute->ipadHandler.addr = (unsigned int)ipad;
	addrsPreCompute->ipadHandler.phyAddr = ipadPhyAddr;
	addrsPreCompute->blkSize = blkSize;
	
	opad = (unsigned int *) dma_alloc_coherent(NULL, blkSize, &opadPhyAddr, GFP_KERNEL);
	if (unlikely(opad == NULL))
	{
		printk("\n\n !!dma_alloc for opad failed!! \n\n");
		errVal = -ENOMEM;
		goto free_ipad;
	}
	addrsPreCompute->opadHandler.addr = (unsigned int)opad;
	addrsPreCompute->opadHandler.phyAddr = opadPhyAddr;	

		
	blkWord = blkSize >> 2;
	hashKey = (unsigned int *)esp->auth.key;
	
	if(hashKeyLen <= blkSize)
	{
		for(i = 0; i < hashKeyWord; i++)
		{
			hashKeyTank[i] = hashKey[i];
		}
		for(j = i; j < blkWord; j++)
		{
			hashKeyTank[j] = 0x0;
		}
	}
	else
	{
		// EIP93 supports md5, sha1, sha256. Their hash key length and their function output length should be the same, which are 128, 160, and 256 bits respectively! Their block size are 64 bytes which are always larger than all of their hash key length! 
		printk("\n !Unsupported hashKeyLen:%d by EIP93! \n", hashKeyLen);
		errVal = -EPERM;
		goto free_opad;
	}
	
	for(i=0; i<blkWord; i++)
	{
		ipad[i] = HASH_IPAD;
		opad[i] = HASH_OPAD;
		ipad[i] ^= hashKeyTank[i];
		opad[i] ^= hashKeyTank[i];			
	}
	

	pIDigest = (unsigned int *) kzalloc(sizeof(unsigned int) << 3, GFP_KERNEL);
	if(pIDigest == NULL)
	{
		printk("\n\n !!kmalloc for Hash Inner Digests failed!! \n\n");
		errVal = -ENOMEM;
		goto free_opad;
	}
	addrsPreCompute->pIDigest = pIDigest;
	
	pODigest = (unsigned int *) kzalloc(sizeof(unsigned int) << 3, GFP_KERNEL);
	if(pODigest == NULL)
	{
		printk("\n\n !!kmalloc for Hash Outer Digests failed!! \n\n");
		errVal = -ENOMEM;
		goto free_pIDigest;
	}
	addrsPreCompute->pODigest = pODigest;
		
	addrsPreCompute->digestWord = digestWord;
	
	currAdapterPtr->isHashPreCompute = 0; //pre-compute init
		
		
	/* start pre-compute for Hash Inner Digests */

	errVal = ipsec_preComputeIn_cmdDescp_set(currAdapterPtr, digestPreComputeDir);
	if (errVal < 0)
	{
		goto free_pODigest;
	}
		
	spin_lock(&cryptoLock);
	
	ipsec_packet_put(addrsPreCompute->cmdHandler, NULL); //mtk_packet_put()
	
	spin_unlock(&cryptoLock);
	
	
	/* start pre-compute for Hash Outer Digests */

	errVal = ipsec_preComputeOut_cmdDescp_set(currAdapterPtr, digestPreComputeDir);
	if (errVal < 0)
	{
		goto free_pODigest;
	}

	spin_lock(&cryptoLock);
	
	ipsec_packet_put(addrsPreCompute->cmdHandler, NULL); //mtk_packet_put()
	
	spin_unlock(&cryptoLock);
	
	
	return 1; //success
	

free_pODigest:
	kfree(pODigest);
free_pIDigest:
	kfree(pIDigest);
free_opad:
	dma_free_coherent(NULL, blkSize, opad, opadPhyAddr);		
free_ipad:
	dma_free_coherent(NULL, blkSize, ipad, ipadPhyAddr);		
free_hashKeyTank:
	kfree(hashKeyTank);
free_addrsPreCompute:
	kfree(addrsPreCompute);
	currAdapterPtr->addrsPreCompute = NULL;
	
	return errVal;	
}

/*_______________________________________________________________________
**function name: ipsec_cmdHandler_prepare
**
**description:
*   Prepare a command handler for a IPSec flow. This handler includes 
*	all needed information for EIP93 to do encryption/decryption.
*	Only the first packet for a IPSec flow need to do this!
**parameters:
*   x -- point to the structure that stores IPSec SA information
*	currAdapterPtr -- point to the structure that will stores the
*		command handler
*	cmdHandlerDir -- indicate direction for encryption or decryption.
**global:
*   none
**return:
*   -EPERM, -ENOMEM -- failed: the pakcet will be dropped!
*	1 -- success
**call:
*   none
**revision:
*   1.Trey 20120209
**_______________________________________________________________________*/
static int 
ipsec_cmdHandler_prepare(
	struct xfrm_state *x, 
	ipsecEip93Adapter_t *currAdapterPtr,
	unsigned int cmdHandlerDir
)
{
	int errVal;
	struct esp_data *esp = x->data;
	int padBoundary = ALIGN(crypto_blkcipher_blocksize(esp->conf.tfm), 4);
	unsigned int padCrtlStat, keyLen;
	char nameString[32];
	unsigned int cipherAlg, cipherMode, aesKeyLen = 0, hashAlg, enHmac;
	unsigned int *cipherKey;
	unsigned int addedLen = 0;

	
	addedLen += 8; //for esp header	
	
	/* decide pad boundary */
	switch(padBoundary){
		case 1:
			padCrtlStat = 0x1;
			addedLen += 1;
			break;
		case 4:
			padCrtlStat = 0x1 << 1;
			addedLen += 4;
			break;
		case 8:
			padCrtlStat = 0x1 << 2;
			addedLen += 8;
			break;
		case 16:
			padCrtlStat = 0x1 << 3;
			addedLen += 16;
			break;
		case 32:
			padCrtlStat = 0x1 << 4;
			addedLen += 32;
			break;
		case 64:
			padCrtlStat = 0x1 << 5;
			addedLen += 64;
			break;
		case 128:
			padCrtlStat = 0x1 << 6;
			addedLen += 128;
			break;
		case 256:
			padCrtlStat = 0x1 << 7;
			addedLen += 256;
			break;
		default:
			printk("\n !Unsupported pad boundary (%d) by EIP93! \n", padBoundary);
			errVal = -EPERM;
			goto free_addrsPreComputes;
	}
	
	
	/* decide cipher */

	strcpy(nameString, esp->conf.tfm->base.__crt_alg->cra_name);
	keyLen = esp->conf.key_len;

	if(strcmp(nameString, CIPHER_DES_CBC) == 0)
	{
		cipherAlg = 0x0; //des
		cipherMode = 0x1; //cbc
		addedLen += (8 + (8 + 1)); //iv + (esp trailer + padding)
	}
	else if(strcmp(nameString, CIPHER_3DES_CBC) == 0)
	{
		cipherAlg = 0x1; //3des
		cipherMode = 0x1; //cbc
		addedLen += (8 + (8 + 1)); //iv + (esp trailer + padding)
	}
	else if(strcmp(nameString, CIPHER_AES_CBC) == 0)
	{
		cipherAlg = 0x3; //aes
		cipherMode = 0x1; //cbc
		addedLen += (16 + (16 + 1)); //iv + (esp trailer + padding)

		switch(keyLen << 3) //keyLen*8
		{ 
			case 128:
				aesKeyLen = 0x2;
				break;
			case 192:
				aesKeyLen = 0x3;
				break;
			case 256:
				aesKeyLen = 0x4;
				break;
			default:
				printk("\n !Unsupported AES key length (%d) by EIP93! \n", keyLen << 3);
				errVal = -EPERM;
				goto free_addrsPreComputes;
		}
	}
	else if(strcmp(nameString, CIPHER_NULL_ECB) == 0)
	{
		cipherAlg = 0xf; //null
		cipherMode = 0x0; //ecb
		addedLen += (8 + (16 + 1) + 16); //iv + (esp trailer + padding) + ICV
	}
	else
	{
		printk("\n !Unsupported Cipher Algorithm (%s) by EIP93! \n", nameString);
		errVal = -EPERM;
		goto free_addrsPreComputes;
	}

	
	/* decide hash */
		
	strcpy(nameString, esp->auth.tfm->base.__crt_alg->cra_name);

	if(strcmp(nameString, HASH_MD5_HMAC) == 0)
	{
		hashAlg = 0x0; //md5
		enHmac = 0x1; //hmac
		addedLen += 12; //ICV
	}
	else if(strcmp(nameString, HASH_SHA1_HMAC) == 0)
	{
		hashAlg = 0x1; //sha1
		enHmac = 0x1; //hmac
		addedLen += 12; //ICV
	}
	else if(strcmp(nameString, HASH_SHA256_HMAC) == 0)
	{
		hashAlg = 0x3; //sha256
		enHmac = 0x1; //hmac
		addedLen += 16; //ICV
	}
	else if(strcmp(nameString, HASH_NULL_HMAC) == 0)
	{
		hashAlg = 0xf; //null
		enHmac = 0x1; //hmac
	}
	else
	{
		printk("\n !Unsupported! Hash Algorithm (%s) by EIP93! \n", nameString);
		errVal = -EPERM;
		goto free_addrsPreComputes;
	}
	
	cipherKey =	(unsigned int *)esp->conf.key;
	currAdapterPtr->addedLen = addedLen;
	

	errVal = ipsec_cmdHandler_cmdDescp_set(currAdapterPtr, cmdHandlerDir, cipherAlg, hashAlg, cipherMode, enHmac, aesKeyLen, cipherKey, keyLen, x->id.spi, padCrtlStat);
	if (errVal < 0)
	{
		goto free_addrsPreComputes;
	}


	return 1; //success
	

free_addrsPreComputes:
	ipsec_addrsDigestPreCompute_free(currAdapterPtr);

	return errVal;
}

static int 
ipsec_esp_preProcess(
	struct xfrm_state *x, 
	struct sk_buff *skb,
	unsigned int direction
)
{
	ipsecEip93Adapter_t **ipsecEip93AdapterList;
	unsigned int i, usedEntryNum = 0;
	ipsecEip93Adapter_t *currAdapterPtr;
	unsigned int spi = x->id.spi;
	int currAdapterIdx = -1;
	int err;
	struct esp_data *esp = x->data;
	unsigned int *addrCurrAdapter;
	
	
	if (direction == HASH_DIGEST_OUT)
	{
		ipsecEip93AdapterList = &ipsecEip93AdapterListOut[0];
	}
	else
	{
		ipsecEip93AdapterList = &ipsecEip93AdapterListIn[0];
	}

	//try to find the matched ipsecEip93Adapter for the ipsec flow
	for (i = 0; i < IPESC_EIP93_ADAPTERS; i++)
	{
		if ((currAdapterPtr = ipsecEip93AdapterList[i]) != NULL)
		{
			if (currAdapterPtr->spi == spi)
			{
				currAdapterIdx = i;
				break;
			}
			usedEntryNum++;
		}
		else
		{	//try to record the first unused entry in ipsecEip93AdapterList
			if (currAdapterIdx == -1)
			{
				currAdapterIdx = i;
			}
		}
	}
	
	if (usedEntryNum == IPESC_EIP93_ADAPTERS)
	{
		printk("\n\n !The ipsecEip93AdapterList table is full! \n\n");
		return -EPERM;
	}

	//no ipsecEip93Adapter matched, so create a new one for the ipsec flow. Only the first packet of a ipsec flow will encounter this.
	if (i == IPESC_EIP93_ADAPTERS)
	{
		if (esp->auth.tfm == NULL)
		{
			printk("\n\n !please set a hash algorithm! \n\n");
			return -EPERM;
		}
		else if (esp->conf.tfm == NULL)
		{
			printk("\n\n !please set a cipher algorithm! \n\n");
			return -EPERM;
		}
	
		currAdapterPtr = (ipsecEip93Adapter_t *) kzalloc(sizeof(ipsecEip93Adapter_t), GFP_KERNEL);	
		if(currAdapterPtr == NULL)
		{
			printk("\n\n !!kmalloc for new ipsecEip93Adapter failed!! \n\n");
			return -ENOMEM;
		}
		
		spin_lock_init(&currAdapterPtr->lock);
		skb_queue_head_init(&currAdapterPtr->skbQueue);
				
		err = ipsec_hashDigest_preCompute(x, currAdapterPtr, direction);
		if (err < 0)
		{
			printk("\n\n !ipsec_hashDigest_preCompute for direction:%d failed! \n\n", direction);
			kfree(currAdapterPtr);
			return err;
		}
	
		err = ipsec_cmdHandler_prepare(x, currAdapterPtr, direction);
		if (err < 0)
		{
			printk("\n\n !ipsec_cmdHandler_prepare for direction:%d failed! \n\n", direction);
			kfree(currAdapterPtr);
			return err;
		}		
		
		currAdapterPtr->spi = spi;
		ipsecEip93AdapterList[currAdapterIdx] = currAdapterPtr;
	}
	
	
	currAdapterPtr = ipsecEip93AdapterList[currAdapterIdx];


	if (direction == HASH_DIGEST_IN)
	{
		currAdapterPtr->x = x;
	}

	
	spin_lock(&currAdapterPtr->lock);
	//Hash Digests are ready
	if (currAdapterPtr->isHashPreCompute == 2)
	{	 
		ipsec_hashDigests_get(currAdapterPtr);
		currAdapterPtr->isHashPreCompute = 3; //pre-compute done
		ipsec_addrsDigestPreCompute_free(currAdapterPtr);
	}
	
	spin_unlock(&currAdapterPtr->lock);
	
	//save needed info skb (cryptoDriver will save skb in EIP93's userID), so the needed info can be used by the tasklet which is raised by interrupt.
	addrCurrAdapter = (unsigned int *) &(skb->cb[36]);
	*addrCurrAdapter = (unsigned int)currAdapterPtr;
	
	return 1; //success
}

static int 
ipsec_esp_pktPut(
	struct sk_buff *skb
)
{
	ipsecEip93Adapter_t *currAdapterPtr;
	eip93DescpHandler_t *cmdHandler;
	struct sk_buff *pSkb;
	unsigned int isQueueFull = 0;
	unsigned int addedLen;
	struct sk_buff *skb2;
	struct dst_entry *dst = skb->dst;
	unsigned int *addrCurrAdapter;
	int isOriSkbFree = 0;
	
	
	addrCurrAdapter = (unsigned int *) &(skb->cb[36]);
	currAdapterPtr = (ipsecEip93Adapter_t *)(*addrCurrAdapter);
	cmdHandler = currAdapterPtr->cmdHandler;
	addedLen = currAdapterPtr->addedLen;


	//resemble paged packets if needed
	if (skb_is_nonlinear(skb)) 
	{
		if (skb_linearize(skb) != 0)
		{
			printk("\n !resembling paged packets failed! \n");
			return -EPERM;
		}
		
		//skb_linearize() may return a new skb, so insert currAdapterPtr back to skb!
		addrCurrAdapter = (unsigned int *) &(skb->cb[36]);
		*addrCurrAdapter = (unsigned int)currAdapterPtr;
	}
	
	
	//make sure that tailroom is enough for the added length due to encryption	
	if (skb_tailroom(skb) < addedLen)
	{
		skb2 = skb_copy_expand(skb, skb_headroom(skb), addedLen, GFP_ATOMIC);


		if (skb2 == NULL)
		{
			printk("\n !skb_copy_expand failed! \n");
			return -EPERM;	//old skb will be free.
		}

		kfree_skb(skb); //free original skb
		isOriSkbFree = 1;

		skb = skb2; //the new skb
		skb->dst = dst;
		addrCurrAdapter = (unsigned int *) &(skb->cb[36]);
		*addrCurrAdapter = (unsigned int)currAdapterPtr;
	}

	
	if (currAdapterPtr->skbQueue.qlen < SKB_QUEUE_MAX_SIZE)
	{
		skb_queue_tail(&currAdapterPtr->skbQueue, skb);
	}
	else
	{
		isQueueFull = 1;
	}
		
	//ipsec_BH_handler_resultGet has no chance to set isHashPreCompute as 3, so currAdapterPtr->lock is not needed here!
	if (currAdapterPtr->isHashPreCompute == 3) //pre-compute done
	{		
		spin_lock(&cryptoLock);
		
		while (
			ipsec_eip93CmdResCnt_check() &&
			((pSkb = skb_dequeue(&currAdapterPtr->skbQueue)) != NULL)
		)
		{
			ipsec_packet_put(cmdHandler, pSkb); //mtk_packet_put
		}
		
		spin_unlock(&cryptoLock);
		
		if (
			isQueueFull && 
			(currAdapterPtr->skbQueue.qlen < SKB_QUEUE_MAX_SIZE)
		)
		{
			isQueueFull = 0;
			skb_queue_tail(&currAdapterPtr->skbQueue, skb);
		}
	}

	
	if (isQueueFull == 0)
	{
		return 1; //success
	}
	else
	{
		if (isOriSkbFree)
		{
			kfree_skb(skb); //free the new skb
			return 1;
		}
		else
			return -ENOMEM; //drop the original skb
	}
}

/*_______________________________________________________________________
**function name: ipsec_esp_output_finish
**
**description:
*   Deal with the rest of Linux Kernel's esp_output(). Then,
*	the encrypted packet can do the correct post-routing.
**parameters:
*   resHandler -- point to the result descriptor handler that stores
*		the needed info comming from EIP93's Result Descriptor Ring.
**global:
*   none
**return:
*   none
**call:
*   ip_output()
**revision:
*   1.Trey 20120209
**_______________________________________________________________________*/
static void 
ipsec_esp_output_finish(
	eip93DescpHandler_t *resHandler
)
{
	struct sk_buff *skb = (struct sk_buff *) ipsec_eip93UserId_get(resHandler);
	struct iphdr *top_iph = ip_hdr(skb);
	unsigned int length;
	struct dst_entry *dst = skb->dst;
	struct xfrm_state *x = dst->xfrm;
	
			
	//Try to adjust skb->tail
	length = ipsec_pktLength_get(resHandler);
	skb_put(skb, length - skb->len); //adjust skb->tail
		
	length += skb->data - skb_network_header(skb); //IP total length

	//let skb->data point to the outer IP header
	__skb_push(skb, skb->data - skb_network_header(skb));

	top_iph->tot_len = htons(length);
	top_iph->protocol = IPPROTO_ESP;
			 
	ip_send_check(top_iph);
	
		
	/* adjust for IPSec post-routing */
	
	spin_lock_bh(&x->lock);
	x->curlft.bytes += skb->len;
	x->curlft.packets++;
	spin_unlock_bh(&x->lock);

	if (!(skb->dst = dst_pop(dst))) {
		kfree_skb(skb);
		return;
	}

	nf_reset(skb);
	
	if (!skb->dst->xfrm)
	{		
		ip_output(skb);
	}	
}

/* this inline function comes from xfrm4_input.c in Linux Kernel */
#ifdef CONFIG_NETFILTER
static inline int xfrm4_rcv_encap_finish(struct sk_buff *skb)
{
	if (skb->dst == NULL) {
		const struct iphdr *iph = ip_hdr(skb);

		if (ip_route_input(skb, iph->daddr, iph->saddr, iph->tos,
				   skb->dev))
			goto drop;
	}
	return dst_input(skb);
drop:
	kfree_skb(skb);
	return NET_RX_DROP;
}
#endif

/*_______________________________________________________________________
**function name: ipsec_esp_input_finish
**
**description:
*   Deal with the rest of Linux Kernel's esp_input(). Then,
*	the decrypted packet can do the correct post-routing.
**parameters:
*   resHandler -- point to the result descriptor handler that stores
*		the needed info comming from EIP93's Result Descriptor Ring.
*   x -- point to the structure that stores IPSec SA information
**global:
*   none
**return:
*   none
**call:
*   netif_rx() for tunnel mode, or xfrm4_rcv_encap_finish() for transport
*		mode.
**revision:
*   1.Trey 20120209
**_______________________________________________________________________*/
static void 
ipsec_esp_input_finish(
	eip93DescpHandler_t *resHandler, 
	struct xfrm_state *x
)
{
	struct sk_buff *skb = (struct sk_buff *) ipsec_eip93UserId_get(resHandler);
	struct iphdr *iph;
	unsigned int ihl, pktLen;
	struct esp_data *esp = x->data;
	struct xfrm_state *xfrm_vec[XFRM_MAX_DEPTH];
	int xfrm_nr = 0;
	int decaps = 0;
	__be32 spi, seq;
	int err;


	skb->ip_summed = CHECKSUM_NONE;	
	iph = ip_hdr(skb);
	ihl = iph->ihl << 2; //iph->ihl * 4	
	iph->protocol = ipsec_espNextHeader_get(resHandler);
	
		
	//adjest skb->tail & skb->len
	pktLen = ipsec_pktLength_get(resHandler);
	
	//pskb_trim(skb, pktLen);
	skb->len = pktLen;
	skb_set_tail_pointer(skb, pktLen);
	//adjust skb->data
	__skb_pull(skb, esp->conf.ivlen);

	
	skb_set_transport_header(skb, -ihl); //for transport mode

	
	/* adjust for IPSec post-routing */

	spin_lock(&x->lock);
	x->curlft.bytes += skb->len;
	x->curlft.packets++;
	spin_unlock(&x->lock);

	xfrm_vec[xfrm_nr++] = x;
	
	//xfrm4_tunnel_input(), xfrm4_transport_input()
	if (x->mode->input(x, skb)) 
		goto drop;
						
	if (x->props.mode == XFRM_MODE_TUNNEL) {
		decaps = 1;
	}
	else
	{
		err = xfrm_parse_spi(skb, ip_hdr(skb)->protocol, &spi, &seq);
		if (err < 0)
			goto drop;
	}
	
	/* Allocate new secpath or COW existing one. */

	if (!skb->sp || atomic_read(&skb->sp->refcnt) != 1) {
		struct sec_path *sp;
		sp = secpath_dup(skb->sp);
		if (!sp)
			goto drop;
		if (skb->sp)
			secpath_put(skb->sp);
		skb->sp = sp;
	}
	if (xfrm_nr + skb->sp->len > XFRM_MAX_DEPTH)
		goto drop;

	memcpy(skb->sp->xvec + skb->sp->len, xfrm_vec,
	       xfrm_nr * sizeof(xfrm_vec[0]));
	skb->sp->len += xfrm_nr;

	nf_reset(skb);

	if (decaps) {
		dst_release(skb->dst);
		skb->dst = NULL;
		netif_rx(skb);
		return;
	} else {
#ifdef CONFIG_NETFILTER
		__skb_push(skb, skb->data - skb_network_header(skb));
		ip_hdr(skb)->tot_len = htons(skb->len);
		ip_send_check(ip_hdr(skb));

		NF_HOOK(PF_INET, NF_IP_PRE_ROUTING, skb, skb->dev, NULL,
			xfrm4_rcv_encap_finish);
#endif
		return;
	}


drop:
	while (--xfrm_nr >= 0)
		xfrm_state_put(xfrm_vec[xfrm_nr]);

	kfree_skb(skb);
	return;
}


/************************************************************************
*              P U B L I C     F U N C T I O N S
*************************************************************************
*/
void 
ipsec_eip93Adapter_free(
	unsigned int spi
)
{
	unsigned int i;
	ipsecEip93Adapter_t *currAdapterPtr;

	
	for (i = 0; i < IPESC_EIP93_ADAPTERS; i++)
	{
		if ((currAdapterPtr = ipsecEip93AdapterListOut[i]) != NULL)
		{
			if (currAdapterPtr->spi == spi)
			{
				ipsec_cmdHandler_free(currAdapterPtr->cmdHandler);
				kfree(currAdapterPtr);
				ipsecEip93AdapterListOut[i] = NULL;
				return;
			}
		}
	}
	
	for (i = 0; i < IPESC_EIP93_ADAPTERS; i++)
	{
		if ((currAdapterPtr = ipsecEip93AdapterListIn[i]) != NULL)
		{
			if (currAdapterPtr->spi == spi)
			{
				ipsec_cmdHandler_free(currAdapterPtr->cmdHandler);
				kfree(currAdapterPtr);
				ipsecEip93AdapterListIn[i] = NULL;
				return;
			}
		}
	}
}

/*_______________________________________________________________________
**function name: ipsec_esp_output
**
**description:
*   Replace Linux Kernel's esp_output(), in order to use EIP93
*	to do encryption for a IPSec ESP flow.
**parameters:
*   x -- point to the structure that stores IPSec SA information
*	skb -- the packet that is going to be encrypted.
**global:
*   none
**return:
*   -EPERM, -ENOMEM -- failed: the pakcet will be dropped!
*	1 -- success: the packet's command decsriptor is put into
*		EIP93's Command Descriptor Ring.
**call:
*   none
**revision:
*   1.Trey 20120209
**_______________________________________________________________________*/
int 
ipsec_esp_output(
	struct xfrm_state *x, 
	struct sk_buff *skb
)
{
	ipsecEip93Adapter_t *currAdapterPtr;
	int err;
	eip93DescpHandler_t *cmdHandler;
	struct iphdr *top_iph = ip_hdr(skb);
	unsigned int *addrCurrAdapter;


	err = ipsec_esp_preProcess(x, skb, HASH_DIGEST_OUT);
	if (err < 0)
	{
		printk("\n\n ipsec_esp_preProcess for HASH_DIGEST_OUT failed! \n\n");
		return err;
	}

	addrCurrAdapter = (unsigned int *) &(skb->cb[36]);
	currAdapterPtr = (ipsecEip93Adapter_t *)(*addrCurrAdapter);
	cmdHandler = currAdapterPtr->cmdHandler;
		

	/* this is non-NULL only with UDP Encapsulation for NAT-T */
	if (unlikely(x->encap)) 
	{		
		printk("\n\n NAT-T is not supported yet! \n\n");
		return -EPERM;
	}

	/* in case user will change between tunnel and transport mode,
	 * we have to set "padValue" every time before every packet 
	 * goes into EIP93 for esp outbound! */
	ipsec_espNextHeader_set(cmdHandler, top_iph->protocol);
			
	//let skb->data point to the payload which is going to be encrypted
	__skb_pull(skb, skb_transport_offset(skb));
	
	
	return ipsec_esp_pktPut(skb);
}

/*_______________________________________________________________________
**function name: ipsec_esp_input
**
**description:
*   Replace Linux Kernel's esp_input(), in order to use EIP93
*	to do decryption for a IPSec ESP flow.
**parameters:
*   x -- point to the structure that stores IPSec SA information
*	skb -- the packet that is going to be decrypted.
**global:
*   none
**return:
*   -EPERM, -ENOMEM -- failed: the pakcet will be dropped!
*	1 -- success: the packet's command decsriptor is put into
*		EIP93's Command Descriptor Ring.
**call:
*   none
**revision:
*   1.Trey 20120209
**_______________________________________________________________________*/
int 
ipsec_esp_input(
	struct xfrm_state *x, 
	struct sk_buff *skb
)
{
	int err;
	struct esp_data *esp = x->data;
	struct crypto_blkcipher *tfm = esp->conf.tfm;
	int blksize = ALIGN(crypto_blkcipher_blocksize(tfm), 4);
	int alen = esp->auth.icv_trunc_len;
	int elen = skb->len - sizeof(struct ip_esp_hdr) - esp->conf.ivlen - alen;
	

	err = ipsec_esp_preProcess(x, skb, HASH_DIGEST_IN);
	if (err < 0)
	{
		printk("\n\n ipsec_esp_preProcess for HASH_DIGEST_IN failed! \n\n");
		return err;
	}
		

	if (!pskb_may_pull(skb, sizeof(struct ip_esp_hdr)))
		goto out;
		
	if (elen <= 0 || (elen & (blksize-1)))
		goto out;

	if (x->encap) 
	{
		printk("\n !NAT-T is not supported! \n");
		goto out;
	}
		

	return ipsec_esp_pktPut(skb);
	
out:
	printk("\n Something's wrong! Go out! \n");
	return -EINVAL;
}

/************************************************************************
*              E X T E R N E L     F U N C T I O N S
*************************************************************************
*/
/*_______________________________________________________________________
**function name: ipsec_eip93_adapters_init
**
**description:
*   initialize ipsecEip93AdapterListOut[] and ipsecEip93AdapterListIn[]
*	durin EIP93's initialization.
**parameters:
*   none
**global:
*   none
**return:
*   none
**call:
*   none
**revision:
*   1.Trey 20120209
**_______________________________________________________________________*/
void 
ipsec_eip93_adapters_init(
	void
)
{
	unsigned int i;
	
	for (i = 0; i < IPESC_EIP93_ADAPTERS; i++)
	{
		ipsecEip93AdapterListOut[i] = NULL;
		ipsecEip93AdapterListIn[i] = NULL;
	}
}

/*_______________________________________________________________________
**function name: ipsec_cryptoLock_init
**
**description:
*   initialize cryptoLock durin EIP93's initialization. cryptoLock is
*	used to make sure only one process can access EIP93 at a time.
**parameters:
*   none
**global:
*   none
**return:
*   none
**call:
*   none
**revision:
*   1.Trey 20120209
**_______________________________________________________________________*/
void 
ipsec_cryptoLock_init(
	void
)
{
	spin_lock_init(&cryptoLock);
}

EXPORT_SYMBOL(ipsec_eip93_adapters_init);
EXPORT_SYMBOL(ipsec_cryptoLock_init);

/*_______________________________________________________________________
**function name: ipsec_BH_handler_resultGet
**
**description:
*   This tasklet is raised by EIP93's interrupt after EIP93 finishs
*	a command descriptor and puts the result in Result Descriptor Ring.
*	This tasklet gets a result descriptor from EIP93 at a time and do
*	the corresponding atcion until all results from EIP93 are finished.
**parameters:
*   none
**global:
*   none
**return:
*   none
**call:
*   ipsec_esp_output_finish() when the result is for encryption.
*	ipsec_esp_input_finish() when the result is for decryption.
**revision:
*   1.Trey 20120209
**_______________________________________________________________________*/
void 
ipsec_BH_handler_resultGet(
	void
)
{
	int retVal;
	struct sk_buff *skb;
	ipsecEip93Adapter_t *currAdapterPtr;
	unsigned int *addrCurrAdapter;

	
	while (1)
	{
		memset(&resDescpHandler, 0, sizeof(eip93DescpHandler_t));
		
		retVal = ipsec_packet_get(&resDescpHandler);
		
		//got the correct result from eip93
		if (likely(retVal == 1))
		{
			//the result is for encrypted or encrypted packet
			if (ipsec_eip93HashFinal_get(&resDescpHandler) == 0x1)
			{
				skb = (struct sk_buff *) ipsec_eip93UserId_get(&resDescpHandler);
				
				addrCurrAdapter = (unsigned int *) &(skb->cb[36]);
				currAdapterPtr = (ipsecEip93Adapter_t *)(*addrCurrAdapter);
				
				if (currAdapterPtr->isEncryptOrDecrypt == CRYPTO_ENCRYPTION)
				{
					ipsec_esp_output_finish(&resDescpHandler);
				}
				else if (currAdapterPtr->isEncryptOrDecrypt == CRYPTO_DECRYPTION)
				{
					ipsec_esp_input_finish(&resDescpHandler, currAdapterPtr->x);
				}
				else
				{
					printk("\n\n !can't tell encrypt or decrypt! \n\n");
					return;
				}
			}
			//the result is for inner and outer hash digest pre-compute
			else
			{
				currAdapterPtr = (ipsecEip93Adapter_t *) ipsec_eip93UserId_get(&resDescpHandler);
				
				spin_lock(&currAdapterPtr->lock);
				//for the inner digests			
				if (currAdapterPtr->isHashPreCompute == 0)
				{
					//resDescpHandler only has physical addresses, so we have to get saState's virtual address from addrsPreCompute.
					ipsec_hashDigests_set(currAdapterPtr, 1);
					
					//inner digest done
					currAdapterPtr->isHashPreCompute = 1; 
				}
				//for the outer digests	
				else if (currAdapterPtr->isHashPreCompute == 1)
				{
					ipsec_hashDigests_set(currAdapterPtr, 2);
					//outer digest done
					currAdapterPtr->isHashPreCompute = 2;				
				}
				else
				{
					printk("\n\n !can't tell inner or outer digests! \n\n");
					return;
				}
							
				spin_unlock(&currAdapterPtr->lock);
			}
		}
		//if packet is not done, don't wait! (for speeding up)
		else if (retVal == 0)
		{
			break;
		}
	} //end while (1)
	
	return;
}
EXPORT_SYMBOL(ipsec_BH_handler_resultGet);

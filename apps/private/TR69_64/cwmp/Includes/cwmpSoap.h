#ifndef __CWMPSOAP_H__
#define __CWMPSOAP_H__

//#include "cwmpRpc.h"

#if 1
#define kCwmpEnvelopeTag            "SOAP-ENV:Envelope" /* wclai: match Microsoft packet */

#define kCwmpHeaderTag                "SOAP-ENV:Header" /* wclai: match Microsoft packet */

#define kCwmpBodyTag                "SOAP-ENV:Body" /* wclai: match Microsoft packet */

#define kCwmpEncodingStyleAttribute "SOAP-ENV:encodingStyle" /* wclai: match Microsoft packet */

#define kCwmpXmlns             " xmlns:SOAP-ENV" /* wclai: match Microsoft packet */

#define kCwmpEncXmlns             "xmlns:SOAP-ENC"

#define kCwmpEncArray             "SOAP-ENC:Array"

#define kCwmpEncArrayType             "SOAP-ENC:arrayType"

#define kCwmpEnvMustUnderStand             "SOAP-ENV:mustUnderstand"

#define kCwmpFaultTag               "SOAP-ENV:Fault" /* wclai: match Microsoft packet */

#else
#define kCwmpEnvelopeTag            "soap:Envelope" /* wclai: match Microsoft packet */

#define kCwmpHeaderTag                "soap:Header" /* wclai: match Microsoft packet */

#define kCwmpBodyTag                "soap:Body" /* wclai: match Microsoft packet */

#define kCwmpEncodingStyleAttribute "soap:encodingStyle" /* wclai: match Microsoft packet */

#define kCwmpXmlns             " xmlns:soap" /* wclai: match Microsoft packet */

#define kCwmpEncXmlns             "xmlns:soap-enc"

#define kCwmpEncArray             "soap-enc:Array"

#define kCwmpEncArrayType             "soap-enc:arrayType"

#define kCwmpEnvMustUnderStand             "soap:mustUnderstand"

#define kCwmpFaultTag               "soap:Fault" /* wclai: match Microsoft packet */

#endif
// RPC
int cwmpSoapInform(cwmp_rpc_t *, InformEventIndex eventIndex);
int cwmpSoapRebootResponse(cwmp_rpc_t *);

// General

int	cwmpSoapHead(cwmp_rpc_t *);
int	cwmpSoapDeviceID(cwmp_rpc_t *,DeviceIdStruct);
int	cwmpSoapEvent(cwmp_rpc_t *,EventStruct);
int	cwmpSoapParameterValueStruct(cwmp_rpc_t *);


#endif //__CWMPSOAP_H__

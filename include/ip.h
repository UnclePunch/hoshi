/*
    IP library decompilation was sourced from https://github.com/Cuyler36/dolsocketlibrary
*/

#ifndef __DOLPHIN_OS_IP_H__
#define __DOLPHIN_OS_IP_H__

#include <ip/IFQueue.h>
#include <ip/IP.h>
#include <ip/IPIgmp.h>
#include <ip/IPIcmp.h>
#include <ip/IPRoute.h>
#include <ip/IPTcpTimeWait.h>
#include <ip/IPOpt.h>
#include <ip/IPFrag.h>
#include <ip/IPTcp.h>
#include <ip/IPUdp.h>
#include <ip/IPSocket.h>
#include <ip/IFFifo.h>
#include <ip/IPDns.h>
#include <ip/IPPpp.h>
#include <ip/IPDhcp.h>
#include <ip/IPEther.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IPU32(ipBytes) (*(u32*)(ipBytes))
#define IPEQ(ip0, ip1) (IPU32(ip0) == IPU32(ip1))
#define IPNEQ(ip0, ip1) (IPU32(ip0) != IPU32(ip1))

#define IP_CLASSA(ip) (((ip)[0] & 0x80) == 0x00) /*   0 - 127 */
#define IP_CLASSB(ip) (((ip)[0] & 0xC0) == 0x80) /* 128 - 191 */
#define IP_CLASSC(ip) (((ip)[0] & 0xE0) == 0xC0) /* 192 - 223 */
#define IP_CLASSD(ip) (((ip)[0] & 0xF0) == 0xE0) /* 224 - 239 | Multicasting */
#define IP_CLASSE(ip) (((ip)[0] & 0xF8) == 0xF0) /* 240 - 255 | Experimental */

extern const u8 IPAddrAny[4];

char* IPAtoN(const char* dotted, u8* addr);
int IPRecoverGateway(const u8* dst);
int IPAutoConfig(void);
s32 IPSetConfigError(IPInterface* interface, s32 err);

#ifdef __cplusplus
}
#endif

#endif

#ifndef __DOLPHIN_OS_IP_ROUTE_H__
#define __DOLPHIN_OS_IP_ROUTE_H__

#include <ip/IP.h>

#ifdef __cplusplus
extern "C" {
#endif

IPInterface* IPGetRoute(const u8* addr, u8* dst);
int IPIsBroadcastAddr(IPInterface* interface, const u8* addr);
int IPIsLoopbackAddr(IPInterface* interface, const u8* addr);
void IPSetMtu(IPInterface * interface /* r31 */, s32 mtu /* r30 */);

#ifdef __cplusplus
}
#endif

#endif

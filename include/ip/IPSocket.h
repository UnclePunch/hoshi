#ifndef __DOLPHIN_OS_IP_SOCKET_H__
#define __DOLPHIN_OS_IP_SOCKET_H__

#include <ip/IP.h>
#include <ip/IPDns.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SO_VENDOR_NINTENDO  0x0000      // Nintendo TCP/IP stack
#define SO_VERSION          0x0100      // Version 1.0

#define SO_PF_UNSPEC 0 // Unspecified
#define SO_PF_INET 2   // ARPA Internet protocols

#define SO_INET_ADDRSTRLEN  16

#define SO_SOCK_STREAM 1 // stream socket
#define SO_SOCK_DGRAM 2  // datagram socket

#define SO_MSG_OOB 0x01      // send or receive out-of-band data
#define SO_MSG_PEEK 0x02     // take data but leave it
#define SO_MSG_DONTWAIT 0x04 // non-block operation

// Socket option levels
#define SO_SOL_SOCKET 0xffff // options for socket level
#define SO_SOL_IP 0
#define SO_SOL_ICMP 1
#define SO_SOL_TCP 6
#define SO_SOL_UDP 17

#define SO_INADDR_ANY (0x00000000)             // 0.0.0.0
#define SO_INADDR_BROADCAST (0xffffffff)       // 255.255.255.255
#define SO_INADDR_LOOPBACK (0x7f000001)        // 127.0.0.1
#define SO_INADDR_UNSPEC_GROUP (0xe0000000)    // 224.0.0.0
#define SO_INADDR_ALLHOSTS_GROUP (0xe0000001)  // 224.0.0.1
#define SO_INADDR_MAX_LOCAL_GROUP (0xe00000ff) // 224.0.0.255

// SOGetSockOpt() / SOSetSockOpt()
#define SO_IP_TOS 0x00000007             // int
#define SO_IP_TTL 0x00000008             // int
#define SO_IP_MULTICAST_LOOP 0x00000009  // unsigned char
#define SO_IP_MULTICAST_TTL 0x0000000a   // unsigned char
#define SO_IP_ADD_MEMBERSHIP 0x0000000b  // SOIpMreq
#define SO_IP_DROP_MEMBERSHIP 0x0000000c // SOIpMreq
#define SO_SO_REUSEADDR 0x00000004       // BOOL
#define SO_SO_LINGER 0x00000080          // SOLinger
#define SO_SO_OOBINLINE 0x00000100       // BOOL
#define SO_SO_SNDBUF 0x00001001          // int
#define SO_SO_RCVBUF 0x00001002          // int
#define SO_SO_TYPE 0x00001008            // int
#define SO_TCP_NODELAY 0x00002001        // BOOL
#define SO_TCP_MAXSEG 0x00002002         // int

// SOShutdown()
#define SO_SHUT_RD 0
#define SO_SHUT_WR 1
#define SO_SHUT_RDWR 2

// SOFcntl()
#define SO_F_GETFL 3
#define SO_F_SETFL 4
#define SO_O_NONBLOCK 0x04

// SOPoll()
#define SO_POLLRDNORM 0x0001 // Normal data read
#define SO_POLLRDBAND 0x0002 // Priority data read
#define SO_POLLPRI 0x0004    // High priority data read
#define SO_POLLWRNORM 0x0008 // Normal data write
#define SO_POLLWRBAND 0x0010 // Priority data write
#define SO_POLLERR 0x0020    // Error (revents only)
#define SO_POLLHUP 0x0040    // Disconnected (revents only)
#define SO_POLLNVAL 0x0080   // Invalid fd (revents only)
#define SO_POLLIN (SO_POLLRDNORM | SO_POLLRDBAND)
#define SO_POLLOUT SO_POLLWRNORM

#define SO_INFTIM -1

#define SO_E2BIG -1
#define SO_EACCES -2
#define SO_EADDRINUSE -3 // Address is already in use
#define SO_EADDRNOTAVAIL -4
#define SO_EAFNOSUPPORT -5 // Non-supported address family
#define SO_EAGAIN -6       // Posix.1
#define SO_EALREADY -7     // Already in progress
#define SO_EBADF -8        // Bad socket descriptor
#define SO_EBADMSG -9
#define SO_EBUSY -10
#define SO_ECANCELED -11
#define SO_ECHILD -12
#define SO_ECONNABORTED -13 // Connection aborted
#define SO_ECONNREFUSED -14 // Connection refused
#define SO_ECONNRESET -15   // Connection reset
#define SO_EDEADLK -16
#define SO_EDESTADDRREQ -17 // Not bound to a local address
#define SO_EDOM -18
#define SO_EDQUOT -19
#define SO_EEXIST -20
#define SO_EFAULT -21
#define SO_EFBIG -22
#define SO_EHOSTUNREACH -23
#define SO_EIDRM -24
#define SO_EILSEQ -25
#define SO_EINPROGRESS -26
#define SO_EINTR -27   // Canceled
#define SO_EINVAL -28  // Invalid operation
#define SO_EIO -29     // I/O error
#define SO_EISCONN -30 // Socket is already connected
#define SO_EISDIR -31
#define SO_ELOOP -32
#define SO_EMFILE -33 // No more socket descriptors
#define SO_EMLINK -34
#define SO_EMSGSIZE -35 // Too large to be sent
#define SO_EMULTIHOP -36
#define SO_ENAMETOOLONG -37
#define SO_ENETDOWN -38
#define SO_ENETRESET -39
#define SO_ENETUNREACH -40 // Unreachable
#define SO_ENFILE -41
#define SO_ENOBUFS -42 // Insufficient resources
#define SO_ENODATA -43
#define SO_ENODEV -44
#define SO_ENOENT -45
#define SO_ENOEXEC -46
#define SO_ENOLCK -47
#define SO_ENOLINK -48
#define SO_ENOMEM -49 // Insufficient memory
#define SO_ENOMSG -50
#define SO_ENOPROTOOPT -51 // Non-supported option
#define SO_ENOSPC -52
#define SO_ENOSR -53
#define SO_ENOSTR -54
#define SO_ENOSYS -55
#define SO_ENOTCONN -56 // Not connected
#define SO_ENOTDIR -57
#define SO_ENOTEMPTY -58
#define SO_ENOTSOCK -59 // Not a socket
#define SO_ENOTSUP -60
#define SO_ENOTTY -61
#define SO_ENXIO -62
#define SO_EOPNOTSUPP -63 // Non-supported operation
#define SO_EOVERFLOW -64
#define SO_EPERM -65
#define SO_EPIPE -66
#define SO_EPROTO -67
#define SO_EPROTONOSUPPORT -68 // Non-supported protocol
#define SO_EPROTOTYPE -69      // Non-supported socket type
#define SO_ERANGE -70
#define SO_EROFS -71
#define SO_ESPIPE -72
#define SO_ESRCH -73
#define SO_ESTALE -74
#define SO_ETIME -75
#define SO_ETIMEDOUT -76 // Timed out
#define SO_ETXTBSY -77
#define SO_EWOULDBLOCK SO_EAGAIN // Posix.1g
#define SO_EXDEV -78

// Name for SOConfig.alloc() and SOConfig.free()
#define SO_MEM_TCP_INFO 0
#define SO_MEM_TCP_SENDBUF 1
#define SO_MEM_TCP_RECVBUF 2
#define SO_MEM_UDP_INFO 3
#define SO_MEM_UDP_SENDBUF 4
#define SO_MEM_UDP_RECVBUF 5
#define SO_MEM_TIMEWAITBUF 6
#define SO_MEM_REASSEMBLYBUF 7

// Flags for SOConfig{}.flag
#define SO_FLAG_DHCP 0x0001  // Use DHCP
#define SO_FLAG_PPPoE 0x0002 // Use PPPoE
#define SO_FLAG_ZEROCONF 0x8000 // Use AutoIP

#define SO_MTU_MAX 1500

#define SO_GET_CONFIG_MTU(config) ((config)->mtu >= SO_MTU_MAX ? SO_MTU_MAX : (config)->mtu)

#define SONtoHl(netlong) ((u32)(netlong))
#define SONtoHs(netshort) ((u16)(netshort))
#define SOHtoNl(hostlong) ((u32)(hostlong))
#define SOHtoNs(hostshort) ((u16)(hostshort))

typedef struct SOInAddr {
    // total size: 0x4
    u32 addr; // offset 0x0, size 0x4
} SOInAddr;

typedef struct SOSockAddrIn {
    u8 len;
    u8 family;
    u16 port;
    SOInAddr addr;
} SOSockAddrIn;

typedef struct SOIpMreq {
    // total size: 0x8
    SOInAddr multiaddr; // offset 0x0, size 0x4
    SOInAddr interface; // offset 0x4, size 0x4
} SOIpMreq;

typedef struct SONode {
    // total size: 0x38
    u8 proto; // offset 0x0, size 0x1
    u8 flag; // offset 0x1, size 0x1
    s16 ref; // offset 0x2, size 0x2
    IPInfo* info; // offset 0x4, size 0x4
    OSMutex mutexRead; // offset 0x8, size 0x18
    OSMutex mutexWrite; // offset 0x20, size 0x18
} SONode;

typedef struct SOSockAddr {
    // total size: 0x8
    u8 len; // offset 0x0, size 0x1
    u8 family; // offset 0x1, size 0x1
    u8 data[6]; // offset 0x2, size 0x6
} SOSockAddr;

typedef struct SOHostEnt {
    // total size: 0x10
    char* name; // offset 0x0, size 0x4
    char** aliases; // offset 0x4, size 0x4
    s16 addrType; // offset 0x8, size 0x2
    s16 length; // offset 0xA, size 0x2
    u8** addrList; // offset 0xC, size 0x4
} SOHostEnt;

typedef struct SOResolver {
    // total size: 0x790
    DNSInfo info; // offset 0x0, size 0x560
    SOHostEnt ent; // offset 0x560, size 0x10
    char name[256]; // offset 0x570, size 0x100
    char* zero; // offset 0x670, size 0x4
    u8 addrList[140]; // offset 0x674, size 0x8C
    u8* ptrList[36]; // offset 0x700, size 0x90
} SOResolver;

typedef void* (*SOAllocFunc)(u32, s32);
typedef void (*SOFreeFunc)(u32, void*, s32);

typedef struct SOConfig {
    // total size: 0x60
    u16 vendor; // offset 0x0, size 0x2
    u16 version; // offset 0x2, size 0x2
    SOAllocFunc alloc; // offset 0x4, size 0x4
    SOFreeFunc free; // offset 0x8, size 0x4
    u32 flag; // offset 0xC, size 0x4
    SOInAddr addr; // offset 0x10, size 0x4
    SOInAddr netmask; // offset 0x14, size 0x4
    SOInAddr router; // offset 0x18, size 0x4
    SOInAddr dns1; // offset 0x1C, size 0x4
    SOInAddr dns2; // offset 0x20, size 0x4
    s32 timeWaitBuffer; // offset 0x24, size 0x4
    s32 reassemblyBuffer; // offset 0x28, size 0x4
    s32 mtu; // offset 0x2C, size 0x4
    s32 rwin; // offset 0x30, size 0x4
    OSTime r2; // offset 0x38, size 0x8
    const char* peerid; // offset 0x40, size 0x4
    const char* passwd; // offset 0x44, size 0x4
    const char* serviceName; // offset 0x48, size 0x4
    const char* hostName; // offset 0x4C, size 0x4
    s32 rdhcp; // offset 0x50, size 0x4
    s32 udpSendBuff; // offset 0x54, size 0x4
    s32 udpRecvBuff; // offset 0x58, size 0x4
} SOConfig;

typedef struct SOLinger {
    // total size: 0x8
    int onoff; // offset 0x0, size 0x4
    int linger; // offset 0x4, size 0x4
} SOLinger;

typedef struct SOPollFD {
    // total size: 0x8
    int fd; // offset 0x0, size 0x4
    s16 events; // offset 0x4, size 0x2
    s16 revents; // offset 0x6, size 0x2
} SOPollFD;

int SOGetHostID();

int SOSocket(int af, int type, int protocol);
int SOClose(int s);
int SOShutdown(int s, int how);

int SOBind(int s, void *sockAddr);
int SOConnect(int s, void *sockAddr);

int SORead(int s, void *buf, int len);
int SORecvFrom(int s, void *buf, int len, int flags,
               void *sockFrom);

int SOWrite(int s, void *buf, int len);
int SOSendTo(int s, void *buf, int len, int flags,
             void *sockTo);

int SOAccept(int s, void *sockAddr);
int SOListen(int s, int backlog);

int SOSetSockOpt(int s, int level, int optname,
                 void *optval, int optlen);

int SOFcntl(int s, int cmd, ...);

int SOPoll(SOPollFD fds[], unsigned nfds, OSTime timeout);

void SOInit(void);
int SOStartup(SOConfig *config);
int SOCleanup(void);

int SOSetResolver(SOInAddr *dns1, SOInAddr *dns2);
int SOGetResolver(SOInAddr *dns1, SOInAddr *dns2);


#ifdef __cplusplus
}
#endif

#endif

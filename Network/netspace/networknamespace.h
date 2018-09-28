#ifndef NETWORKNAMESPACE_H
#define NETWORKNAMESPACE_H
#include "config/usertypedef.h"
#include "netbuff/ccyclebuffer.h"
#include "connect/socketconnectudp.h"

//class CCycleBuffer;
//class SocketConnectUDP;
namespace networkNameSpace
{
    extern SocketConnectUDP* pSocketClass;
    extern CCycleBuffer* pCycleBuffClass;
}


#endif // NETWORKNAMESPACE_H

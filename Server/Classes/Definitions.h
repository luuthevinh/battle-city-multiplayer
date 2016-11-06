#ifndef __DEFINITIONS_SERVER_H__
#define __DEFINITIONS_SERVER_H__

#include <WinSock2.h>

#include "Shared\SharedDefinitions.h"

#define PORT 1495
#define DATA_BUFSIZE 8192

#define GAME_FRAMERATE 30

typedef struct _SocketInfo {
	CHAR buffer[DATA_BUFSIZE];
	WSABUF dataBuf;
	SOCKET socket;
	OVERLAPPED overlapped;
	DWORD bytesSend;
	DWORD bytesRecv;
} SocketInfo, *pSocketInfo;

#endif // !__DEFINITIONS_SERVER_H__

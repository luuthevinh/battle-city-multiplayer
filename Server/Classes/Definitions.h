#ifndef __DEFINITIONS_SERVER_H__
#define __DEFINITIONS_SERVER_H__

#include <WinSock2.h>

#define PORT 1495
#define DATA_BUFSIZE 8192

typedef struct _SocketInfo {
	CHAR buffer[DATA_BUFSIZE];
	WSABUF dataBuf;
	SOCKET socket;
	OVERLAPPED overlapped;
	DWORD bytesSend;
	DWORD bytesRecv;
} SocketInfo, *pSocketInfo;

enum eDirection
{
	LEFT = 1,
	UP = 2,
	RIGHT = 3,
	DOWN = 4
};

struct ObjectPacket {
	int id;
	int direction;
	float x;
	float y;
	float dx;
	float dy;
};

#define TANK_NORMAL_VELOCITY 100

#endif // !__DEFINITIONS_SERVER_H__

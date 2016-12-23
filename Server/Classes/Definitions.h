#ifndef __DEFINITIONS_SERVER_H__
#define __DEFINITIONS_SERVER_H__

#include <WinSock2.h>
#include <assert.h>

#include "Shared\SharedDefinitions.h"

#define LOCALHOST "127.0.0.1"
#define PORT 1495
#define DATA_BUFFER_SIZE 51200			// 5 Mb
#define DATA_BUFFER_RECIEVE_SIZE 1024	// 1 Mb

#define GAME_FRAMERATE 60

#define MAX_NUMBER_OF_BOTS 50

// uncomment to disable assert
// #define NDEBUG

#define ASSERT_MSG(cond, message) do { \
    if (!(cond)) { \
		assert(cond); \
    } \
} while (0)

#endif // !__DEFINITIONS_SERVER_H__

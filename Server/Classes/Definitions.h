#ifndef __DEFINITIONS_SERVER_H__
#define __DEFINITIONS_SERVER_H__

#include <WinSock2.h>
#include <assert.h>

#include "Shared\SharedDefinitions.h"

#define PORT 1495
#define DATA_BUFFER_SIZE 5120			// 5 Mb
#define DATA_BUFFER_RECIEVE_SIZE 1024	// 1 Mb

#define GAME_FRAMERATE 60

// uncomment to disable assert
// #define NDEBUG

#define ASSERT_MSG(cond, message) do { \
    if (!(cond)) { \
		assert(cond); \
    } \
} while (0)

#endif // !__DEFINITIONS_SERVER_H__

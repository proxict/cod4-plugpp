#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_

#include "pinc.h"

extern "C" {
enum svscmd_type {
    SV_CMD_CAN_IGNORE = 0x0,
    SV_CMD_RELIABLE = 0x1,
};

typedef enum {
    // bk001129 - make sure SE_NONE is zero
    SE_NONE = 0, // evTime is still valid
    SE_CONSOLE,  // evPtr is a char*
    SE_PACKET    // evPtr is a netadr_t followed by data bytes to evPtrLength
} sysEventType_t;

void Info_SetValueForKey(char* s, const char* key, const char* value);
char* Info_ValueForKey(const char* s, const char* key);
void Q_strncpyz(char* dest, const char* src, int destsize);
void SV_UpdateClientConfigInfo(client_t* cl);
void SV_DropClientNoNotify(client_t* drop, const char* reason);
void SV_GameSendServerCommand(int clientNum, svscmd_type type, const char* text);
void* Z_TagMalloc(int size, int tag); // NOT 0 filled memory
void* Z_Malloc(int size);             // returns 0 filled memory
void* S_Malloc(int size);             // NOT 0 filled memory only for small allocations
void Com_QueueEvent(int time, sysEventType_t type, int value, int value2, int ptrLength, void* ptr);
}

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_

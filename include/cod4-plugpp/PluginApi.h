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

enum SCR_TYPES {
    VAR_UNDEFINED = 0x0,
    VAR_BEGIN_REF = 0x1,
    VAR_POINTER = 0x1,
    VAR_STRING = 0x2,
    VAR_ISTRING = 0x3,
    VAR_VECTOR = 0x4,
    VAR_END_REF = 0x5,
    VAR_FLOAT = 0x5,
    VAR_INTEGER = 0x6,
    VAR_CODEPOS = 0x7,
    VAR_PRECODEPOS = 0x8,
    VAR_FUNCTION = 0x9,
    VAR_STACK = 0xA,
    VAR_ANIMATION = 0xB,
    VAR_DEVELOPER_CODEPOS = 0xC,
    VAR_INCLUDE_CODEPOS = 0xD,
    VAR_THREAD = 0xE,
    VAR_NOTIFY_THREAD = 0xF,
    VAR_TIME_THREAD = 0x10,
    VAR_CHILD_THREAD = 0x11,
    VAR_OBJECT = 0x12,
    VAR_DEAD_ENTITY = 0x13,
    VAR_ENTITY = 0x14,
    VAR_ARRAY = 0x15,
    VAR_DEAD_THREAD = 0x16,
    VAR_COUNT = 0x17,
    VAR_THREAD_LIST = 0x18,
    VAR_ENDON_LIST = 0x19
};

void Info_SetValueForKey(char* s, const char* key, const char* value);
char* Info_ValueForKey(const char* s, const char* key);
void Q_strncpyz(char* dest, const char* src, int destsize);
void SV_UpdateClientConfigInfo(client_t* cl);
void SV_DropClientNoNotify(client_t* drop, const char* reason);
void SV_GameSendServerCommand(int clientNum, svscmd_type type, const char* text);
void SV_SendServerCommand(client_t* cl, const char* fmt, ...);
void SV_SendServerCommandString(client_t* cl, int type, char* message);
void* Z_TagMalloc(int size, int tag); // NOT 0 filled memory
void* Z_Malloc(int size);             // returns 0 filled memory
void* S_Malloc(int size);             // NOT 0 filled memory only for small allocations
void Com_QueueEvent(int time, sysEventType_t type, int value, int value2, int ptrLength, void* ptr);
qboolean SV_Map(const char* levelname);
void SV_MapRestart(qboolean fastrestart);
void SV_ScreenshotClient(client_t* cl, const char* name);
}

const char* BG_WeaponName(int weapon);

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_

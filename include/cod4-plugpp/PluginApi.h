#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_

#include "pinc.h"

extern "C" {
enum svscmd_type {
    SV_CMD_CAN_IGNORE = 0x0,
    SV_CMD_RELIABLE = 0x1,
};

void Info_SetValueForKey(char* s, const char* key, const char* value);
char* Info_ValueForKey(const char* s, const char* key);
void Q_strncpyz(char* dest, const char* src, int destsize);
void SV_UpdateClientConfigInfo(client_t* cl);
void SV_DropClientNoNotify(client_t* drop, const char* reason);
void SV_GameSendServerCommand(int clientNum, svscmd_type type, const char* text);
}

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_

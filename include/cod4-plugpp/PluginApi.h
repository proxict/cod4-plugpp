#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_

#include "pinc.h"

extern "C" {
void Info_SetValueForKey(char* s, const char* key, const char* value);
char* Info_ValueForKey(const char* s, const char* key);
void Q_strncpyz(char* dest, const char* src, int destsize);
void SV_UpdateClientConfigInfo(client_t* cl);
}

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINAPI_HPP_

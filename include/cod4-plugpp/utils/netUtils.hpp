#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_NETUTILS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_NETUTILS_HPP_

#include "cod4-plugpp/PluginApi.h"

#include <lib-optional/optional.hpp>

#include <string>

namespace plugpp {

using namespace libOptional;

inline std::string toStr(const netadr_t* netaddress) {
    char buffer[53];
    Plugin_NET_AdrToStringMT(const_cast<netadr_t*>(netaddress), buffer, sizeof(buffer));
    std::string ip(buffer);

    const std::size_t portIndex = ip.find_last_of(':');
    if (portIndex != std::string::npos) {
        ip.erase(portIndex, std::string::npos);
    }

    const std::size_t openBracketIndex = ip.find_first_of('[');
    if (openBracketIndex != std::string::npos) {
        ip.erase(openBracketIndex, 1);
    }

    const std::size_t closeBracketIndex = ip.find_last_of(']');
    if (closeBracketIndex != std::string::npos) {
        ip.erase(closeBracketIndex, 1);
    }

    return ip;
}

inline bool toNetAddr(const std::string& address, netadr_t* out) {
    const netadrtype_t ipType = address.find('.') != std::string::npos ? NA_IP : NA_IP6;
    return Plugin_NET_StringToAdr(address.c_str(), out, ipType) != 0;
}

inline Optional<netadr_t> toNetAddr(const std::string& address) {
    netadr_t out;
    if (!toNetAddr(address, &out)) {
        return NullOptional;
    }
    return out;
}

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_NETUTILS_HPP_

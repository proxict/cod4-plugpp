#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_NETUTILS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_NETUTILS_HPP_

#include "cod4-plugpp/Optional.hpp"
#include "cod4-plugpp/PluginApi.hpp"

#include <string>

namespace plugpp {

/// Gets a string representation of the given network address
/// @param netaddress Binary representation of the network address.
/// @returns A string representation of the network address or "(null)" if the @ref netaddress is nullptr.
[[nodiscard]] inline std::string toStr(const netadr_t* netaddress) noexcept {
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

/// Gets a binary representation of the given network address
///
/// The given network address can be an IPv4 or IPv6 address.
/// @param address String representation of the network address.
/// @param[out] out A pointer to a structure where the binary representation of the given address will be
/// stored.
/// @returns true if the conversion was successful, false otherwise.
[[maybe_unused]] inline bool toNetAddr(const std::string& address, netadr_t* out) noexcept {
    const netadrtype_t ipType = address.find('.') != std::string::npos ? NA_IP : NA_IP6;
    return Plugin_NET_StringToAdr(address.c_str(), out, ipType) != 0;
}

/// Gets a binary representation of the given network address
///
/// The given network address can be an IPv4 or IPv6 address.
/// @param address String representation of the network address.
/// @returns The binary representation of the given address if the conversion was successful, NullOptional
/// otherwise.
[[nodiscard]] inline Optional<netadr_t> toNetAddr(const std::string& address) noexcept {
    netadr_t out;
    if (!toNetAddr(address, &out)) {
        return NullOptional;
    }
    return out;
}

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_NETUTILS_HPP_

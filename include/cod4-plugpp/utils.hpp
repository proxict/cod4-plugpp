#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_

#include "cod4-plugpp/PluginApi.h"

#include <sstream>
#include <string>

#include <lib-optional/optional.hpp>

namespace plugpp {

using namespace libOptional;

inline Optional<std::string> getUserInfoAttribute(const std::string& userInfo, const std::string& attrName) {
    const std::string escapedAttrName = "\\" + attrName + "\\";
    const std::size_t attrPos = userInfo.find(escapedAttrName);
    if (attrPos == std::string::npos) {
        return NullOptional;
    }
    const std::size_t valueStartPos = attrPos + escapedAttrName.size();
    const std::size_t valueEndPos = userInfo.find("\\", valueStartPos);
    return userInfo.substr(valueStartPos, valueEndPos - valueStartPos);
}

template <typename T>
inline std::string join(const T first, const T last, const std::string& separator) {
    std::stringstream ss;
    const auto* sep = "";
    for (T walk = first; walk != last; ++walk) {
        ss << sep << *walk;
        sep = separator.c_str();
    }
    return ss.str();
}

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

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_

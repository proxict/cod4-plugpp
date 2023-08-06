#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_

#include "cod4-plugpp/Optional.hpp"
#include "cod4-plugpp/typeTraits.hpp"

#include <string>

namespace plugpp {

[[nodiscard]] inline Optional<std::string> getUserInfoAttribute(const std::string& userInfo,
                                                                const std::string& attrName) {
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
[[nodiscard]] detail::reverse_wrapper<T> reverse(T&& iterable) {
    return { iterable };
}

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_

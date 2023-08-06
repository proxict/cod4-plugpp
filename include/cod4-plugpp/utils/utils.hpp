#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_

#include "cod4-plugpp/Optional.hpp"
#include "cod4-plugpp/typeTraits.hpp"

#include <string>

namespace plugpp {

/// Gets an attribute's value from the given userinfo string.
/// @param userInfo The userinfo string to get the value from. It's a backslash separated key-value list.
/// @param attrName Name of the attribute to get the value of.
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

/// Gets a reverse iterator for the given iterable object.
/// @param iterable The object to get a reverse iterator of.
/// @returns A reversed iterator for the given object.
template <typename T>
[[nodiscard]] detail::reverse_wrapper<T> reverse(T&& iterable) {
    return { iterable };
}

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_

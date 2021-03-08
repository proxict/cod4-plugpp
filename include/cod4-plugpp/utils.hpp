#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_

#include "cod4-plugpp/PluginApi.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

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

inline std::vector<std::string> tokenize(const std::string& str, const std::string& delim = " ") {
    if (str.empty()) {
        return {};
    }
    std::vector<std::string> tokens;
    std::size_t last = 0;
    std::size_t next = 0;
    while ((next = str.find(delim, last)) != std::string::npos) {
        tokens.emplace_back(str.substr(last, next - last));
        last = next + delim.size();
    }
    tokens.emplace_back(str.substr(last));
    return tokens;
}

inline std::string toLower(const std::string& str) {
    std::string ret(str);
    std::transform(ret.begin(), ret.end(), ret.begin(), [](unsigned char c) { return std::tolower(c); });
    return ret;
}

inline std::string toUpper(const std::string& str) {
    std::string ret(str);
    std::transform(ret.begin(), ret.end(), ret.begin(), [](unsigned char c) { return std::toupper(c); });
    return ret;
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

class Time final {
public:
    enum class Segment { YEARS, MONTHS, DAYS, HOURS, MINUTES, SECONDS };
    Time(const std::uint64_t timeSec) {
        // clang-format off
        const int years = std::floor(timeSec / (365 * 60 * 60 * 24));
        const int months = std::floor((timeSec - years * 365 * 60 * 60 * 24) / (30 * 60 * 60 * 24));
        const int days = std::floor((timeSec - years * 365 * 60 * 60 * 24 - months * 30 * 60 * 60 * 24)/ (60 * 60 * 24));
        const int hours = std::floor((timeSec - years * 365 * 60 * 60 * 24 - months * 30 * 60 * 60 * 24 - days * 60 * 60 * 24) / (60 * 60));
        const int minutes = std::floor((timeSec - years * 365 * 60 * 60 * 24 - months * 30 * 60 * 60 * 24 - days * 60 * 60 * 24 - hours * 60 * 60)/ 60);
        const int seconds = std::floor((timeSec - years * 365 * 60 * 60 * 24 - months * 30 * 60 * 60 * 24 - days * 60 * 60 * 24 - hours * 60 * 60 - minutes * 60));
        // clang-format on
        mSegments = { years, months, days, hours, minutes, seconds };
    }

    int getSegment(const Segment segment) const {
        int index = static_cast<int>(segment);
        if (index < 0 || index > int(mSegments.max_size()) - 1) {
            return -1;
        }
        return mSegments.at(index);
    }

    std::array<int, 6> mSegments;
};

inline std::string toStr(const Time& time) {
    std::vector<std::string> segments;

    auto maybeAppendSegment = [&](const Time::Segment segment) {
        const int value = time.getSegment(segment);
        if (value <= 0) {
            return;
        }
        std::stringstream ss;
        ss << value << " ";
        switch (segment) {
        case Time::Segment::YEARS:
            ss << "year";
            break;
        case Time::Segment::MONTHS:
            ss << "month";
            break;
        case Time::Segment::DAYS:
            ss << "day";
            break;
        case Time::Segment::HOURS:
            ss << "hour";
            break;
        case Time::Segment::MINUTES:
            ss << "hour";
            break;
        case Time::Segment::SECONDS:
            ss << "second";
            break;
        }
        if (value > 1) {
            ss << "s";
        }
        segments.emplace_back(ss.str());
    };

    for (int i = static_cast<int>(Time::Segment::YEARS); i <= static_cast<int>(Time::Segment::SECONDS); ++i) {
        maybeAppendSegment(static_cast<Time::Segment>(i));
    }
    return join(std::begin(segments), std::end(segments), ", ");
}

class ScopedCriticalSection final {
public:
    explicit ScopedCriticalSection() { Plugin_EnterCriticalSection(); }
    ~ScopedCriticalSection() noexcept { Plugin_LeaveCriticalSection(); }

    ScopedCriticalSection& operator=(const ScopedCriticalSection&) = delete;
    ScopedCriticalSection(const ScopedCriticalSection&) = delete;
    ScopedCriticalSection& operator=(ScopedCriticalSection&&) = delete;
    ScopedCriticalSection(ScopedCriticalSection&&) = delete;
};

class Cvar final {
public:
    Cvar(std::string name, const std::string& description = "") noexcept
        : mName(std::move(name)) {
        ScopedCriticalSection criticalSectionGuard;
        mCvar = static_cast<cvar_t*>(Plugin_Cvar_RegisterString(mName.c_str(), "", 0, description.c_str()));
    }

    template <typename T>
    Optional<T> get() const {
        if (empty()) {
            return NullOptional;
        }
        std::stringstream ss;
        char buffer[1024];
        ss << std::string(Plugin_Cvar_GetString(mCvar, buffer, sizeof(buffer)));
        T out;
        ss >> out;
        if (ss.fail()) {
            return NullOptional;
        }
        return out;
    }

    template <typename T>
    void set(const T& value) const {
        std::stringstream ss;
        ss << value;
        Plugin_Cvar_SetString(mCvar, ss.str().c_str());
    }

    bool empty() const noexcept { return mCvar->string[0] == 0; }

private:
    std::string mName;
    cvar_t* mCvar;
};

class CmdLine final {
public:
    CmdLine() noexcept = default;

    Optional<const char*> get(const int index) const noexcept {
        if (index + mShift >= Plugin_Cmd_Argc()) {
            return NullOptional;
        }
        return Plugin_Cmd_Argv(index + mShift);
    }

    template <typename T>
    Optional<T> get(const int index) const {
        Optional<const char*> arg = get(index);
        if (!arg) {
            return NullOptional;
        }
        std::stringstream ss;
        ss << *arg;
        T out;
        ss >> out;
        return out;
    }

    void shift(const int shift = 1) noexcept { mShift += shift; }

    void reset() noexcept { mShift = 0; }

    CmdLine(const CmdLine&) = delete;
    CmdLine& operator=(const CmdLine&) = delete;
    CmdLine(CmdLine&&) = delete;
    CmdLine& operator=(CmdLine&&) = delete;

private:
    int mShift = 0;
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_HPP_

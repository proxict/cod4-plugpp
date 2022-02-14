#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_TIMEUTILS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_TIMEUTILS_HPP_

#include "cod4-plugpp/utils/stringUtils.hpp"

#include <array>
#include <cmath>
#include <sstream>
#include <vector>

namespace plugpp {

class Time final {
public:
    enum class Segment { YEARS, MONTHS, DAYS, HOURS, MINUTES, SECONDS };
    explicit Time(const std::uint64_t timeSec) noexcept {
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

    int getSegment(const Segment segment) const noexcept {
        const int index = static_cast<int>(segment);
        if (index < 0 || index > int(mSegments.max_size()) - 1) {
            return -1;
        }
        return mSegments.at(index);
    }

    std::array<int, 6> mSegments;
};

inline std::string toStr(const Time& time, const bool condensed = false) {
    std::vector<std::string> segments;

    auto maybeAppendSegment = [&](const Time::Segment segment) {
        const int value = time.getSegment(segment);
        if (value <= 0) {
            return false;
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
            ss << "minute";
            break;
        case Time::Segment::SECONDS:
            ss << "second";
            break;
        }
        if (value > 1) {
            ss << "s";
        }
        segments.emplace_back(ss.str());
        return true;
    };

    int stopWith = static_cast<int>(Time::Segment::SECONDS);
    for (int i = static_cast<int>(Time::Segment::YEARS); i <= stopWith; ++i) {
        if (maybeAppendSegment(static_cast<Time::Segment>(i)) && condensed) {
            stopWith = std::min(stopWith, i + 1);
        }
    }
    return join(segments, ", ");
}

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_TIMEUTILS_HPP_

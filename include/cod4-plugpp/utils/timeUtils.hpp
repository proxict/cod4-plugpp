#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_TIMEUTILS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_TIMEUTILS_HPP_

#include "cod4-plugpp/utils/stringUtils.hpp"

#include <array>
#include <chrono>
#include <cmath>
#include <sstream>
#include <vector>

namespace plugpp {

/// Time duration fragmented into years, months, days, hours, minutes, and seconds.
class TimeDuration final {
public:
    enum class Fraction { YEARS, MONTHS, DAYS, HOURS, MINUTES, SECONDS };

    /// Constructs the object from the given duration
    /// @param duration The duration to fraction.
    template <typename TRep, typename TPeriod>
    explicit TimeDuration(const std::chrono::duration<TRep, TPeriod>& duration) noexcept {
        const std::uint64_t timeSec = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        // clang-format off
        const int years = std::floor(timeSec / (365 * 60 * 60 * 24));
        const int months = std::floor((timeSec - years * 365 * 60 * 60 * 24) / (30 * 60 * 60 * 24));
        const int days = std::floor((timeSec - years * 365 * 60 * 60 * 24 - months * 30 * 60 * 60 * 24)/ (60 * 60 * 24));
        const int hours = std::floor((timeSec - years * 365 * 60 * 60 * 24 - months * 30 * 60 * 60 * 24 - days * 60 * 60 * 24) / (60 * 60));
        const int minutes = std::floor((timeSec - years * 365 * 60 * 60 * 24 - months * 30 * 60 * 60 * 24 - days * 60 * 60 * 24 - hours * 60 * 60)/ 60);
        const int seconds = std::floor((timeSec - years * 365 * 60 * 60 * 24 - months * 30 * 60 * 60 * 24 - days * 60 * 60 * 24 - hours * 60 * 60 - minutes * 60));
        // clang-format on
        mFractions = { years, months, days, hours, minutes, seconds };
    }

    /// Gets the amount of units in the given fraction
    /// @param fraction The fraction to get.
    /// @returns The amount of units in the given fraction.
    [[nodiscard]] int getFractionAmount(const Fraction fraction) const noexcept {
        const int index = static_cast<int>(fraction);
        if (index < 0 || index > int(mFractions.max_size()) - 1) {
            return -1;
        }
        return mFractions.at(index);
    }

private:
    std::array<int, 6> mFractions;
};

/// Converts the given time duration to a human readable string
///
/// The format of the string can be e.g., "2 months, 1 hour, 35 minutes". Only non-zero fractions are included
/// in the output.
///
/// @param time The time duration to convert to string.
/// @param condensed If true, only the 2 most significant non-zero fractions are going to be included in the
/// output.
/// @returns A human readable string representing the given duration.
[[nodiscard]] inline std::string toStr(const TimeDuration& time, const bool condensed = false) {
    std::vector<std::string> fractions;

    auto maybeAppendFraction = [&](const TimeDuration::Fraction fraction) {
        const int value = time.getFractionAmount(fraction);
        if (value <= 0) {
            return false;
        }
        std::stringstream ss;
        ss << value << " ";
        switch (fraction) {
        case TimeDuration::Fraction::YEARS:
            ss << "year";
            break;
        case TimeDuration::Fraction::MONTHS:
            ss << "month";
            break;
        case TimeDuration::Fraction::DAYS:
            ss << "day";
            break;
        case TimeDuration::Fraction::HOURS:
            ss << "hour";
            break;
        case TimeDuration::Fraction::MINUTES:
            ss << "minute";
            break;
        case TimeDuration::Fraction::SECONDS:
            ss << "second";
            break;
        }
        if (value > 1) {
            ss << "s";
        }
        fractions.emplace_back(ss.str());
        return true;
    };

    static auto toInt = [](TimeDuration::Fraction fraction) { return static_cast<int>(fraction); };
    static auto toFraction = [](int fraction) { return static_cast<TimeDuration::Fraction>(fraction); };

    int fractionsAdded = 0;
    for (auto i = toInt(TimeDuration::Fraction::YEARS); i <= toInt(TimeDuration::Fraction::SECONDS); ++i) {
        fractionsAdded += maybeAppendFraction(toFraction(i));
        if (condensed && fractionsAdded >= 2) {
            break;
        }
    }
    return join(fractions, ", ");
}

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_TIMEUTILS_HPP_

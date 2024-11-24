#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_ARITHMETIC_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_ARITHMETIC_HPP_

#include <cstdint>
#include <limits>
#include <optional>
#include <string>
#include <type_traits>
#include <typeinfo>

namespace plugpp::utils::arithmetic {

template <typename R, typename T>
constexpr bool in_range(T value) noexcept {
    constexpr R kMin = std::numeric_limits<R>::min();
    constexpr R kMax = std::numeric_limits<R>::max();

    if constexpr (std::is_signed_v<T> == std::is_signed_v<R>) {
        return value >= kMin && value <= kMax;
    } else if constexpr (std::is_signed_v<T>) {
        return value >= 0 && std::make_unsigned_t<T>(value) <= kMax;
    } else {
        return value <= std::make_unsigned_t<R>(kMax);
    }
}

template <typename TTo, typename TFrom>
constexpr TTo numeric_cast(TFrom from) {
    if constexpr (!std::is_same_v<TFrom, bool>) {
        if constexpr (std::is_same_v<TTo, bool>) {
            if (from != 0 && from != 1) {
                throw std::bad_cast();
            }
        } else if (!in_range<TTo>(from)) {
            throw std::bad_cast();
        }
    }
    return static_cast<TTo>(from);
}

template <typename T, typename..., std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
inline std::optional<T> parse(const std::string&) {
    static_assert(!std::is_same_v<T, T>, "No 'parse()' specialization found for this type");
}

namespace detail {
    template <typename T, typename TParser>
    std::optional<T> parse(const std::string& num, TParser&& parser) {
        if constexpr (std::is_unsigned_v<T>) {
            if (!num.empty() && num.front() == '-') {
                return std::nullopt;
            }
        }

        try {
            std::size_t idx = 0;
            if (const auto ret = parser(num, &idx); idx == num.size()) {
                return ret;
            }
        } catch (const std::exception&) {
        }
        return std::nullopt;
    }

    template <typename T>
    std::optional<T> parse_inferred(const std::string& num) {
        try {
            using ::plugpp::utils::arithmetic::parse;
            if (const auto res = parse<std::conditional_t<std::is_unsigned_v<T>, uint64_t, int64_t>>(num);
                res) {
                return plugpp::utils::arithmetic::numeric_cast<T>(*res);
            }
        } catch (const std::exception&) {
        }
        return std::nullopt;
    }
} // namespace detail

// Signed integers
template <>
inline std::optional<int64_t> parse(const std::string& num) {
    return detail::parse<int64_t>(num,
                                  [](const std::string& n, std::size_t* idx) { return std::stoll(n, idx); });
}

template <>
inline std::optional<int32_t> parse(const std::string& num) {
    return detail::parse_inferred<int32_t>(num);
}

template <>
inline std::optional<int16_t> parse(const std::string& num) {
    return detail::parse_inferred<int16_t>(num);
}

template <>
inline std::optional<int8_t> parse(const std::string& num) {
    return detail::parse_inferred<int8_t>(num);
}

// Unsigned integers
template <>
inline std::optional<uint64_t> parse(const std::string& num) {
    return detail::parse<uint64_t>(
        num, [](const std::string& n, std::size_t* idx) { return std::stoull(n, idx); });
}

template <>
inline std::optional<uint32_t> parse(const std::string& num) {
    return detail::parse_inferred<uint32_t>(num);
}

template <>
inline std::optional<uint16_t> parse(const std::string& num) {
    return detail::parse_inferred<uint16_t>(num);
}

template <>
inline std::optional<uint8_t> parse(const std::string& num) {
    return detail::parse_inferred<uint8_t>(num);
}

// Floating point
template <>
inline std::optional<float> parse(const std::string& num) {
    return detail::parse<float>(num,
                                [](const std::string& n, std::size_t* idx) { return std::stof(n, idx); });
}

template <>
inline std::optional<double> parse(const std::string& num) {
    return detail::parse<double>(num,
                                 [](const std::string& n, std::size_t* idx) { return std::stod(n, idx); });
}

template <>
inline std::optional<long double> parse(const std::string& num) {
    return detail::parse<long double>(
        num, [](const std::string& n, std::size_t* idx) { return std::stold(n, idx); });
}

// Bool
template <>
inline std::optional<bool> parse(const std::string& num) {
    if (num == "true") {
        return true;
    } else if (num == "false") {
        return false;
    }
    return detail::parse_inferred<bool>(num);
}

} // namespace plugpp::utils::arithmetic

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_UTILS_ARITHMETIC_HPP_

#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_STRINGUTILS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_STRINGUTILS_HPP_

#include "cod4-plugpp/typeTraits.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

namespace plugpp {

[[maybe_unused]] inline std::string&
replaceAllInplace(std::string& str, const std::string& from, const std::string& to) {
    std::size_t pos = 0;
    while (!from.empty() && (pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.size(), to);
        pos += to.size();
    }
    return str;
}

[[nodiscard]] inline std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    return replaceAllInplace(str, from, to);
}

template <typename T>
[[nodiscard]] inline std::string join(const T first, const T last, const std::string& separator) {
    std::stringstream ss;
    const auto* sep = "";
    for (T walk = first; walk != last; ++walk) {
        ss << sep << *walk;
        sep = separator.c_str();
    }
    return ss.str();
}

template <typename TContainer, typename std::enable_if<IsIterable_v<TContainer>, int>::type = 1>
[[nodiscard]] inline std::string join(const TContainer& c, const std::string& separator) {
    return join(std::begin(c), std::end(c), separator);
}

enum class TokenizeMode {
    INCLUDE_EMPTY_TOKENS,
    EXCLUDE_EMPTY_TOKENS,
};

[[nodiscard]] inline std::vector<std::string>
tokenize(const std::string& str,
         const std::string& delimiter,
         const TokenizeMode mode = TokenizeMode::INCLUDE_EMPTY_TOKENS) {
    if (str.empty()) {
        return mode == TokenizeMode::EXCLUDE_EMPTY_TOKENS ? std::vector<std::string>{}
                                                          : std::vector<std::string>({ str });
    }
    if (delimiter.empty()) {
        return { str };
    }
    std::vector<std::string> tokens;
    std::size_t last = 0;
    for (std::size_t next = 0; (next = str.find(delimiter, last)) != std::string::npos;) {
        if (mode == TokenizeMode::INCLUDE_EMPTY_TOKENS || next > last) {
            tokens.emplace_back(str.substr(last, next - last));
        }
        last = next + delimiter.size();
    }
    if (mode == TokenizeMode::INCLUDE_EMPTY_TOKENS || last < str.size()) {
        tokens.emplace_back(str.substr(last));
    }
    return tokens;
}

template <typename TPredicate,
          typename...,
          typename std::enable_if<IsCallable<TPredicate, bool(char)>::value, int>::type = 1>
[[nodiscard]] inline std::vector<std::string>
tokenize(const std::string& str,
         const TPredicate& predicate,
         const TokenizeMode mode = TokenizeMode::INCLUDE_EMPTY_TOKENS) {
    if (str.empty()) {
        return mode == TokenizeMode::EXCLUDE_EMPTY_TOKENS ? std::vector<std::string>{}
                                                          : std::vector<std::string>({ str });
    }
    std::vector<std::string> tokens;
    auto last = std::begin(str);
    for (auto next = last; (next = std::find_if(last, std::end(str), predicate)) != std::end(str);) {
        if (mode == TokenizeMode::INCLUDE_EMPTY_TOKENS || next > last) {
            tokens.emplace_back(str.substr(static_cast<std::size_t>(std::distance(std::begin(str), last)),
                                           static_cast<std::size_t>(std::distance(last, next))));
        }
        last = next + 1;
    }
    if (mode == TokenizeMode::INCLUDE_EMPTY_TOKENS || last != std::end(str)) {
        tokens.emplace_back(str.substr(static_cast<std::size_t>(std::distance(std::begin(str), last))));
    }
    return tokens;
}

[[nodiscard]] inline std::vector<std::string>
tokenize(const std::string& s,
         const char delimiter,
         const TokenizeMode mode = TokenizeMode::INCLUDE_EMPTY_TOKENS) {
    return tokenize(
        s, [delimiter](const char c) { return c == delimiter; }, mode);
}

[[nodiscard]] inline bool startsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() &&
           std::mismatch(std::begin(prefix), std::end(prefix), std::begin(str)).first == std::end(prefix);
}

template <typename T>
[[nodiscard]] inline std::vector<std::string>
joinQuotedArgs(const T first, const T last, const char quote = '"') {
    bool openQuote = false;
    std::vector<std::string> tokens;
    for (T walk = first; walk != last; ++walk) {
        const std::string& arg = *walk;
        const bool oddNumberOfQuotes = std::count(std::begin(arg), std::end(arg), quote) & 1;
        if (!openQuote) {
            tokens.emplace_back(arg);
        } else {
            tokens.back().push_back(' ');
            tokens.back() += arg;
        }
        openQuote = oddNumberOfQuotes != openQuote;

        if (!openQuote) {
            std::string& lastToken = tokens.back();
            lastToken.erase(std::remove(std::begin(lastToken), std::end(lastToken), quote),
                            std::end(lastToken));
        }
    }

    return tokens;
}

[[nodiscard]] inline std::vector<std::string> joinQuotedArgs(const std::vector<std::string>& args,
                                                             const char quote = '"') {
    return joinQuotedArgs(std::begin(args), std::end(args), quote);
}

template <typename T, typename... TArgs>
[[maybe_unused]] std::ostream& serialize(std::ostream& stream, T&& first, TArgs&&... args) {
    stream << std::forward<T>(first);
    ((stream << std::forward<TArgs>(args)), ...);
    return stream;
}

template <typename T, typename... TArgs>
[[nodiscard]] std::string concatenate(T&& first, TArgs&&... args) {
    std::stringstream ss;
    serialize(ss, std::forward<T>(first), std::forward<TArgs>(args)...);
    return ss.str();
}

[[nodiscard]] inline std::string trim(const std::string& str) {
    static constexpr const char* const whitespaces = "\t\n\v\f\r ";
    std::string result(str);
    result.erase(0, str.find_first_not_of(whitespaces)); // left trim
    result.erase(str.find_last_not_of(whitespaces) + 1); // right trim
    return result;
}

[[nodiscard]] inline std::string toLower(const std::string& str) {
    std::string ret(str);
    std::transform(ret.begin(), ret.end(), ret.begin(), [](unsigned char c) { return std::tolower(c); });
    return ret;
}

[[nodiscard]] inline std::string toUpper(const std::string& str) {
    std::string ret(str);
    std::transform(ret.begin(), ret.end(), ret.begin(), [](unsigned char c) { return std::toupper(c); });
    return ret;
}

[[nodiscard]] inline std::string removeColor(const std::string& str) {
    std::string res;
    for (std::size_t i = 0; i < str.size(); ++i) {
        if (str.at(i) == '^' && i + 1 < str.size() && std::isdigit(str.at(i + 1))) {
            ++i;
            continue;
        }
        res += str.at(i);
    }
    return res;
}

[[nodiscard]] inline bool isInteger(const std::string& str) {
    for (std::size_t i = 0; i < str.size(); ++i) {
        if (!std::isdigit(str.at(i))) {
            if (i != 0 || str.at(i) != '-') {
                return false;
            }
        }
    }
    return !str.empty();
}

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_STRINGUTILS_HPP_

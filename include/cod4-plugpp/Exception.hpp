#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_EXCEPTION_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_EXCEPTION_HPP_

#include <sstream>
#include <string>

namespace plugpp {

class Exception {
public:
    template <typename... TArgs>
    explicit Exception(const std::string& message, TArgs&&... args)
        : mMessage(stringify(message, std::forward<TArgs>(args)...)) {}

    virtual ~Exception() noexcept = default;

    virtual const std::string& what() const noexcept { return mMessage; }

    Exception(Exception&&) = default;

private:
    template <typename T>
    std::string stringify(const T& arg) {
        std::ostringstream ss;
        ss << arg;
        return ss.str();
    }

    template <typename T, typename... Args>
    std::string stringify(const T& first, Args&&... args) {
        return stringify(first) + stringify(std::forward<Args>(args)...);
    }

    Exception(const Exception&) = delete;
    Exception& operator=(const Exception&) = delete;
    Exception& operator=(Exception&&) = delete;

    const std::string mMessage;
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_EXCEPTION_HPP_

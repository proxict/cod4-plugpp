#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_EXCEPTION_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_EXCEPTION_HPP_

#include <cod4-plugpp/utils/stringUtils.hpp>

#include <sstream>
#include <stdexcept>
#include <string>

namespace plugpp {

class Exception : public std::runtime_error {
public:
    template <typename... TArgs>
    explicit Exception(const std::string& message, TArgs&&... args)
        : std::runtime_error(plugpp::concatenate(message, std::forward<TArgs>(args)...)) {}

    virtual ~Exception() noexcept = default;

    Exception(Exception&&) noexcept = default;

    Exception(const Exception&) = delete;
    Exception& operator=(const Exception&) = delete;
    Exception& operator=(Exception&&) = delete;
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_EXCEPTION_HPP_

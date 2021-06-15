#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_COMMAND_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_COMMAND_HPP_

#include <functional>
#include <memory>
#include <string>

namespace plugpp {

class Command final {
public:
    Command(std::string functionName, std::function<void()> functor);

    ~Command() noexcept;

private:
    void callback();

    class Impl;
    std::unique_ptr<Impl> mImpl;

    std::string mFunctionName;
};

struct CommandRemover {
    CommandRemover(const std::string& commandName) noexcept;
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_COMMAND_HPP_

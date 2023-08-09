#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_COMMAND_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_COMMAND_HPP_

#include <functional>
#include <memory>
#include <string>

namespace plugpp {

/// Creates a new command that can be issued from the server's command line.
///
/// @warning By default, there is a limitation of only 20 commands per plugin that can be registered. This
/// value can be overridden using the PLUGPP_MAX_FUNCTIONS preprocessor macro (-DPLUGPP_MAX_FUNCTIONS=40).
class Command final {
public:
    /// Registeres the command and its callback
    /// @param commandName The name of the command to register.
    /// @param callback The callback that's going to be called when the command gets issued.
    /// @param replace If true and a command with the given name already exists, replace its callback with the
    /// supplied one. If false and the command already exists, the callback is not going to be replaced.
    /// @throws plugpp::Exception if the number of registered commands is exceeded.
    Command(std::string commandName, std::function<void()> callback, bool replace = false);

    ~Command() noexcept;

private:
    void callback();

    class Impl;
    std::unique_ptr<Impl> mImpl;

    std::string mCommandName;
};

void removeCommand(const std::string& commandName);

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_COMMAND_HPP_

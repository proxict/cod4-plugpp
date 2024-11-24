#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CMDLINE_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CMDLINE_HPP_

#include "cod4-plugpp/PluginApi.hpp"

#include <optional>
#include <sstream>

namespace plugpp {

/// A helper for getting arguments from the server's command line
class CmdLine final {
public:
    CmdLine() noexcept = default;

    /// Gets the Nth command line argument
    ///
    /// @warning Unlike the conventional CLI argument parsing, no 0th argument (usually the command's name) is
    /// passed into this helper. That means the argument at the `0` index is the first argument passed to the
    /// command, not the command itself.
    ///
    /// @param index The index of the argument to get.
    /// @returns The requested argument or std::nullopt if the @ref index is past the number of arguments on
    /// the command line or if no arguments are remaining after shifting them (@see shift()).
    [[nodiscard]] std::optional<const char*> get(const int index) const noexcept {
        if (index + mShift >= Plugin_Cmd_Argc()) {
            return std::nullopt;
        }
        return Plugin_Cmd_Argv(index + mShift);
    }

    /// @copydoc get()
    ///
    /// This overload converts the argument to the given type. The given type shall have the
    /// operator<<(std::ostream&) defined.
    template <typename T>
    [[nodiscard]] std::optional<T> get(const int index) const {
        std::optional<const char*> arg = get(index);
        if (!arg) {
            return std::nullopt;
        }
        std::stringstream ss;
        ss << *arg;
        T out;
        ss >> out;
        return ss.fail() ? std::nullopt : std::optional<T>(out);
    }

    /// Shifts the root of the arguments to the left by the specified amount
    /// @param[optional] shift How many arguments to shift (defaults to 1).
    void shift(const int shift = 1) noexcept { mShift += shift; }

    /// Resets shifting of the arguments (@see shift())
    void reset() noexcept { mShift = 0; }

    CmdLine(const CmdLine&) = delete;
    CmdLine& operator=(const CmdLine&) = delete;
    CmdLine(CmdLine&&) = delete;
    CmdLine& operator=(CmdLine&&) = delete;

private:
    int mShift = 0;
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CMDLINE_HPP_

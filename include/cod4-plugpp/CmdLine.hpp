#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CMDLINE_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CMDLINE_HPP_

#include "cod4-plugpp/Optional.hpp"
#include "cod4-plugpp/PluginApi.h"

#include <sstream>

namespace plugpp {

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

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CMDLINE_HPP_

#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CVAR_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CVAR_HPP_

#include "cod4-plugpp/CriticalSection.hpp"
#include "cod4-plugpp/Optional.hpp"
#include "cod4-plugpp/PluginApi.h"

#include <mutex>
#include <sstream>
#include <string>

namespace plugpp {

class Cvar final {
public:
    explicit Cvar(std::string name, const std::string& description = "") noexcept
        : mName(std::move(name)) {
        CriticalSection criticalSection;
        std::lock_guard<CriticalSection> lock(criticalSection);
        mCvar = static_cast<cvar_t*>(Plugin_Cvar_RegisterString(mName.c_str(), "", 0, description.c_str()));
    }

    template <typename T>
    Optional<T> get() const {
        if (empty()) {
            return NullOptional;
        }
        std::stringstream ss;
        char buffer[1024];
        ss << std::string(Plugin_Cvar_GetString(mCvar, buffer, sizeof(buffer)));
        T out;
        ss >> out;
        if (ss.fail()) {
            return NullOptional;
        }
        return out;
    }

    template <typename T>
    void set(const T& value) const {
        std::stringstream ss;
        ss << value;
        Plugin_Cvar_SetString(mCvar, ss.str().c_str());
    }

    bool empty() const noexcept { return mCvar->string[0] == 0; }

private:
    std::string mName;
    cvar_t* mCvar;
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CVAR_HPP_

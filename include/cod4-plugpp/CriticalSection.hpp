#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CRITICALSECTION_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CRITICALSECTION_HPP_

#include "cod4-plugpp/PluginApi.h"

namespace plugpp {

/// A critical section wrapper that can be used with std::lock_guard and alike.
class CriticalSection final {
public:
    explicit CriticalSection() = default;

    /// Enters the plugin's critical section
    void lock() noexcept { Plugin_EnterCriticalSection(); }

    /// Leaves the plugin's critical section
    void unlock() noexcept { Plugin_LeaveCriticalSection(); }

    CriticalSection& operator=(const CriticalSection&) = delete;
    CriticalSection(const CriticalSection&) = delete;
    CriticalSection& operator=(CriticalSection&&) = delete;
    CriticalSection(CriticalSection&&) = delete;
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CRITICALSECTION_HPP_

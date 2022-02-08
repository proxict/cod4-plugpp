#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CRITICALSECTION_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CRITICALSECTION_HPP_

#include "cod4-plugpp/PluginApi.h"

namespace plugpp {

class CriticalSection final {
public:
    explicit CriticalSection() = default;

    void lock() noexcept { Plugin_EnterCriticalSection(); }

    void unlock() noexcept { Plugin_LeaveCriticalSection(); }

    CriticalSection& operator=(const CriticalSection&) = delete;
    CriticalSection(const CriticalSection&) = delete;
    CriticalSection& operator=(CriticalSection&&) = delete;
    CriticalSection(CriticalSection&&) = delete;
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CRITICALSECTION_HPP_

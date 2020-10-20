#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_

#include "pinc.h"

#include <functional>
#include <string>

#include <lib-optional/optional.hpp>

namespace plugpp {

using namespace libOptional;

using KickReason = std::string;
using Kick = Optional<KickReason>;

static constexpr NullOptionalT NoKick(NullOptional);

namespace detail {
    template <int TInstance, typename T>
    struct CallbackImpl;

    template <int TInstance, typename Ret, typename... Params>
    struct CallbackImpl<TInstance, Ret(Params...)> {
        template <typename... Args>
        static Ret callback(Args... args) {
            return functor(args...);
        }
        static std::function<Ret(Params...)> functor;
    };

    template <int TInstance, typename Ret, typename... Params>
    std::function<Ret(Params...)> CallbackImpl<TInstance, Ret(Params...)>::functor;
} // namespace detail

#define InstantiateCallback(signature) ::plugpp::detail::CallbackImpl<__COUNTER__, signature>

class Plugin {
public:
    explicit Plugin() = default;

    virtual ~Plugin() = default;

    virtual int onPluginLoad() { return 0; }

    virtual void onPluginUnload() {}

    virtual void onOneSecond() {}

    virtual void onTenSeconds() {}

    virtual void onPluginInfoRequest(pluginInfo_t* info) = 0;

    virtual Kick onPlayerConnect(int clientnum, netadr_t* netaddr, const char* userinfo) {
        (void)clientnum;
        (void)netaddr;
        (void)userinfo;
        return NoKick;
    }

    virtual void onPlayerDisconnect(client_t* client, const char* reason) {
        (void)client;
        (void)reason;
    }

    virtual void onTerminate() {}
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_

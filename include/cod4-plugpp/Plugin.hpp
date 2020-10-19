#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_

#include "pinc.h"

#include <string>

#include <lib-optional/optional.hpp>

namespace plugpp {

using namespace libOptional;

using KickReason = std::string;
using Kick = Optional<KickReason>;

static constexpr NullOptionalT NoKick(NullOptional);

class Plugin {
public:
    explicit Plugin() = default;

    virtual ~Plugin() = default;

    virtual int onPluginLoad() { return 0; }

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

#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_

#include "cod4-plugpp/Command.hpp"
#include "cod4-plugpp/Exception.hpp"
#include "cod4-plugpp/PluginApi.h"

#include <cstdint>
#include <functional>
#include <string>

#include <lib-optional/optional.hpp>

namespace plugpp {

using namespace libOptional;

using KickReason = std::string;
using Kick = Optional<KickReason>;

enum class MessageVisibility { HIDE, SHOW };
enum class ReservedSlotRequest { DENY, ALLOW };

static constexpr NullOptionalT NoKick(NullOptional);

class Plugin {
public:
    explicit Plugin() = default;

    virtual ~Plugin() = default;

    virtual int onPluginLoad() { return 0; }

    virtual void onPluginUnload() {}

    virtual void onOneSecond() {}

    virtual void onTenSeconds() {}

    virtual MessageVisibility onMessageSent(const std::string& message, int slot, int mode) {
        (void)message;
        (void)slot;
        (void)mode;
        return MessageVisibility::SHOW;
    }

    virtual void onPluginInfoRequest(pluginInfo_t* info) { (void)info; }

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

    virtual void onPlayerAddBan(baninfo_t* banInfo) { (void)banInfo; }

    virtual void onPlayerRemoveBan(baninfo_t* banInfo) { (void)banInfo; }

    virtual std::string onPlayerGetBanStatus(baninfo_t* banInfo) {
        (void)banInfo;
        return std::string();
    }

    virtual void onPreFastRestart() {}

    virtual void onExitLevel() {}

    virtual void onPostFastRestart() {}

    virtual void onSpawnServer() {}

    virtual void onFrame() {}

    virtual void onClientSpawn(gentity_t* entity) { (void)entity; }

    virtual void onClientEnteredWorld(client_t* client) { (void)client; }

    virtual void onClientUserInfoChanged(client_t* client) { (void)client; }

    virtual void onClientMoveCommand(client_t* client, usercmd_t* ucmd) {
        (void)client;
        (void)ucmd;
    }

    virtual ReservedSlotRequest onPlayerReservedSlotRequest(netadr_t* from) {
        (void)from;
        // TODO: Find out what's the default value
        return ReservedSlotRequest::DENY;
    }
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_

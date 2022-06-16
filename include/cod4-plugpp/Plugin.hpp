#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_

#include "cod4-plugpp/Command.hpp"
#include "cod4-plugpp/Exception.hpp"
#include "cod4-plugpp/Kick.hpp"
#include "cod4-plugpp/PluginApi.h"

#include <cstdint>
#include <functional>
#include <string>

namespace plugpp {

enum class MessageVisibility { HIDE, SHOW };
enum class ReservedSlotRequest { DENY, ALLOW };

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

    virtual Kick onPlayerGotAuthInfo(client_t* cl,
                                     netadr_t* from,
                                     uint64_t& playerid,
                                     uint64_t& steamid,
                                     bool& returnNow) {
        (void)cl;
        (void)from;
        (void)playerid;
        (void)steamid;
        (void)returnNow;
        return NoKick;
    }

    virtual Kick onPlayerGetBanStatus(baninfo_t* banInfo) {
        (void)banInfo;
        return NoKick;
    }

    virtual void onPlayerAccessGranted(client_t* client) { (void)client; }

    virtual void onPreFastRestart() {}

    virtual void onExitLevel() {}

    virtual void onPostFastRestart() {}

    virtual void onSpawnServer() {}

    virtual void onFrame() {}

    virtual void onClientSpawn(gentity_t* entity) { (void)entity; }

    virtual void onClientEnteredWorld(client_t* client, bool firstTime) {
        (void)client;
        (void)firstTime;
    }

    virtual void onClientUserInfoChanged(client_t* client) { (void)client; }

    virtual void onClientMoveCommand(client_t* client, usercmd_t* ucmd) {
        (void)client;
        (void)ucmd;
    }

    virtual void onPlayerKilled(gentity_s* self,
                                gentity_s* inflictor,
                                gentity_s* attacker,
                                int damage,
                                int meansOfDeath,
                                int iWeapon,
                                hitLocation_t hitLocation) {
        (void)self;
        (void)inflictor;
        (void)attacker;
        (void)damage;
        (void)meansOfDeath;
        (void)iWeapon;
        (void)hitLocation;
    }

    virtual ReservedSlotRequest onPlayerReservedSlotRequest(netadr_t* from) {
        (void)from;
        return ReservedSlotRequest::ALLOW;
    }
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_

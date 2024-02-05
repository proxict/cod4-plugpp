#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_

#include "cod4-plugpp/Kick.hpp"
#include "cod4-plugpp/PluginApi.h"

#include <cstdint>
#include <functional>
#include <string>

namespace plugpp {

enum class MessageVisibility { HIDE, SHOW };
enum class ReservedSlotRequest { DENY, ALLOW };

struct PluginInfo {
    PluginInfo(int majorVersion_,
               int minorVersion_,
               std::string fullName_,
               std::string shortDescription_,
               std::string longDescription_)
        : majorVersion(majorVersion_)
        , minorVersion(minorVersion_)
        , fullName(std::move(fullName_))
        , shortDescription(std::move(shortDescription_))
        , longDescription(std::move(longDescription_)) {}

    PluginInfo() noexcept = default;

    int majorVersion = 1;
    int minorVersion = 0;
    std::string fullName;
    std::string shortDescription;
    std::string longDescription;
};

class Plugin {
public:
    explicit Plugin() = default;

    virtual ~Plugin() = default;

    /// Gets called whenever a client sends a chat-message
    ///
    /// @param message The message the client sent.
    /// @param slot Slot number of the player sending the message.
    /// @param mode Determines if the message is public (mode = 0) or team-only (mode = 1).
    /// @returns MessageVisibility::SHOW to allow the message to be sent or MessageVisibility::HIDE to prevent
    /// the message from sending.
    virtual MessageVisibility onMessageSent(const std::string& message, int slot, int mode) {
        (void)message;
        (void)slot;
        (void)mode;
        return MessageVisibility::SHOW;
    }

    /// Gets called when the server requests information about this plugin.
    /// @returns PluginInfo initialized with relevant information about this plugin.
    virtual PluginInfo onPluginInfoRequest() { return {}; }

    /// Gets called whenever a client wants to connects to the server
    ///
    /// This callback is called before the client authenticates with a player or a steam ID, but decision to
    /// allow or reject the player can still be made based on the provided information. @see
    /// @ref Plugin::onPlayerGotAuthInfo() or @ref Plugin::onPlayerGetBanStatus() functions if you want to
    /// grant or reject the access based on the player's steam or player ID.
    ///
    /// @param slot Slot number of the connecting player.
    /// @param netaddr IP address of the connecting player.
    /// @param userinfo The userinfo string the client sends during connecting. It's a backslash separated
    /// key-value list.
    /// @returns @ref plugpp::NoKick to allow the player to join or @ref plugpp::Kick to kick the player with
    /// the given reason.
    virtual Kick onPlayerConnect(int clientnum, netadr_t* netaddr, const char* userinfo) {
        (void)clientnum;
        (void)netaddr;
        (void)userinfo;
        return NoKick;
    }

    /// Gets called whenever a client disconnects from the server
    ///
    /// @param client The player disconnecting.
    /// @param reason Reason for the disconnect (can be a kick message, EXE_DISCONNECT or a violation
    /// message).
    virtual void onPlayerDisconnect(client_t* client, const char* reason) {
        (void)client;
        (void)reason;
    }

    /// Gets called when the server asks for child threads termination
    ///
    /// Plugins using threads started with @ref Plugin_CreateNewThread() have to react to this callback by
    /// terminating all spawned threads and unlocking all critical sections before this function returns.
    virtual void onTerminate() {}

    /// Gets called when a player gets banned (can be a result of e.g., calling the banUser/banClient command)
    ///
    /// @param banInfo A structure containing information about the ban.
    virtual void onPlayerAddBan(baninfo_t* banInfo) { (void)banInfo; }

    /// Gets called when a player gets unbanned (can be a result of e.g., calling the unbanUser/unbanClient
    /// command)
    ///
    /// @param banInfo A structure containing information about the ban.
    virtual void onPlayerRemoveBan(baninfo_t* banInfo) { (void)banInfo; }

    /// Gets called whenever a client wants to connects to the server
    ///
    /// This callback is called after the @ref Plugin::onPlayerConnect() function.
    ///
    /// @param cl The client connecting to the server.
    /// @param from The IP address the client is connecting from.
    /// @param playerid The player's player ID.
    /// @param steamid The player's steam ID.
    /// @param returnNow If false (default), the client proceeds with connecting unless they get kicked by
    /// this function's return value. If set to true, the client will keep resending the authentication
    /// request, this this function will keep getting called. This is useful when this callback's
    /// implementation requires longer time to determine whether the client's connection request should be
    /// granted or rejected. For example, running a remote database request in a thread to check for a ban
    /// record and waiting for result in this callback.
    /// @returns @ref plugpp::NoKick to allow the player to join or @ref plugpp::Kick to kick the player with
    /// the given reason.
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

    /// Gets called whenever a client wants to connects to the server
    ///
    /// This callback is called after the @ref Plugin::onPlayerGotAuthInfo() function.
    ///
    /// @param banInfo A structure containing information about the ban.
    /// @returns @ref plugpp::NoKick to allow the player to join or @ref plugpp::Kick to kick the player with
    /// the given reason.
    virtual Kick onPlayerGetBanStatus(baninfo_t* banInfo) {
        (void)banInfo;
        return NoKick;
    }

    /// Gets called when the @ref plugpp::onPlayerGetBanStatus() function returns @ref plugpp::NoKick and the
    /// player is thus allowed to join the server
    ///
    /// This callback is called after the @ref Plugin::onPlayerGetBanStatus() function.
    ///
    /// @param client The client joining the server.
    virtual void onPlayerAccessGranted(client_t* client) { (void)client; }

    /// Gets called before level fast restart (map fast restart)
    virtual void onPreFastRestart() {}

    /// Gets called at the end of a level (end of map)
    virtual void onExitLevel() {}

    /// Gets called after level fast restart (map fast restart)
    virtual void onPostFastRestart() {}

    /// Gets called before game restart (usually a round restart)
    virtual void onPreGameRestart(int savepersist) { (void)savepersist; }

    /// Gets called after game restart (usually a round restart)
    virtual void onPostGameRestart(int savepersist) { (void)savepersist; }

    /// Gets called at the beginning of a level (beginning of a map)
    virtual void onSpawnServer() {}

    /// Gets called every server frame (sv_fps times per second, usually 20 Hz)
    virtual void onFrame() {}

    /// Gets called when a player spawns
    /// @param entity The entity corresponding to the player.
    virtual void onClientSpawn(gentity_t* entity) { (void)entity; }

    /// Gets called when the player enteres the world
    ///
    /// Entering the world means that the world for the player is presented (when the player joins the server
    /// or when a new round starts).
    ///
    /// @param client The client who entered the world.
    /// @param firstTime true if this was the first time the player has entered the world over the period of
    /// the player being connected to the server, false otherwise.
    virtual void onClientEnteredWorld(client_t* client, bool firstTime) {
        (void)client;
        (void)firstTime;
    }

    /// Gets called when the player's userinfo changes
    ///
    /// Userinfo is a string the client sends during connecting and when it changes. It's a backslash
    /// separated key-value list.
    ///
    /// @param client The player whose userinfo has changed.
    virtual void onClientUserInfoChanged(client_t* client) { (void)client; }

    /// Gets called whenever a client sends a command
    /// @param client The client who sent the command.
    /// @param command The command.
    virtual void onClientCommand(client_t* client, const char* command) {
        (void)client;
        (void)command;
    }

    /// Gets called whenever a client sends a move command
    /// @param client The client who sent the move command.
    /// @param ucmd The move command.
    virtual void onClientMoveCommand(client_t* client, usercmd_t* ucmd) {
        (void)client;
        (void)ucmd;
    }

    /// Gets called when a player gets killed
    /// @param self The entity of the player who died.
    /// @param inflictor The entity that caused the death (might be a grenade, car/barrel explosion, etc).
    /// @param attacker The entity of the player who initiated the death.
    /// @param damage The amount of damage inflicted.
    /// @param meansOfDeath An index to the defined array of means of death.
    /// @param iWeapon An index of the weapon that was used to kill the player. The weapon's name can be
    /// quieried using the BG_WeaponName() function.
    /// @param hitLocation An index to the defined array of hit locations.
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

    /// Gets called when a player requests joining to a reserved (password protected) slot
    /// @param from The IP address where the player is trying to connect from.
    /// @returns plugpp::ReservedSlotRequest::ALLOW to allow the player to join or
    /// plugpp::ReservedSlotRequest::DENY to deny the request.
    virtual ReservedSlotRequest onPlayerReservedSlotRequest(netadr_t* from) {
        (void)from;
        return ReservedSlotRequest::ALLOW;
    }

    /// Gets called when a `usercall` function gets called from a GSC script.
    ///
    /// Function arguments passed from the GSC script can be acquired using the @ref Plugin_Scr_GetNumParam(),
    /// @ref Plugin_Scr_GetInt(), @ref Plugin_Scr_GetFloat(), @ref Plugin_Scr_GetString(),
    /// @ref Plugin_Scr_GetEntity(), @ref Plugin_Scr_GetConstString(), @ref Plugin_Scr_GetType(),
    /// @ref Plugin_Scr_GetVector(), @ref Plugin_Scr_GetFunc() functions.
    /// As opposed to the @ref onScrUsercallMethod(), this function is called without a player object.
    ///
    /// @param functionName The function name supplied to the `usercall` GSC function call.
    virtual void onScrUsercallFunction(const std::string& functionName) { (void)functionName; }

    /// Gets called when a `usercall` method gets called from a GSC script.
    ///
    /// Metrod arguments passed from the GSC script can be acquired using the @ref Plugin_Scr_GetNumParam(),
    /// @ref Plugin_Scr_GetInt(), @ref Plugin_Scr_GetFloat(), @ref Plugin_Scr_GetString(),
    /// @ref Plugin_Scr_GetEntity(), @ref Plugin_Scr_GetConstString(), @ref Plugin_Scr_GetType(),
    /// @ref Plugin_Scr_GetVector(), @ref Plugin_Scr_GetFunc() functions.
    /// As opposed to the @ref onScrUsercallFunction(), this function is called with a specific player object.
    ///
    /// @param functionName The function name supplied to the `usercall` GSC method call.
    /// @param slot The slot of the player this function was called for.
    virtual void onScrUsercallMethod(const std::string& methodName, int slot) {
        (void)methodName;
        (void)slot;
    }

    /// This function is currently never called
    virtual void onModuleLoaded(client_t* client, const std::string& fullpath, long checksum) {
        (void)client;
        (void)fullpath;
        (void)checksum;
    }

    /// Gets called when a screenshot arrives on the server
    /// @param client The client captured in the arrived screenshot.
    /// @param path The path to the screenshot.
    virtual void onScreenshotArrived(client_t* client, const std::string& path) {
        (void)client;
        (void)path;
    }

    /// Gets called whenever there is a UDP packet event on the network
    ///
    /// This function is called only when compiled with -DOVERRIDE_UDP_EVENT_CALLBACKS
    ///
    /// @param from The IP address of the sender.
    /// @param data The pointer to the received data.
    /// @param size The size of the received data.
    /// @returns true if the packet should be dropped, false otherwise.
    virtual bool onUdpNetEvent(netadr_t* from, void* data, int size) {
        (void)from;
        (void)data;
        (void)size;
        return false;
    }

    /// Gets called whenever a UDP packet is sent from the server
    ///
    /// This function is called only when compiled with -DOVERRIDE_UDP_EVENT_CALLBACKS
    ///
    /// @param to The IP address of the receiver.
    /// @param data The pointer to the transmitted data.
    /// @param len The size of the transmitted data.
    /// @returns true if the packet should be dropped, false otherwise.
    virtual bool onUdpSend(netadr_t* to, void* data, int len) {
        (void)to;
        (void)data;
        (void)len;
        return false;
    }
};

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGIN_HPP_

#include "cod4-plugpp/PluginEntry.hpp"

#include "cod4-plugpp/Exception.hpp"

#include <cstring>
#include <type_traits>

static plugpp::PluginEntry* gEntry = nullptr;

namespace plugpp {

PluginEntry::PluginEntry() {
    Plugin_Printf("^2Plugin entry constructed\n");
    pluginMain(*this);
}

PluginEntry::~PluginEntry() {
    Plugin_Printf("^2Plugin entry destroyed\n");
}

Plugin& PluginEntry::getPlugin() {
    if (!mPlugin) {
        throw Exception("No plugin registered");
    }
    return *mPlugin.get();
}
} // namespace plugpp

template <typename TCallable>
std::enable_if_t<std::is_same_v<std::invoke_result_t<TCallable>, void>, void> callableError() {}

template <typename TCallable>
std::enable_if_t<std::is_same_v<std::invoke_result_t<TCallable>, int>, int> callableError() {
    return -1;
}

template <typename TCallable>
typename std::result_of<TCallable()>::type doNoexcept(TCallable&& callable) noexcept {
    try {
        return callable();
    } catch (const std::exception& e) {
        Plugin_Printf("^1Exception raised: %s\n", e.what());
    } catch (...) {
        Plugin_Printf("^1Unknown exception raised during plugin loading\n");
    }
    return callableError<TCallable>();
}

PCL int OnInit() {
    return doNoexcept([]() {
        gEntry = gEntry ? gEntry : new plugpp::PluginEntry();
        if (!gEntry) {
            Plugin_Printf("^1Failed to initialize plugin entry\n");
            return -1;
        }
        return 0;
    });
}

PCL void OnUnload() {
    doNoexcept([]() {
        if (gEntry) {
            delete gEntry;
            gEntry = nullptr;
        }
    });
}

PCL void OnTerminate() {
    doNoexcept([]() { gEntry->getPlugin().onTerminate(); });
}

PCL void OnPlayerConnect(int clientnum,
                         netadr_t* netaddress,
                         char* pbguid,
                         char* userinfo,
                         int authstatus,
                         char* deniedmsg,
                         int deniedmsgbufmaxlen) {
    // The plugin callback handler only passes an empty string here.
    (void)pbguid;

    // The plugin callback handler only passes 0 here.
    (void)authstatus;

    // The plugin callback handler has a bug that it can only pass 6 arguments to a callback function.
    // This argument is 7th so nothing is passed here.
    (void)deniedmsgbufmaxlen;

    if (!deniedmsg[0]) {
        doNoexcept([&]() {
            const plugpp::Kick kick = gEntry->getPlugin().onPlayerConnect(clientnum, netaddress, userinfo);
            if (kick) {
                std::strcpy(deniedmsg, kick->c_str());
            }
        });
    }
}

PCL void OnPlayerDC(client_t* client, const char* reason) {
    doNoexcept([&]() { gEntry->getPlugin().onPlayerDisconnect(client, reason); });
}

PCL void OnPlayerAddBan(baninfo_t* baninfo) {
    doNoexcept([&]() { gEntry->getPlugin().onPlayerAddBan(baninfo); });
}

PCL void OnPlayerRemoveBan(baninfo_t* baninfo) {
    return doNoexcept([&]() { gEntry->getPlugin().onPlayerRemoveBan(baninfo); });
}

PCL void OnPlayerGotAuthInfo(netadr_t* from,
                             uint64_t* playerid,
                             uint64_t* steamid,
                             char* rejectmsg,
                             qboolean* returnNow,
                             client_t* cl) {
    doNoexcept([&]() {
        bool returnNowNative = *returnNow == qboolean::qtrue;
        const plugpp::Kick kick =
            gEntry->getPlugin().onPlayerGotAuthInfo(cl, from, *playerid, *steamid, returnNowNative);

        if (kick && !rejectmsg[0]) {
            static constexpr std::size_t rejectMsgSize =
                1024; // From sv_client.c:1662 (SV_SendClientGameState())
            std::strncpy(rejectmsg, kick->c_str(), rejectMsgSize - 1);
        }

        *returnNow = returnNowNative ? qboolean::qtrue : qboolean::qfalse;
    });
}

PCL void OnPlayerGetBanStatus(baninfo_t* baninfo, char* message, int len) {
    if (!message[0]) {
        doNoexcept([&]() {
            const plugpp::Kick kick = gEntry->getPlugin().onPlayerGetBanStatus(baninfo);
            if (kick) {
                std::strncpy(message, kick->c_str(), len);
            } else {
                for (int i = 0; i < Plugin_GetSlotCount(); ++i) {
                    if (client_t* cl = Plugin_GetClientForClientNum(i);
                        cl && cl->playerid == baninfo->playerid && cl->steamid == baninfo->steamid &&
                        Plugin_NET_CompareAdr(&cl->netchan.remoteAddress, &baninfo->adr)) {
                        return gEntry->getPlugin().onPlayerAccessGranted(cl);
                    }
                }
                Plugin_Printf("^1ERROR: Failed to call onPlayerAccessGranted() callback for %s\n",
                              std::to_string(baninfo->playerid).c_str());
            }
        });
    }
}

PCL void OnInfoRequest(pluginInfo_t* info) {
    info->handlerVersion.major = PLUGIN_HANDLER_VERSION_MAJOR;
    info->handlerVersion.minor = PLUGIN_HANDLER_VERSION_MINOR;
    info->pluginVersion.major = 1;
    info->pluginVersion.minor = 0;
    Q_strncpyz(info->fullName, "C++ Plugin API Wrapper", sizeof(info->fullName));
    Q_strncpyz(info->shortDescription, "C++ wrapper for CoD4x plugin API", sizeof(info->shortDescription));
    Q_strncpyz(info->longDescription, "C++ wrapper for CoD4x plugin API", sizeof(info->longDescription));
    doNoexcept([&]() {
        if (gEntry) {
            plugpp::PluginInfo overrideInfo = gEntry->getPlugin().onPluginInfoRequest();
            if (overrideInfo.majorVersion) {
                info->pluginVersion.major = overrideInfo.majorVersion;
            }
            if (overrideInfo.minorVersion) {
                info->pluginVersion.minor = overrideInfo.minorVersion;
            }
            if (!overrideInfo.fullName.empty()) {
                Q_strncpyz(info->fullName, overrideInfo.fullName.c_str(), sizeof(info->fullName));
            }
            if (!overrideInfo.shortDescription.empty()) {
                Q_strncpyz(info->shortDescription,
                           overrideInfo.shortDescription.c_str(),
                           sizeof(info->shortDescription));
            }
            if (!overrideInfo.longDescription.empty()) {
                Q_strncpyz(info->longDescription,
                           overrideInfo.longDescription.c_str(),
                           sizeof(info->longDescription));
            }
        }
    });
}

PCL void OnMessageSent(char* message, int slot, qboolean* show, int mode) {
    doNoexcept([&]() {
        while (message && *message < 0x20) {
            ++message;
        }
        if (!message) {
            *show = qboolean::qfalse;
            return;
        }
        *show = (*show &&
                 gEntry->getPlugin().onMessageSent(message, slot, mode) == plugpp::MessageVisibility::SHOW)
                    ? qboolean::qtrue
                    : qboolean::qfalse;
    });
}

PCL void OnPreFastRestart() {
    doNoexcept([]() { gEntry->getPlugin().onPreFastRestart(); });
}

PCL void OnExitLevel() {
    doNoexcept([]() { gEntry->getPlugin().onExitLevel(); });
}

PCL void OnPostFastRestart() {
    doNoexcept([]() { gEntry->getPlugin().onPostFastRestart(); });
}

PCL void OnPreGameRestart(int savepersist) {
    doNoexcept([=]() { gEntry->getPlugin().onPreGameRestart(savepersist); });
}

PCL void OnPostGameRestart(int savepersist) {
    doNoexcept([=]() { gEntry->getPlugin().onPostGameRestart(savepersist); });
}

PCL void OnSpawnServer() {
    doNoexcept([]() { gEntry->getPlugin().onSpawnServer(); });
}

PCL void OnFrame() {
    doNoexcept([]() { gEntry->getPlugin().onFrame(); });
}

PCL void OnClientSpawn(gentity_t* ent) {
    doNoexcept([&]() { gEntry->getPlugin().onClientSpawn(ent); });
}

PCL void OnClientEnterWorld(client_t* client) {
    doNoexcept([&]() {
        gEntry->getPlugin().onClientEnteredWorld(
            client, client->connectedTime == static_cast<unsigned int>(Plugin_GetServerTime()));
    });
}

PCL void OnClientUserinfoChanged(client_t* client) {
    doNoexcept([&]() { gEntry->getPlugin().onClientUserInfoChanged(client); });
}

PCL void OnClientCommand(client_t* client, const char* command) {
    doNoexcept([&]() { gEntry->getPlugin().onClientCommand(client, command); });
}

PCL void OnClientMoveCommand(client_t* client, usercmd_t* ucmd) {
    doNoexcept([&]() { gEntry->getPlugin().onClientMoveCommand(client, ucmd); });
}

PCL void OnPlayerKilled(gentity_s* self,
                        gentity_s* inflictor,
                        gentity_s* attacker,
                        int damage,
                        int meansOfDeath,
                        int iWeapon,
                        hitLocation_t hitLocation) {
    doNoexcept([&]() {
        gEntry->getPlugin().onPlayerKilled(
            self, inflictor, attacker, damage, meansOfDeath, iWeapon, hitLocation);
    });
}

PCL void
OnPlayerWantReservedSlot(netadr_t* from, char* pbguid, char* userinfo, int authstate, qboolean* isallowed) {
    (void)pbguid;
    (void)userinfo;
    (void)authstate;
    doNoexcept([&]() {
        *isallowed = *isallowed && gEntry->getPlugin().onPlayerReservedSlotRequest(from) ==
                                       plugpp::ReservedSlotRequest::ALLOW
                         ? qboolean::qtrue
                         : qboolean::qfalse;
    });
}

PCL void OnFilesystemStarted(const searchpath_t* searchpaths) {
    doNoexcept([=]() { gEntry->getPlugin().onFsStarted(searchpaths); });
}

PCL void OnScrUsercallFunction(const char* function_name) {
    doNoexcept([=]() { gEntry->getPlugin().onScrUsercallFunction(function_name); });
}

PCL void OnScrUsercallMethod(const char* method_name, int clientnum) {
    doNoexcept([=]() { gEntry->getPlugin().onScrUsercallMethod(method_name, clientnum); });
}

PCL void OnModuleLoaded(client_t* client, char* fullpath, long checksum) {
    doNoexcept([=]() { gEntry->getPlugin().onModuleLoaded(client, fullpath, checksum); });
}

PCL void OnScreenshotArrived(client_t* client, const char* path) {
    doNoexcept([=]() { gEntry->getPlugin().onScreenshotArrived(client, path); });
}

// Intentionally not implementing these by default because of a slight overhead of the function call.
// If needed, compile with -DOVERRIDE_UDP_EVENT_CALLBACKS

#ifdef OVERRIDE_UDP_EVENT_CALLBACKS
PCL void OnUdpNetEvent(netadr_t* from, void* data, int size, qboolean* returnNow) {
    doNoexcept([&]() {
        const qboolean returnNowOverride =
            gEntry->getPlugin().onUdpNetEvent(from, data, size) ? qboolean::qtrue : qboolean::qfalse;
        *returnNow = (*returnNow || returnNowOverride) ? qboolean::qtrue : qboolean::qfalse;
    });
}

PCL void OnUdpNetSend(netadr_t* to, void* data, int len, qboolean* returnNow) {
    doNoexcept([&]() {
        const qboolean returnNowOverride =
            gEntry->getPlugin().onUdpSend(to, data, len) ? qboolean::qtrue : qboolean::qfalse;
        *returnNow = (*returnNow || returnNowOverride) ? qboolean::qtrue : qboolean::qfalse;
    });
}
#endif

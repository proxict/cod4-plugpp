#include "cod4-plugpp/PluginEntry.hpp"
#include "cod4-plugpp/Exception.hpp"

#include <cstring>

static plugpp::PluginEntry* gEntry = nullptr;

namespace plugpp {

PluginEntry::PluginEntry() {
    Plugin_Printf("^2Plugin entry constructed\n");
    pluginMain(*this);
}

PluginEntry::~PluginEntry() {
    Plugin_Printf("^2Plugin entry destroyed\n");
}
} // namespace plugpp

template <typename TCallable>
typename std::enable_if<std::is_same<typename std::result_of<TCallable()>::type, void>::value, void>::type
callableError() {}

template <typename TCallable>
typename std::enable_if<std::is_same<typename std::result_of<TCallable()>::type, int>::value, int>::type
callableError() {
    return -1;
}

template <typename TCallable>
typename std::result_of<TCallable()>::type doNoexcept(TCallable&& callable) noexcept {
    try {
        return callable();
    } catch (const plugpp::Exception& e) {
        Plugin_Printf("^1Exception raised: %s\n", e.what().c_str());
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
        return gEntry->getPlugin()->onPluginLoad();
    });
}

PCL void OnUnload() {
    doNoexcept([]() {
        gEntry->getPlugin()->onPluginUnload();
        if (gEntry) {
            delete gEntry;
            gEntry = nullptr;
        }
    });
}

PCL void OnTerminate() {
    doNoexcept([]() { gEntry->getPlugin()->onTerminate(); });
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

    doNoexcept([&]() {
        const plugpp::Kick kick = gEntry->getPlugin()->onPlayerConnect(clientnum, netaddress, userinfo);
        if (kick) {
            std::strcpy(deniedmsg, kick.value().c_str());
        }
    });
}

PCL void OnPlayerDC(client_t* client, const char* reason) {
    doNoexcept([&]() { gEntry->getPlugin()->onPlayerDisconnect(client, reason); });
}

PCL void OnPlayerAddBan(baninfo_t* baninfo) {
    doNoexcept([&]() { gEntry->getPlugin()->onPlayerAddBan(baninfo); });
}

PCL void OnPlayerRemoveBan(baninfo_t* baninfo) {
    return doNoexcept([&]() { gEntry->getPlugin()->onPlayerRemoveBan(baninfo); });
}

PCL void OnPlayerGetBanStatus(baninfo_t* baninfo, char* message, int len) {
    doNoexcept([&]() {
        const std::string status = gEntry->getPlugin()->onPlayerGetBanStatus(baninfo);
        if (!status.empty()) {
            std::strncpy(message, status.c_str(), len);
        }
    });
}

PCL void OnInfoRequest(pluginInfo_t* info) {
    info->handlerVersion.major = PLUGIN_HANDLER_VERSION_MAJOR;
    info->handlerVersion.minor = PLUGIN_HANDLER_VERSION_MINOR;
    info->pluginVersion.major = 1;
    info->pluginVersion.minor = 0;
    std::strncpy(info->fullName, "C++ Plugin API Wrapper", sizeof(info->fullName));
    std::strncpy(info->shortDescription, "C++ wrapper for CoD4x plugin API", sizeof(info->shortDescription));
    std::strncpy(info->longDescription, "C++ wrapper for CoD4x plugin API", sizeof(info->longDescription));
    doNoexcept([&]() {
        if (gEntry) {
            gEntry->getPlugin()->onPluginInfoRequest(info);
        }
    });
}

PCL void OnOneSecond() {
    doNoexcept([]() { gEntry->getPlugin()->onOneSecond(); });
}

PCL void OnTenSeconds() {
    doNoexcept([]() { gEntry->getPlugin()->onTenSeconds(); });
}

PCL void OnMessageSent(char* message, int slot, qboolean* show, int mode) {
    doNoexcept([&]() {
        *show = (*show && gEntry->getPlugin()->onMessageSent(message + 1, slot, mode) ==
                              plugpp::MessageVisibility::SHOW)
                    ? qboolean::qtrue
                    : qboolean::qfalse;
    });
}

PCL void OnPreFastRestart() {
    doNoexcept([]() { gEntry->getPlugin()->onPreFastRestart(); });
}

PCL void OnExitLevel() {
    doNoexcept([]() { gEntry->getPlugin()->onExitLevel(); });
}

PCL void OnPostFastRestart() {
    doNoexcept([]() { gEntry->getPlugin()->onPostFastRestart(); });
}

PCL void OnSpawnServer() {
    doNoexcept([]() { gEntry->getPlugin()->onSpawnServer(); });
}

PCL void OnFrame() {
    doNoexcept([]() { gEntry->getPlugin()->onFrame(); });
}

PCL void OnClientSpawn(gentity_t* ent) {
    doNoexcept([&]() { gEntry->getPlugin()->onClientSpawn(ent); });
}

PCL void OnClientEnterWorld(client_t* client) {
    doNoexcept([&]() { gEntry->getPlugin()->onClientEnteredWorld(client); });
}

PCL void OnClientUserinfoChanged(client_t* client) {
    doNoexcept([&]() { gEntry->getPlugin()->onClientUserInfoChanged(client); });
}

PCL void OnClientMoveCommand(client_t* client, usercmd_t* ucmd) {
    doNoexcept([&]() { gEntry->getPlugin()->onClientMoveCommand(client, ucmd); });
}

PCL void
OnPlayerWantReservedSlot(netadr_t* from, char* pbguid, char* userinfo, int authstate, qboolean* isallowed) {
    (void)pbguid;
    (void)userinfo;
    (void)authstate;
    doNoexcept([&]() {
        *isallowed =
            gEntry->getPlugin()->onPlayerReservedSlotRequest(from) == plugpp::ReservedSlotRequest::ALLOW
                ? qboolean::qtrue
                : qboolean::qfalse;
    });
}


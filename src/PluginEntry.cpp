#include "cod4-plugpp/PluginEntry.hpp"
#include "cod4-plugpp/Exception.hpp"

#include <cstring>

static plugpp::PluginEntry* gEntry = nullptr;

extern "C" __attribute__((constructor)) void onLibraryLoad() {
    try {
        gEntry = new plugpp::PluginEntry();
    } catch (const std::exception& e) {
        Plugin_Printf("^1Exception raised: %s^7\n", e.what());
    } catch (const plugpp::Exception& e) {
        Plugin_Printf("^1Exception raised: %s^7\n", e.what());
    } catch (...) {
        Plugin_Printf("^1Unknown exception raised^7\n");
    }
}

extern "C" __attribute__((destructor)) void onLibraryUnload() {
    if (gEntry) {
        delete gEntry;
        gEntry = nullptr;
    }
}

namespace plugpp {
PluginEntry::PluginEntry() {
    Plugin_Printf("^2Plugin entry constructed^7\n");
    pluginMain(*this);
}

PluginEntry::~PluginEntry() {
    Plugin_Printf("^2Plugin entry destroyed^7\n");
}
} // namespace plugpp

PCL int OnInit() {
    // If a plugin is unloaded and then loaded again, the C constructor won't be triggered, so we'll rely on
    // this function being called and re-construct the entry here. This has one implication that the
    // overridden onPluginInfoRequest won't be handled at the plugin startup.
    gEntry = gEntry ? gEntry : new plugpp::PluginEntry();
    if (!gEntry) {
        Plugin_Printf("^1Failed to load plugin^7\n");
        return 1;
    }
    try {
        return gEntry->getPlugin()->onPluginLoad();
    } catch (const std::exception& e) {
        Plugin_Printf("^1Exception raised: %s^7\n", e.what());
        return 1;
    } catch (const plugpp::Exception& e) {
        Plugin_Printf("^1Exception raised: %s^7\n", e.what());
        return 1;
    } catch (...) {
        Plugin_Printf("^1Unknown exception raised^7\n");
        return 1;
    }
    return 0;
}

PCL void OnUnload() {
    try {
        if (gEntry) {
            delete gEntry;
            gEntry = nullptr;
        }
    } catch (...) {
        Plugin_Printf("^1Exception raised during plugin unloading^7\n");
    }
}

PCL void OnTerminate() {
    try {
        gEntry->getPlugin()->onTerminate();
    } catch (...) {
        Plugin_Printf("^1Exception raised during plugin terminating^7\n");
    }
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

    const plugpp::Kick kick = gEntry->getPlugin()->onPlayerConnect(clientnum, netaddress, userinfo);
    if (kick) {
        std::strcpy(deniedmsg, kick.value().c_str());
    }
}

PCL void OnPlayerDC(client_t* client, const char* reason) {
    gEntry->getPlugin()->onPlayerDisconnect(client, reason);
}

PCL void OnInfoRequest(pluginInfo_t* info) {
    info->handlerVersion.major = PLUGIN_HANDLER_VERSION_MAJOR;
    info->handlerVersion.minor = PLUGIN_HANDLER_VERSION_MINOR;
    info->pluginVersion.major = 1;
    info->pluginVersion.minor = 0;
    std::strncpy(info->fullName, "C++ Plugin API Wrapper", sizeof(info->fullName));
    std::strncpy(info->shortDescription, "Wrapper around the C plugin API", sizeof(info->shortDescription));
    std::strncpy(info->longDescription,
                 "Fill information about your plugin by creating the onPluginInfoRequest function",
                 sizeof(info->longDescription));
    if (gEntry) {
        gEntry->getPlugin()->onPluginInfoRequest(info);
    }
}

PCL void OnOneSecond() {
    gEntry->getPlugin()->onOneSecond();
}

PCL void OnTenSeconds() {
    gEntry->getPlugin()->onTenSeconds();
}


#include <cod4-plugpp/Plugin.hpp>
#include <cod4-plugpp/PluginApi.hpp>
#include <cod4-plugpp/PluginEntry.hpp>

#include <cassert>

extern "C" {
void Q_strncpyz([[maybe_unused]] char* dest,
                [[maybe_unused]] const char* src,
                [[maybe_unused]] int destsize) {
    assert(false && "Not implemented");
}

void Plugin_RemoveCommand([[maybe_unused]] char* name) {
    assert(false && "Not implemented");
}

void Plugin_AddCommand([[maybe_unused]] const char* name,
                       [[maybe_unused]] xcommand_t command,
                       [[maybe_unused]] int defaultpower) {
    assert(false && "Not implemented");
}

void Plugin_Printf([[maybe_unused]] const char* fmt, ...) {
    assert(false && "Not implemented");
}

int Plugin_GetServerTime() {
    assert(false && "Not implemented");
    return 42;
}

int Plugin_GetSlotCount() {
    assert(false && "Not implemented");
    return 16;
}

qboolean Plugin_NET_CompareAdr([[maybe_unused]] netadr_t* a, [[maybe_unused]] netadr_t* b) {
    assert(false && "Not implemented");
    return qtrue;
}

client_t* Plugin_GetClientForClientNum([[maybe_unused]] int clientnum) {
    assert(false && "Not implemented");
    return nullptr;
}
} // extern "C"

class TestPlugin final : public plugpp::Plugin {
public:
    explicit TestPlugin() = default;
    virtual ~TestPlugin() = default;
};

void pluginMain([[maybe_unused]] plugpp::PluginEntry& entry) {
    entry.registerPlugin<TestPlugin>();
}

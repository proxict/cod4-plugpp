![GitHub](https://img.shields.io/github/license/proxict/cod4-plugpp)

cod4-plugpp
------
As the name may suggest, **cod4-plugpp**, or cod4-plug++, is a C++ wrapper for the [CoD4x plugin system](https://github.com/callofduty4x/cod4x-docs/tree/master/pluginsystem). As of now, only a fraction of the API is wrapped in C++, but it may still come in handy when developing new plugins in C++.
 
How to use
----------
First of all, make sure you're using a compatible revision of the [CoD4x server](https://github.com/callofduty4x/CoD4x_Server). This wrapper was only tested on revision [0581a7d](https://github.com/callofduty4x/CoD4x_Server/tree/0581a7dc6c53b834877103e5982d9c8ab845b3c3) and earlier versions may not work due to plugin API changes.

This library makes use of [CMake's object libraries](https://cmake.org/cmake/help/v3.5/command/add_library.html#object-libraries) so that the plugin can later link the objects from this wrapper into its shared library.

In code, it's really straightforward:
```c++
class MyPlugin final : public plugpp::Plugin {
public:
    explicit MyPlugin() {
        Plugin_EnterCriticalSection();
        mMyCvar = static_cast<cvar_t*>(
            Plugin_Cvar_RegisterString("my_cvar", "default value", 0, "I'm a cool CVAR description!"));
        if (!mMyCvar->string[0]) {
            // You can throw an exception if your CVAR is mandatory and wasn't provided:
            throw plugpp::Exception("CVAR \"my_cvar\" wasn't defined");
            // This will make loading of the plugin fail and the exceptionm message will be written into the server's console.
        }
        Plugin_LeaveCriticalSection();
    }

    virtual ~MyPlugin() {}
    
    /* ======================== These virtual functions can be overridden: =========================

    virtual int onPluginLoad() override;

    virtual void onOneSecond() override;

    virtual void onTenSeconds() override;

    virtual Kick onPlayerConnect(int clientnum, netadr_t* netaddr, const char* userinfo) override;

    virtual void onPlayerDisconnect(client_t* client, const char* reason) override;

    virtual void onTerminate() override;

    ============================================================================================= */

private:
    // Overriding this function is mandatory
    virtual void onPluginInfoRequest(pluginInfo_t* info) final override {
        std::strncpy(info->fullName, "Your plugin name", sizeof(info->fullName));
        std::strncpy(info->shortDescription, "Briefly describe your plugin", sizeof(info->shortDescription));
        std::strncpy(info->longDescription, "Fully describe your plugin here", sizeof(info->longDescription));
    }

    cvar_t* mMyCvar;
};

void pluginMain(plugpp::PluginEntry& entry) {
    entry.registerPlugin<MyPlugin>(/*If your class constructor has any arguments, pass them here*/);
    // The `MyPlugin` object will live throughout the entire lifetime of the dynamic library in the memory.
}
```

To see an example of integrating this library into your plugin, see the [cod4-pureip](https://github.com/proxict/cod4-pureip) plugin.


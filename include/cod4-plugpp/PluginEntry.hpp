#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINENTRY_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINENTRY_HPP_

#include <memory>
#include <utility>

#include "cod4-plugpp/Plugin.hpp"

namespace plugpp {

class PluginEntry final {
public:
    PluginEntry();

    ~PluginEntry();

    Plugin& getPlugin();

    template <typename TPlugin, typename... TArgs>
    void registerPlugin(TArgs&&... args) {
        mPlugin = std::make_unique<TPlugin>(std::forward<TArgs>(args)...);
    }

private:
    std::unique_ptr<Plugin> mPlugin;
};

} // namespace plugpp

void pluginMain(plugpp::PluginEntry& entry);

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_PLUGINENTRY_HPP_

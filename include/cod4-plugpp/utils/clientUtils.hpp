#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CLIENTUTILS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CLIENTUTILS_HPP_

#include "cod4-plugpp/Optional.hpp"
#include "cod4-plugpp/PluginApi.h"
#include "cod4-plugpp/utils/stringUtils.hpp"

#include <algorithm>
#include <vector>

namespace plugpp {

[[nodiscard]] inline Optional<client_t*> getClientBySlot(const int slot) {
    client_t* cl = Plugin_GetClientForClientNum(slot);
    if (!cl || !cl->state) {
        return NullOptional;
    }
    return cl;
}

[[nodiscard]] inline std::vector<std::pair<int, client_t*>> getActiveClients() {
    std::vector<std::pair<int, client_t*>> players;
    for (int i = 0; i < Plugin_GetSlotCount(); ++i) {
        if (auto client = getClientBySlot(i); client) {
            players.emplace_back(i, *client);
        }
    }
    return players;
}

enum class HandleType { INVALID, SLOTID, PLAYER_STEAMID, NAME };

[[nodiscard]] inline HandleType getHandleType(const std::string& handle) {
    if (handle.empty()) {
        return HandleType::INVALID;
    } else if (handle.size() <= 2 && isInteger(handle)) {
        return HandleType::SLOTID;
    } else if ((handle.size() == 17 || handle.size() == 19) && isInteger(handle)) {
        return HandleType::PLAYER_STEAMID;
    }
    return HandleType::NAME;
}

[[nodiscard]] inline Optional<client_t*> findClient(const std::string& handle) {
    const HandleType handleType = getHandleType(handle);
    switch (handleType) {
    case HandleType::INVALID:
        return NullOptional;
    case HandleType::SLOTID:
        return getClientBySlot(std::stoi(handle));
    case HandleType::PLAYER_STEAMID: {
        const auto players = getActiveClients();
        auto it = std::find_if(std::begin(players), std::end(players), [&handle](const auto& player) {
            return std::to_string(player.second->playerid) == handle ||
                   (player.second->steamid != 0 && std::to_string(player.second->steamid) == handle);
        });
        return it != std::end(players) ? Optional<client_t*>(it->second) : NullOptional;
    }
    case HandleType::NAME: {
        client_t* cl = nullptr;
        for (const auto& [slot, client] : getActiveClients()) {
            if (toLower(removeColor(client->name)).find(toLower(handle)) != std::string::npos) {
                if (!cl) {
                    cl = client;
                } else {
                    return NullOptional;
                }
            }
        }
        return cl ? Optional<client_t*>(cl) : NullOptional;
    }
    default:
        return NullOptional;
    }
}

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CLIENTUTILS_HPP_

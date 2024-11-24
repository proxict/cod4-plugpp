#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CLIENTUTILS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_CLIENTUTILS_HPP_

#include "cod4-plugpp/Optional.hpp"
#include "cod4-plugpp/PluginApi.hpp"
#include "cod4-plugpp/utils/stringUtils.hpp"

#include <algorithm>
#include <vector>

namespace plugpp {

/// Gets a client in the given slot if the slot is occupied by a player (active client)
/// @param slot The slot to get the client for.
/// @returns Pointer to the client or NullOptional if the slot is vacant.
[[nodiscard]] inline Optional<client_t*> getClientBySlot(const int slot) {
    client_t* cl = Plugin_GetClientForClientNum(slot);
    if (!cl || !cl->state) {
        return NullOptional;
    }
    return cl;
}

/// Gets all active clients (slots occupied by players and their corresponding client structures)
[[nodiscard]] inline std::vector<std::pair<int, client_t*>> getActiveClients() {
    std::vector<std::pair<int, client_t*>> players;
    for (int i = 0; i < Plugin_GetSlotCount(); ++i) {
        if (auto client = getClientBySlot(i); client) {
            players.emplace_back(i, *client);
        }
    }
    return players;
}

/// Handle type used to find a client on the server uniquely matching the given string
enum class HandleType { INVALID, SLOTID, PLAYER_STEAMID, NAME };

/// Gets the type of @ref HandleType based on the given string
/// @param handle A string that could represet the player's name, slot number, player ID or steam ID.
/// @returns @ref HandleType matching the given handle string.
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

/// Gets a client uniquely matching the given handle
/// @param handle A string that could represet the player's name, slot number, player ID or steam ID.
/// @returns A pointer to the client matching the given handle or NullOptional if zero or multiple players
/// match.
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

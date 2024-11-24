#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_

#include <string>
#include <optional>

namespace plugpp {

/// Represents a player kick reason for pluin's callbacks that can optionally return a kick.
using KickReason = std::string;

/// Represents a player kick with a reason for pluin's callbacks that can optionally return a kick.
using Kick = std::optional<KickReason>;

/// Represents a granted access for pluin's callbacks that can optionally return a kick.
inline constexpr std::nullopt_t NoKick(std::nullopt);

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_

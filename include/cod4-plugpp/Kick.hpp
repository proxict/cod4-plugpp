#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_

#include "cod4-plugpp/Optional.hpp"

#include <string>

namespace plugpp {

/// Represents a player kick reason for pluin's callbacks that can optionally return a kick.
using KickReason = std::string;

/// Represents a player kick with a reason for pluin's callbacks that can optionally return a kick.
using Kick = Optional<KickReason>;

/// Represents a granted access for pluin's callbacks that can optionally return a kick.
inline constexpr NullOptionalT NoKick(NullOptional);

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_

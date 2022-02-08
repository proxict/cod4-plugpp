#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_

#include "cod4-plugpp/Optional.hpp"

#include <string>

namespace plugpp {

using KickReason = std::string;
using Kick = Optional<KickReason>;

inline constexpr NullOptionalT NoKick(NullOptional);

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_

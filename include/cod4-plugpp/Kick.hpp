#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_

#include <lib-optional/optional.hpp>

namespace plugpp {

using namespace libOptional;

using KickReason = std::string;
using Kick = Optional<KickReason>;

inline constexpr NullOptionalT NoKick(NullOptional);

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_KICK_HPP_

#ifndef COD4_PLUGPP_INCLUDE_COD4_PLUGPP_TYPETRAITS_HPP_
#define COD4_PLUGPP_INCLUDE_COD4_PLUGPP_TYPETRAITS_HPP_

#include <functional>
#include <type_traits>

namespace plugpp {

template <typename T, typename TSignature>
struct IsCallable;

template <typename T, typename TRet, class... TArgs>
struct IsCallable<T, TRet(TArgs...)>
    : std::conditional<std::is_assignable<std::function<TRet(TArgs...)>, T>::value,
                       std::true_type,
                       std::false_type>::type {};

namespace detail::adlns {
    using std::begin;
    using std::end;

    template <typename T>
    auto IsIterable(int) -> decltype(begin(std::declval<T&>()) != end(std::declval<T&>()),
                                     void(),
                                     ++std::declval<decltype(begin(std::declval<T&>()))&>(),
                                     void(*begin(std::declval<T&>())),
                                     std::true_type{});

    template <typename T>
    std::false_type IsIterable(...);
} // namespace detail::adlns

template <typename T>
using IsIterable = decltype(detail::adlns::IsIterable<T>(0));

template <typename T>
inline constexpr bool IsIterable_v = IsIterable<T>::value;

namespace detail {
    template <typename T>
    struct reverse_wrapper {
        T& iterable;
    };

    template <typename T>
    auto begin(reverse_wrapper<T> w) {
        return std::rbegin(w.iterable);
    }

    template <typename T>
    auto end(reverse_wrapper<T> w) {
        return std::rend(w.iterable);
    }

} // namespace detail

} // namespace plugpp

#endif // COD4_PLUGPP_INCLUDE_COD4_PLUGPP_TYPETRAITS_HPP_

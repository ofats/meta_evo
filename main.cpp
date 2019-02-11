#include <utility>
#include <array>
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <cassert>
#include <set>

template <class... Ts>
struct type_pack {};

template <class T>
struct just_type { using type = T; };

template <class... Ts, class... Us>
constexpr bool operator==(type_pack<Ts...>, type_pack<Us...>) { return false; }
template <class... Ts>
constexpr bool operator==(type_pack<Ts...>, type_pack<Ts...>) { return true; }

template <class... Ts, class... Us>
constexpr bool operator!=(type_pack<Ts...>, type_pack<Us...>) { return true; }
template <class... Ts>
constexpr bool operator!=(type_pack<Ts...>, type_pack<Ts...>) { return false; }

template <class T, class U>
constexpr bool operator==(just_type<T>, just_type<U>) { return false; }
template <class T>
constexpr bool operator==(just_type<T>, just_type<T>) { return true; }

template <class T, class U>
constexpr bool operator!=(just_type<T>, just_type<U>) { return true; }
template <class T>
constexpr bool operator!=(just_type<T>, just_type<T>) { return false; }

template <class... Ts, class... Us>
constexpr auto operator+(type_pack<Ts...>, type_pack<Us...>) {
    return type_pack<Ts..., Us...>{};
}

namespace tp {

using empty_pack = type_pack<>;

template <class... Ts>
constexpr size_t size(type_pack<Ts...>) {
    return sizeof...(Ts);
}

template <class... Ts>
constexpr bool empty(type_pack<Ts...> tp) {
    return size(tp) == 0;
}

static_assert(empty(empty_pack{}));
static_assert(!empty(type_pack<int, double>{}));

template <class T, class... Ts>
constexpr just_type<T> head(type_pack<T, Ts...>) {
    return {};
}

template <class T, class... Ts>
constexpr type_pack<Ts...> tail(type_pack<T, Ts...>) {
    return {};
}

// ==================== push front ====================

// type-based
template <class T, class... Ts>
constexpr type_pack<T, Ts...> push_front(type_pack<Ts...>) {
    return {};
}

// value-based
template <class... Ts, class T>
constexpr type_pack<T, Ts...> push_front(type_pack<Ts...>, just_type<T>) {
    return {};
}

static_assert(push_front<int>(type_pack<double, char>{}) ==
              type_pack<int, double, char>{});

// ==================== pop front ====================

template <class T, class... Ts>
constexpr type_pack<Ts...> pop_front(type_pack<T, Ts...>) {
    return {};
}

static_assert(pop_front(type_pack<int, double, char>{}) ==
              type_pack<double, char>{});

// ==================== push back ====================

// type-based
template <class T, class... Ts>
constexpr type_pack<Ts..., T> push_back(type_pack<Ts...>) {
    return {};
}

// value-based
template <class... Ts, class T>
constexpr type_pack<Ts..., T> push_back(type_pack<Ts...>, just_type<T>) {
    return {};
}

static_assert(push_back<int>(type_pack<double, char>{}) ==
              type_pack<double, char, int>{});

// ==================== contains ====================

namespace bad {

template <class U, class... Ts>
struct contains_impl;

template <class U, class... Ts>  // found case
struct contains_impl<U, U, Ts...> : std::true_type {};

template <class U, class T, class... Ts>  // continue case
struct contains_impl<U, T, Ts...> : contains_impl<U, Ts...> {};

template <class U>  // not found case
struct contains_impl<U> : std::false_type {};

// type-based
template <class T, class... Ts>
constexpr bool contains(type_pack<Ts...>) {
    return contains_impl<T, Ts...>::value;
}

// value-based
template <class... Ts, class T>
constexpr bool contains(type_pack<Ts...>, just_type<T>) {
    return contains_impl<T, Ts...>::value;
}

static_assert(contains<int>(type_pack<int, double, char>{}));
static_assert(!contains<int*>(type_pack<int, double, char>{}));
static_assert(!contains<int>(empty_pack{}));

}  // namespace bad

namespace good {

// type-based
template <class T, class... Ts>
constexpr bool contains(type_pack<Ts...>) {
    bool bs[] = {std::is_same<T, Ts>::value...};
    bool res = false;
    for (bool b : bs) {
        res |= b;
    }
    return res;
}

// value-based
template <class... Ts, class T>
constexpr bool contains(type_pack<Ts...> tp, just_type<T>) {
    return contains<T>(tp);
}

static_assert(contains<int>(type_pack<int, double, char>{}));
static_assert(!contains<int*>(type_pack<int, double, char>{}));
static_assert(!contains<int>(empty_pack{}));

}  // namespace good

namespace best {

// type-based
template <class T, class... Ts>
constexpr bool contains(type_pack<Ts...>) {
    return (... || std::is_same_v<T, Ts>);
}

// value-based
template <class... Ts, class T>
constexpr bool contains(type_pack<Ts...> tp, just_type<T>) {
    return contains<T>(tp);
}

static_assert(contains<int>(type_pack<int, double, char>{}));
static_assert(!contains<int*>(type_pack<int, double, char>{}));
static_assert(!contains<int>(empty_pack{}));

}  // namespace best

// ==================== find ====================

namespace bad {

template <class U, class... Ts>
struct find_impl;

template <class U, class... Ts>  // found case
struct find_impl<U, U, Ts...> : std::integral_constant<size_t, 0> {};

template <class U, class T, class... Ts>  // continue case
struct find_impl<U, T, Ts...>
    : std::integral_constant<size_t, find_impl<U, Ts...>::value + 1> {};

template <class U>  // not found case (value == size(tp))
struct find_impl<U> : std::integral_constant<size_t, 0> {};

// type-based
template <class U, class... Ts>
constexpr size_t find(type_pack<Ts...>) {
    return find_impl<U, Ts...>::value;
}

// value-based
template <class... Ts, class U>
constexpr size_t find(type_pack<Ts...> tp, just_type<U>) {
    return find<U>(tp);
}

static_assert(find<int>(type_pack<double, int, char>{}) == 1);
static_assert(find<int*>(type_pack<double, int, char>{}) == 3);

}  // namespace bad

namespace good {

// type-based
template <class T, class... Ts>
constexpr size_t find(type_pack<Ts...> tp) {
    bool bs[] = {std::is_same_v<T, Ts>...};
    for (size_t i = 0; i < size(tp); ++i) {
        if (bs[i]) {
            return i;
        }
    }
    return size(tp);
}

// value-based
template <class... Ts, class T>
constexpr size_t find(type_pack<Ts...> tp, just_type<T>) {
    return find<T>(tp);
}

static_assert(find<int>(type_pack<double, int, char>{}) == 1);
static_assert(find<int*>(type_pack<double, int, char>{}) == 3);

}  // namespace good

namespace best {

// type-based
template <class T, class... Ts>
constexpr size_t find(type_pack<Ts...> tp) {
    bool bs[] = {std::is_same_v<T, Ts>...};
    return std::find(bs, bs + size(tp), true) - bs;
}

// value-based
template <class... Ts, class T>
constexpr size_t find(type_pack<Ts...> tp, just_type<T>) {
    return find<T>(tp);
}

}  // namespace best

// ==================== find if ====================

template <template <class...> class F, class... Ts>
constexpr size_t find_if(type_pack<Ts...> tp) {
    bool bs[] = {F<Ts>::value...};
    for (size_t i = 0; i < size(tp); ++i) {
        if (bs[i]) {
            return i;
        }
    }
    return size(tp);
}

// ==================== any, all, none of ====================

template <template <class...> class F, class... Ts>
constexpr bool all_of(type_pack<Ts...>) {
    return (... && F<Ts>::value);
}

template <template <class...> class F, class... Ts>
constexpr bool any_of(type_pack<Ts...>) {
    return (... || F<Ts>::value);
}

template <template <class...> class F, class... Ts>
constexpr bool none_of(type_pack<Ts...> tp) {
    return !any_of<F>(tp);
}

static_assert(all_of<std::is_pointer>(type_pack<int*, double*, char*>{}));
static_assert(all_of<std::is_pointer>(empty_pack{}));
static_assert(any_of<std::is_reference>(type_pack<int&, double, char**>{}));
static_assert(!any_of<std::is_reference>(empty_pack{}));
static_assert(none_of<std::is_void>(type_pack<int, double, char>{}));
static_assert(none_of<std::is_void>(empty_pack{}));

// ==================== transform ====================

template <template <class...> class F, class... Ts>
constexpr type_pack<typename F<Ts>::type...> transform(type_pack<Ts...>) {
    return {};
}

static_assert(transform<std::add_pointer>(type_pack<int, double, char>{}) ==
              type_pack<int*, double*, char*>{});

// ==================== reverse ====================

template <class... Ts>
constexpr type_pack<Ts...> reverse_impl(empty_pack, type_pack<Ts...>) {
    return {};
}

template <class T, class... Ts, class... Us>
constexpr auto reverse_impl(type_pack<T, Ts...>, type_pack<Us...>) {
    return reverse_impl(type_pack<Ts...>{}, type_pack<T, Us...>{});
}

template <class... Ts>
constexpr auto reverse(type_pack<Ts...> tp) {
    return reverse_impl(tp, {});
}

static_assert(reverse(type_pack<int, double, char>{}) ==
              type_pack<char, double, int>{});

// ==================== get ====================

namespace bad {

template <size_t I, class... Ts>
struct get_impl;

template <class T, class... Ts>  // found case
struct get_impl<0, T, Ts...> { using type = T; };

template <size_t I, class T, class... Ts>  // continue case
struct get_impl<I, T, Ts...> {
    using type = typename get_impl<I - 1, Ts...>::type;
};

template <size_t I>  // not found case, SFINAE-frendly
struct get_impl<I> {};

template <size_t I, class... Ts>
constexpr auto get(type_pack<Ts...>) {
    return just_type<typename get_impl<I, Ts...>::type>{};
}

static_assert(get<1>(type_pack<double, int, char>{}) == just_type<int>{});

}  // namespace bad

namespace good {

template <size_t I, class T>
struct indexed_type {
    static constexpr size_t value = I;
    using type = T;
};

template <class IS, class... Ts>
struct indexed_types;

template <size_t... Is, class... Ts>
struct indexed_types<std::index_sequence<Is...>, Ts...> {
    struct type : indexed_type<Is, Ts>... {};
};

template <class... Ts>
using indexed_types_for =
    typename indexed_types<std::index_sequence_for<Ts...>, Ts...>::type;

template <size_t I, class T>
constexpr just_type<T> get_indexed_type(indexed_type<I, T>) {
    return {};
}

template <size_t I, class... Ts>
constexpr auto get(type_pack<Ts...>) {
    return get_indexed_type<I>(indexed_types_for<Ts...>{});
}

static_assert(get<1>(type_pack<double, int, char>{}) == just_type<int>{});

}  // namespace good

namespace best {

template <class IS>
struct get_impl;

template <size_t... Is>
struct get_impl<std::index_sequence<Is...>> {
    template <class T>
    static constexpr T dummy(decltype(Is, (void*)0)..., T*, ...);
};

template <size_t I, class... Ts>
constexpr auto get(type_pack<Ts...>) {
    return just_type<decltype(
        get_impl<std::make_index_sequence<I>>::dummy((Ts*)(0)...))>{};
}

static_assert(get<1>(type_pack<double, int, char>{}) == just_type<int>{});

}  // namespace best

// ==================== generate ====================

namespace bad {

template <size_t I, class T, class... Ts>  // continue case
struct generate_impl {
    using type = typename generate_impl<I - 1, T, Ts..., T>::type;
};

template <class T, class... Ts>  // finish case
struct generate_impl<0, T, Ts...> {
    using type = type_pack<Ts...>;
};

template <size_t I, class T>
constexpr auto generate() {
    return typename generate_impl<I, T>::type{};
}

static_assert(generate<3, int>() == type_pack<int, int, int>{});
static_assert(generate<3, int>() != type_pack<int, int, double>{});

}  // namespace bad

namespace best {

template <class... Ts>
constexpr type_pack<Ts...> generate_helper(Ts*...) {
    return {};
}

template <class T, size_t... Is>
constexpr auto generate_impl(std::index_sequence<Is...>) {
    return generate_helper(((void)Is, (T*)0)...);
}

template <size_t I, class T>
constexpr auto generate() {
    return generate_impl<T>(std::make_index_sequence<I>{});
}

static_assert(generate<3, int>() == type_pack<int, int, int>{});
static_assert(generate<3, int>() != type_pack<int, int, double>{});

}  // namespace best

// ==================== filter ====================

namespace bad {

template <template <class...> class F, class... Us>
constexpr type_pack<Us...> filter_impl(empty_pack, type_pack<Us...>) {
    return {};
}

template <template <class...> class F, class T, class... Ts, class... Us>
constexpr auto filter_impl(type_pack<T, Ts...>, type_pack<Us...>) {
    if constexpr (F<T>::value) {
        return filter_impl<F>(type_pack<Ts...>{}, type_pack<Us..., T>{});
    } else {
        return filter_impl<F>(type_pack<Ts...>{}, type_pack<Us...>{});
    }
}

template <template <class...> class F, class... Ts>
constexpr auto filter(type_pack<Ts...> tp) {
    return filter_impl<F>(tp, {});
}

static_assert(filter<std::is_pointer>(type_pack<char, double*, int*>{}) ==
              type_pack<double*, int*>{});
static_assert(filter<std::is_pointer>(empty_pack{}) == empty_pack{});

}  // bad

namespace best {

template <template <class...> class F, class T>
constexpr auto filter_one() {
    if constexpr (F<T>::value) {
        return type_pack<T>{};
    } else {
        return empty_pack{};
    }
}

template <template <class...> class F, class... Ts>
constexpr auto filter(type_pack<Ts...>) {
    return (empty_pack{} + ... + filter_one<F, Ts>());
}

static_assert(filter<std::is_pointer>(type_pack<char, double*, int*>{}) ==
              type_pack<double*, int*>{});
static_assert(filter<std::is_pointer>(empty_pack{}) == empty_pack{});

}  // best

}  // namespace tp

int main() {
    constexpr type_pack<int, double> tp1;
    constexpr auto tp2 = tp::push_back<char*>(tp1);
    constexpr auto tp3 = tp::push_front(tp2, just_type<std::string>{});
    static_assert(tp3 == type_pack<std::string, int, double, char*>{});
    constexpr auto tp4 = tp::best::filter<std::is_scalar>(tp3);
    static_assert(tp4 == type_pack<int, double, char*>{});
    static_assert(tp::find_if<std::is_pointer>(tp4) == 2);
    constexpr auto tp5 = tp::transform<std::add_lvalue_reference>(tp4) +
                         tp::best::generate<2, short**>();
    static_assert(tp5 == type_pack<int&, double&, char*&, short**, short**>{});

    return 0;
}

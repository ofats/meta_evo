#include <utility>
#include <array>
#include <iostream>
#include <vector>
#include <tuple>

template <int... ids>
struct int_sequence {};

template <int head, int... tail>
struct my_iota {
    using type = typename my_iota<head - 1, head - 1, tail...>::type;
};

template <int... tail>
struct my_iota<0, tail...> {
    using type = int_sequence<tail...>;
};

template <class F, class Tuple, int... ids>
auto call_with_tuple_impl(F&& f, Tuple&& t, int_sequence<ids...>)
    -> decltype(std::forward<F>(f)(std::get<ids>(std::forward<Tuple>(t))...)) {
    return std::forward<F>(f)(std::get<ids>(std::forward<Tuple>(t))...);
}

template <class F, class Tuple>
auto call_with_tuple(F&& f, Tuple&& t) -> decltype(call_with_tuple_impl(
    std::forward<F>(f), std::forward<Tuple>(t),
    typename my_iota<
        std::tuple_size<typename std::decay<Tuple>::type>::value>::type{})) {
    return call_with_tuple_impl(
        std::forward<F>(f), std::forward<Tuple>(t),
        typename my_iota<
            std::tuple_size<typename std::decay<Tuple>::type>::value>::type{});
}

double foo(int a, double b, char c, std::string s) {
    return a + b + c + s.size();
}

int main() {
    static_assert(std::is_same<int_sequence<0, 1, 2, 3>, typename my_iota<4>::type>::value, "");
    std::tuple<int, double, char, std::string> t{5, 6.0, 'a', "abc"};
    std::cout << call_with_tuple(foo, t) << std::endl;;

    return 0;
}

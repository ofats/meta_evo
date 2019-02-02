#include <utility>
#include <array>
#include <iostream>
#include <vector>
#include <tuple>

template <class F, class Tuple, size_t... ids>
decltype(auto) call_with_tuple_impl(F&& f, Tuple&& t,
                                    std::index_sequence<ids...>) {
    return std::forward<F>(f)(std::get<ids>(std::forward<Tuple>(t))...);
}

template <class F, class Tuple>
decltype(auto) call_with_tuple(F&& f, Tuple&& t) {
    return call_with_tuple_impl(
        std::forward<F>(f), std::forward<Tuple>(t),
        std::make_index_sequence<
            std::tuple_size<std::decay_t<Tuple>>::value>{});
}

double foo(int a, double b, char c, std::string s) {
    return a + b + c + s.size();
}

int main() {
    std::tuple<int, double, char, std::string> t{5, 6.0, 'a', "abc"};
    std::cout << call_with_tuple(foo, t) << std::endl;;

    return 0;
}

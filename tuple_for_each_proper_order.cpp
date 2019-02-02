#include <utility>
#include <array>
#include <iostream>
#include <vector>
#include <tuple>

template <class F, class Tuple, size_t... ids>
void call_for_each_impl(F&& f, Tuple&& t, std::index_sequence<ids...>) {
    int dummy[] = {
        ((void)std::forward<F>(f)(std::get<ids>(std::forward<Tuple>(t))), 0)...
    };
    (void)dummy;
}

template <class F, class Tuple>
void call_for_each(F&& f, Tuple&& t) {
    call_for_each_impl(std::forward<F>(f), std::forward<Tuple>(t),
                       std::make_index_sequence<
                           std::tuple_size<std::decay_t<Tuple>>::value>{});
}

int main() {
    struct visitor {
        void operator()(int) {
            std::cout << "int given!" << std::endl;
        }
        void operator()(double) {
            std::cout << "double given!" << std::endl;
        }
        void operator()(std::string) {
            std::cout << "string given!" << std::endl;
        }
    } v;

    std::tuple<int, double, std::string> t;
    call_for_each(v, t);

    return 0;
}

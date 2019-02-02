#include <utility>
#include <array>
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>

template <class F, class Tuple>
void call_for_each(F&& f, Tuple&& t) {
    std::apply([&](auto&&... args) {
        (..., std::invoke(std::forward<F>(f), std::forward<decltype(args)>(args)));
    }, std::forward<Tuple>(t));
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

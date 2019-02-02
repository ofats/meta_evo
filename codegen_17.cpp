#include <utility>
#include <array>
#include <iostream>
#include <vector>
#include <tuple>

double foo(int a, double b, char c, std::string s) {
    return a + b + c + s.size();
}

int main() {
    std::tuple<int, double, char, std::string> t{5, 6.0, 'a', "abc"};
    std::cout << std::apply(foo, t) << std::endl;;

    return 0;
}

#include <utility>
#include <array>
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <cassert>

// namespace std {

template <class T>
class vector {
    using value_type = T;

    template <class InputIt>
    vector(InputIt f, InputIt l);

    vector(size_t sz, value_type val);
};

// }  // namespace std


int main() {
    std::vector<int> v(5, 1);
    std::cout << (v == std::vector{1, 1, 1, 1, 1}) << std::endl;

    return 0;
}

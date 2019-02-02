#include <utility>
#include <array>
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <cassert>
#include <set>

namespace meta {

template <class It, class = void>
struct is_iterator : std::false_type {};

template <class It>
using iterator_category = typename std::iterator_traits<It>::iterator_category;

template <class It>
struct is_iterator<It, std::void_t<iterator_category<It>>> : std::true_type {};

}  // namespace meta

int main() {
    std::vector<int> v(5, 1);

    std::cout << meta::is_iterator<std::vector<int>::iterator>::value;
    std::cout << meta::is_iterator<std::set<int>::iterator>::value;
    std::cout << meta::is_iterator<std::string::iterator>::value;
    std::cout << meta::is_iterator<std::string::const_iterator>::value;
    std::cout << meta::is_iterator<int>::value;
    std::cout << meta::is_iterator<int*>::value;

    return 0;
}

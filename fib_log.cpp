#include <utility>
#include <array>
#include <iostream>

using matrix_t = std::array<size_t, 4>;

constexpr matrix_t unit_matrix{1, 0, 0, 1};

// 0, 1
// 2, 3
constexpr matrix_t mul(const matrix_t& a, const matrix_t& b) {
    return {a[0] * b[0] + a[1] * b[2], a[0] * b[1] + a[1] * b[3],
            a[2] * b[0] + a[3] * b[2], a[2] * b[1] + a[3] * b[3]};
}

constexpr matrix_t pow(const matrix_t& mat, size_t st) {
    if (st == 0) {
        return unit_matrix;
    }
    auto ret = pow(mul(mat, mat), st >> 1);
    return (st & 1 ? mul(ret, mat) : ret);
}

constexpr size_t fib(size_t idx) { return pow({1, 1, 1, 0}, idx)[1]; }

int main() {
    matrix_t mat{{1, 1, 1, 0}};
    for (int i = 0; i < 10; ++i) {
        auto res = pow(mat, i);
        std::cerr << res[0] << ' ' << res[1] << std::endl
                  << res[2] << ' ' << res[3] << std::endl
                  << std::endl;
    }

    return 0;
}

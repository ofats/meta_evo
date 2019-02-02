#include <utility>
#include <array>
#include <iostream>
#include <vector>

class vec_t {
public:
    vec_t(size_t sz) : data_(sz) {}

    double operator[](size_t idx) const { return data_[idx]; }
    double& operator[](size_t idx) { return data_[idx]; }
    size_t size() const noexcept { return data_.size(); }

private:
    std::vector<double> data_;
};

vec_t operator+(const vec_t& a, const vec_t& b) {
    // assert_equals(a.size(), b.size())
    vec_t res{a.size()};
    for (size_t i = 0; i < a.size(); ++i) {
        res[i] = a[i] + b[i];
    }
    return res;
}

vec_t operator*(const vec_t& v, double s) {
    vec_t res{v.size()};
    for (size_t i = 0; i < v.size(); ++i) {
        res[i] = v[i] * s;
    }
    return res;
}

vec_t operator*(double s, const vec_t& v) { return v * s; }

int main() {
    vec_t a{3}, b{3};
    auto c = a * 42 + b * 35;
    auto d = c * 8800 + b * 55 + a * 3535;
    for (size_t i = 0 ; i < c.size(); ++i) {
        std::cerr << c[i] << ' ';
    }
    std::cerr << std::endl;
    c = c * 2;
    for (size_t i = 0 ; i < c.size(); ++i) {
        std::cerr << c[i] << ' ';
    }
    std::cerr << std::endl;

    return 0;
}

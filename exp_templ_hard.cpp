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

template <class LeftExp, class RightExp>
class vec_sum_t {
public:
    template <class A, class B>
    vec_sum_t(A&& a, B&& b)
        : left_exp_(std::forward<A>(a)), right_exp_(std::forward<B>(b)) {
        // assert_equal(left_exp_.size(), right_exp_size());
    }

    size_t size() const noexcept { return left_exp_.size(); }

    double operator[](size_t idx) const {
        return left_exp_[idx] + right_exp_[idx];
    }
    operator vec_t() const {
        vec_t res{size()};
        for (size_t i = 0; i < res.size(); ++i) {
            res[i] = (*this)[i];
        }
        return res;
    }

private:
    LeftExp left_exp_;
    RightExp right_exp_;
};

template <class A, class B>
vec_sum_t<A, B> operator+(A&& a, B&& b) {
    return vec_sum_t<A, B>{std::forward<A>(a), std::forward<B>(b)};
}

template <class Exp>
class vec_mul_t {
public:
    template <class E>
    vec_mul_t(E&& exp, double scalar)
        : exp_(std::forward<E>(exp)), scalar_(scalar) {}

    size_t size() const noexcept { return exp_.size(); }
    double operator[](size_t idx) const { return exp_[idx] * scalar_; }

    operator vec_t() const {
        vec_t res{size()};
        for (size_t i = 0; i < res.size(); ++i) {
            res[i] = (*this)[i];
        }
        return res;
    }

private:
    Exp exp_;
    double scalar_;
};

template <class V>
vec_mul_t<V> operator*(V&& v, double s) {
    return vec_mul_t<V>{std::forward<V>(v), s};
}

template <class V>
vec_mul_t<V> operator*(double s, V&& v) {
    return v * s;
}

int main() {
    vec_t a{3}, b{3};
    auto c = a * 42 + b * 35;
    static_assert(
        std::is_same_v<decltype(c),
                       vec_sum_t<vec_mul_t<vec_t&>, vec_mul_t<vec_t&>>>);
    auto d = c * 8800 + b * 55 + a * 3535;
    for (size_t i = 0 ; i < d.size(); ++i) {
        std::cerr << d[i] << ' ';
    }
    std::cerr << std::endl;
    auto f = c * 2;
    for (size_t i = 0 ; i < f.size(); ++i) {
        std::cerr << f[i] << ' ';
    }
    std::cerr << std::endl;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    vec_t e{(a + a) * 5};
    for (size_t i = 0; i < e.size(); ++i) {
        std::cerr << e[i] << ' ';
    }
    std::cerr << std::endl;

    return 0;
}

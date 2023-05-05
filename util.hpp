#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>

template <typename T>
class vec2 {
public:
    T x;
    T y;

    vec2(T x, T y) : x(x), y(y) {}

    T length2() {
        return x * x + y * y;
    };

    T length() {
        return sqrt(length2());
    }

    static vec2<T> read() {
        T x, y;
        std::cin >> x >> y;
        return vec2(x, y);
    };

    void print() {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
    
    vec2<T> operator-() const {
        return vec2(-x, -y);
    }

    vec2<T>& operator+=(const vec2<T>& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    vec2<T>& operator-=(const vec2<T>& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    friend vec2<T> operator+(vec2<T> lhs, const vec2<T> rhs) {
        lhs += rhs;
        return lhs;
    }

    friend vec2<T> operator-(vec2<T> lhs, const vec2<T> rhs) {
        lhs -= rhs;
        return lhs;
    }
    
    friend T dot(vec2<T>& a, vec2<T>& b) {
        return a.x * b.x + a.y * b.y;
    }

    friend T det(vec2<T>& a, vec2<T>& b) {
        return a.x * b.y - a.y * b.x;
    }
};

#endif

#pragma once

#include <iostream>
#include <cmath>

class vec2 {
public:
    double x;
    double y;

    vec2() = default;

    vec2(double x, double y) : x(x), y(y) {}

    double length2() {
        return x * x + y * y;
    };

    double length() {
        return sqrt(length2());
    }

    static vec2 read() {
        double x, y;
        std::cin >> x >> y;
        return vec2(x, y);
    };

    void print() {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
    
    vec2 operator-() const {
        return vec2(-x, -y);
    }

    vec2& operator+=(const vec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    vec2& operator-=(const vec2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    friend vec2 operator+(vec2 lhs, const vec2 rhs) {
        lhs += rhs;
        return lhs;
    }

    friend vec2 operator-(vec2 lhs, const vec2 rhs) {
        lhs -= rhs;
        return lhs;
    }
    
    friend double dot(const vec2& a, const vec2& b) {
        return a.x * b.x + a.y * b.y;
    }

    friend double det(const vec2& a, const vec2& b) {
        return a.x * b.y - a.y * b.x;
    }

    friend bool operator==(const vec2& a, const vec2& b) {
        return a.x == b.x && a.y == b.y;
    }
};

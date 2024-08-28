#pragma once
#include "common.h"
template <class T>
struct Vector2 {
    T x;
    T y;
    Vector2(T _x = 0, T _y = 0): x{_x}, y{_y} {}
    Vector2 operator+(const Vector2& a) const {
        return {x + a.x, y + a.y};
    }
    Vector2& operator+=(const Vector2& a) {
        x += a.x;
        y += a.y;
        return *this;
    }
    Vector2 operator-(const Vector2& a) const {
        return {x - a.x, y - a.y};
    }
    Vector2& operator-=(const Vector2& a) {
        this->x -= a.x;
        this->y -= a.y;
        return *this;
    }
    Vector2 operator*(const Vector2& a)  const {
        return {x * a.x, y * a.y};
    }
    friend Vector2 operator*(const Vector2& a, T scalar) {
        return {a.x * scalar, a.y * scalar};
    }
    friend Vector2 operator*(T scalar, const Vector2& a) {
        return a * scalar;
    }
    bool operator==(const Vector2& a) const {
        return x == a.x and y == a.y;
    }
    Vector2 operator/(T scalar) const {
        return {x / scalar, y / scalar};
    }
    T dot(const Vector2& a) const {
        return x * a.x + y * a.y; 
    }
    Vector2 unit() const {
        Expects(magnitude() != 0);
        return (*this) / magnitude();
    }
    T magnitude() const {
        return static_cast<T>(sqrt(pow(x, 2) + pow(y, 2))); 
    }
};
using V2 = Vector2<float>;
struct Rect {
    float x{0};
    float y{0};
    float w{0};
    float h{0};
};
struct Color {
    uint8 r{0};
    uint8 g{0};
    uint8 b{0};
    uint8 a{0};
};
struct Circle {
    V2 center;
    float radius;
};
struct Physical_properties {
    bool welded{false};
    bool collidable{true};
    float elasticity{0};
    float mass{1};
    float friction{.3f};
};
struct View_transform {
    V2 translation{0, 0};
    V2 scaling{1, 1};
    V2 viewport{0, 0};
    //double rotation{0};
    V2 apply(const V2& v) const;
    Circle apply(const Circle& c) const;
    Rect apply(const Rect& r) const;
};
struct World {
    View_transform transform;
    float gravity;
};

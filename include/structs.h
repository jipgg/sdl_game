#pragma once
#include "common.h"
struct Vector2 {
    double x;
    double y;
    Vector2(double _x = 0, double _y = 0);
    Vector2 operator+(const Vector2& a) const;
    Vector2& operator+=(const Vector2& a);
    Vector2 operator-(const Vector2& a) const;
    Vector2& operator-=(const Vector2& a);
    Vector2 operator*(const Vector2& a)  const;
    friend Vector2 operator*(const Vector2& a, double scalar);
    friend Vector2 operator*(double scalar, const Vector2& a);
    bool operator==(const Vector2& a) const;
    Vector2 operator/(double scalar) const;
    double dot(const Vector2& a) const;
    Vector2 unit() const;
    double magnitude() const;
};
using V2 = Vector2;
struct Rect {
    double x{0};
    double y{0};
    double w{0};
    double h{0};
};
struct Color {
    uint8 r{0};
    uint8 g{0};
    uint8 b{0};
    uint8 a{0};
};
struct Circle {
    Vector2 center;
    double radius;
};
struct Physical_properties {
    bool is_welded{false};
    bool is_collidable{true};
    float elasticity{0};
    float mass{1};
};
struct View_transform {
    Vector2 translation{0, 0};
    Vector2 scaling{1, 1};
    Vector2 viewport{0, 0};
    //double rotation{0};
    Vector2 apply(const Vector2& v) const;
    Circle apply(const Circle& c) const;
    Rect apply(const Rect& r) const;
};

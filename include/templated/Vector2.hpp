#pragma once
namespace templated {
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
}

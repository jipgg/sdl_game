#include "structs.h"
// @ Vector2
Vector2::Vector2(double _x, double _y): x{_x}, y{_y} {}
Vector2 Vector2::operator+(const Vector2& a) const {
    return {x + a.x, y + a.y};
}
Vector2 Vector2::operator-(const Vector2& a) const {
    return {x - a.x, y - a.y};
}
double Vector2::dot(const Vector2& a) const {
    return x * a.x + y * a.y; 
}
Vector2 Vector2::operator*(const Vector2& a) const {
    return {x * a.x, y * a.y};
}
Vector2 operator*(const Vector2& a, double scalar) {
    return {a.x * scalar, a.y * scalar};
}
double Vector2::magnitude() const {
    return sqrt(pow(x, 2) + pow(y, 2)); 
}
Vector2& Vector2::operator+=(const Vector2& a) {
    x += a.x;
    y += a.y;
    return *this;
}
Vector2 operator*(double scalar, const Vector2& a) {
    return a * scalar;
}
Vector2 Vector2::unit() const {
    Expects(magnitude() != 0);
    return (*this) / magnitude();
}
Vector2 Vector2::operator/(double scalar) const {
    return {x / scalar, y / scalar};
}
Vector2& Vector2::operator-=(const Vector2& a) {
    this->x -= a.x;
    this->y -= a.y;
    return *this;
}
bool Vector2::operator==(const Vector2& a) const {
    return x == a.x and y == a.y;
}
Vector2 View_transform::apply(const Vector2& v) const {
    Vector2 ret{v};
    ret -= translation;
    return ret;
}
Rect View_transform::apply(const Rect& r) const {
    Rect rect{r};
    /*
    rect.x *= scaling.x;
    rect.y *= scaling.y;
    rect.w *= scaling.x;
    rect.h *= scaling.y;
    rect.x -= translation.x;
    rect.y -= translation.y;
    Vector2 viewport_center = viewport / 2.0;
    Vector2 center{rect.x + rect.w / 2, rect.y + rect.h /2};
    double centerX = rect.x + rect.w / 2.0;
    double centerY = rect.y + rect.h / 2.0;
    rect.w *= scaling.x;
    rect.h *= scaling.y;
    rect.x = center.x - rect.w / 2.0f;
    rect.y = center.y - rect.h / 2.0f;
    rect.x = (center.x - viewport_center.x) 
        * scaling.x + viewport_center.x - rect.w / 2.0;
    rect.y = (center.y - viewport_center.y)
        * scaling.y + viewport_center.y - rect.h / 2.0;
    rect.x -= translation.x;
    rect.y -= translation.y;
    return rect;
    */

    // Calculate the center of the rectangle before scaling
    Vector2 original_center{rect.x + rect.w / 2.0, rect.y + rect.h / 2.0};

    // Scale the width and height
    rect.w *= scaling.x;
    rect.h *= scaling.y;

    // Calculate the new position so that the rectangle remains centered
    rect.x = original_center.x - rect.w / 2.0;
    rect.y = original_center.y - rect.h / 2.0;

    // Calculate the viewport center
    Vector2 viewport_center = viewport / 2.0;

    // Now, adjust for the scaled position relative to the viewport center
    rect.x = (original_center.x - viewport_center.x) 
        * scaling.x + viewport_center.x - rect.w / 2.0;
    rect.y = (original_center.y - viewport_center.y) 
        * scaling.y + viewport_center.y - rect.h / 2.0;

    // Finally, apply the translation
    rect.x -= translation.x;
    rect.y -= translation.y;

    return rect;
}
Circle View_transform::apply(const Circle& c) const {
    Circle circle{c};
    circle.center -= translation;
    circle.radius *= scaling.x;
    return circle;
}

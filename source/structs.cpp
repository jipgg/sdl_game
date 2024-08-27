#include "structs.h"
V2 View_transform::apply(const V2& v) const {
    V2 ret{v};
    ret -= translation;
    return ret;
}
Rect View_transform::apply(const Rect& r) const {
    Rect rect{r};
    const V2 original_center{rect.x + rect.w / 2.f, rect.y + rect.h / 2.f};
    rect.w *= scaling.x;
    rect.h *= -scaling.y;
    rect.x = original_center.x - rect.w / 2.0f;
    rect.y = original_center.y - rect.h / 2.0f;
    const V2 viewport_center = viewport / 2.0f;
    rect.x = (original_center.x - viewport_center.x) 
        * scaling.x + viewport_center.x - rect.w / 2.f;
    rect.y = (original_center.y - viewport_center.y) 
        * -scaling.y + viewport_center.y - rect.h / 2.f;
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

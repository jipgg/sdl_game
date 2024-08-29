#pragma once
#include "common.h"
struct Rect {
    float x{0};
    float y{0};
    float w{0};
    float h{0};
};
struct Color {
    uint8_t r{0};
    uint8_t g{0};
    uint8_t b{0};
    uint8_t a{0};
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
    V2i viewport{0, 0};
    //double rotation{0};
    Rect apply(const Rect& r) const {
        Rect rect{r};
        const V2 original_center{rect.x + rect.w / 2.f, rect.y + rect.h / 2.f};
        rect.w *= scaling.x;
        rect.h *= -scaling.y;
        rect.x = original_center.x - rect.w / 2.0f;
        rect.y = original_center.y - rect.h / 2.0f;
        const V2 viewport_center = V2 {viewport.x / 2.f, viewport.y / 2.f};
        rect.x = (original_center.x - viewport_center.x) 
            * scaling.x + viewport_center.x - rect.w / 2.f;
        rect.y = (original_center.y - viewport_center.y) 
            * -scaling.y + viewport_center.y - rect.h / 2.f;
        rect.x -= translation.x;
        rect.y -= translation.y;
        return rect;
    }
};

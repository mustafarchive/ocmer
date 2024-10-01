#include <math.h>

typedef struct {
    float x;
    float y;
} Vec2f;

Vec2f vec2(float x, float y) {
    Vec2f result = {x, y};
    return result;
}

Vec2f vec2f_multiply_scalar(Vec2f a, float s) {
    return vec2(a.x * s, a.y * s);
}

Vec2f vec2f_divide_scalar(Vec2f a, float s) {
    return vec2(a.x / s, a.y / s);
}

Vec2f vec2f_multiply(Vec2f a, Vec2f b) {
    return vec2(a.x * b.x, a.y * b.y);
}

Vec2f vec2f_divide(Vec2f a, Vec2f b) {
    return vec2(a.x / b.x, a.y / b.y);
}

Vec2f vec2f_subtract(Vec2f a, Vec2f b) {
    return vec2(a.x - b.x, a.y - b.y);
}

Vec2f vec2f_add(Vec2f a, Vec2f b) {
    return vec2(a.x + b.x, a.y + b.y);
}

void vec2f_add_assign(Vec2f* a, Vec2f b) {
    a->x += b.x;
    a->y += b.y;
}

void vec2f_subtract_assign(Vec2f* a, Vec2f b) {
    a->x -= b.x;
    a->y -= b.y;
}

float vec2f_length(Vec2f a) {
    return sqrtf(a.x * a.x + a.y * a.y);
}

Vec2f vec2f_normalize(Vec2f a) {
    float b = vec2f_length(a);
    if (b == 0.0f) {
        return vec2(0.0f, 0.0f);
    } else {
        return vec2(a.x / b, a.y / b);
    }
}
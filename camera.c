#include <X11/Xlib.h>
#include <math.h>
#include "camera.c"  // Assuming this is where Config is defined
#include "config.c"  // Assuming this is where Config is defined
#include "la.c"      // Assuming this is where Vec2f is defined

#define VELOCITY_THRESHOLD 15.0

typedef struct {
    Vec2f curr;
    Vec2f prev;
    int drag;
} Mouse;

typedef struct {
    Vec2f position;
    Vec2f velocity;
    float scale;
    float deltaScale;
    Vec2f scalePivot;
} Camera;

Vec2f world(Camera* camera, Vec2f v) {
    Vec2f result;
    result.x = v.x / camera->scale;
    result.y = v.y / camera->scale;
    return result;
}

float vec2f_length(Vec2f v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

Vec2f vec2f_subtract(Vec2f a, Vec2f b) {
    Vec2f result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

Vec2f vec2f_add(Vec2f a, Vec2f b) {
    Vec2f result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

Vec2f vec2f_multiply(Vec2f v, float scalar) {
    Vec2f result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    return result;
}

void update_camera(Camera* camera, Config* config, float dt, Mouse* mouse, XImage* image, Vec2f windowSize) {
    if (fabs(camera->deltaScale) > 0.5) {
        Vec2f p0 = vec2f_subtract(camera->scalePivot, vec2f_multiply(windowSize, 0.5));
        p0 = vec2f_multiply(p0, 1.0f / camera->scale);

        camera->scale = fmaxf(camera->scale + camera->deltaScale * dt, config->min_scale);

        Vec2f p1 = vec2f_subtract(camera->scalePivot, vec2f_multiply(windowSize, 0.5));
        p1 = vec2f_multiply(p1, 1.0f / camera->scale);

        camera->position = vec2f_add(camera->position, vec2f_subtract(p0, p1));

        camera->deltaScale -= camera->deltaScale * dt * config->scale_friction;
    }

    if (!mouse->drag && (vec2f_length(camera->velocity) > VELOCITY_THRESHOLD)) {
        camera->position = vec2f_add(camera->position, vec2f_multiply(camera->velocity, dt));
        camera->velocity = vec2f_subtract(camera->velocity, 
                                          vec2f_multiply(camera->velocity, dt * config->dragFriction));
    }
}
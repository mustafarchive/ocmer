#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>
#include <X11/cursorfont.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>

// Assuming these are implemented in other files
// #include "navigation.h"
#include "screenshot.c"
#include "camera.c"
// #include "config.h"

#define true 1
#define false 0
typedef int bool;

typedef struct {
    char* path;
    char* content;
} Shader;

typedef struct {
    bool isEnabled;
    float shadow;
    float radius;
    float deltaRadius;
} Flashlight;

#define INITIAL_FL_DELTA_RADIUS 250.0
#define FL_DELTA_RADIUS_DECELERATION 10.0

void updateFlashlight(Flashlight* flashlight, float dt) {
    if (fabs(flashlight->deltaRadius) > 1.0) {
        flashlight->radius = fmax(0.0, flashlight->radius + flashlight->deltaRadius * dt);
        flashlight->deltaRadius -= flashlight->deltaRadius * FL_DELTA_RADIUS_DECELERATION * dt;
    }

    if (flashlight->isEnabled) {
        flashlight->shadow = fmin(flashlight->shadow + 6.0 * dt, 0.8);
    } else {
        flashlight->shadow = fmax(flashlight->shadow - 6.0 * dt, 0.0);
    }
}

void drawScreenshot(XImage* screenshot, Camera* camera, GLuint shader, GLuint vao, GLuint texture,
                    Vec2f windowSize, Mouse mouse, Flashlight flashlight) {
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader);

    glUniform2f(glGetUniformLocation(shader, "cameraPos"), camera->position[0], camera->position[1]);
    glUniform1f(glGetUniformLocation(shader, "cameraScale"), camera->scale);
    glUniform2f(glGetUniformLocation(shader, "screenshotSize"),
                (float)screenshot->width,
                (float)screenshot->height);
    glUniform2f(glGetUniformLocation(shader, "windowSize"),
                windowSize.x,
                windowSize.y);
    glUniform2f(glGetUniformLocation(shader, "cursorPos"),
                mouse.curr.x,
                mouse.curr.y);
    glUniform1f(glGetUniformLocation(shader, "flShadow"), flashlight.shadow);
    glUniform1f(glGetUniformLocation(shader, "flRadius"), flashlight.radius);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

Vec2f getCursorPosition(Display* display) {
    Window root, child;
    int root_x, root_y, win_x, win_y;
    unsigned int mask;
    XQueryPointer(
        display, DefaultRootWindow(display),
        &root, &child,
        &root_x, &root_y,
        &win_x, &win_y,
        &mask);
    Vec2f result = {(float)root_x, (float)root_y};
    return result;
}

Window selectWindow(Display* display) {
    Cursor cursor = XCreateFontCursor(display, XC_crosshair);
    Window root = DefaultRootWindow(display);
    XGrabPointer(display, root, False,
                 ButtonMotionMask | ButtonPressMask | ButtonReleaseMask,
                 GrabModeAsync, GrabModeAsync,
                 root, cursor,
                 CurrentTime);

    XGrabKeyboard(display, root, False,
                  GrabModeAsync, GrabModeAsync,
                  CurrentTime);

    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        switch (event.type) {
            case ButtonPress:
                XUngrabPointer(display, CurrentTime);
                XUngrabKeyboard(display, CurrentTime);
                XFreeCursor(display, cursor);
                return event.xbutton.subwindow;
            case KeyPress:
                XUngrabPointer(display, CurrentTime);
                XUngrabKeyboard(display, CurrentTime);
                XFreeCursor(display, cursor);
                return root;
        }
    }

    return root;
}

int xElevenErrorHandler(Display* display, XErrorEvent* errorEvent) {
    char errorMessage[256];
    XGetErrorText(display, errorEvent->error_code, errorMessage, sizeof(errorMessage));
    fprintf(stderr, "X ELEVEN ERROR: %s\n", errorMessage);
    return 0;
}

int main(int argc, char* argv[]) {
    // Parse command line arguments here...

    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Failed to open display\n");
        return 1;
    }

    XSetErrorHandler(xElevenErrorHandler);

    Window trackingWindow = DefaultRootWindow(display);

    // Initialize GLX context, create window, etc...

    Flashlight flashlight = {false, 0.0, 200.0, 0.0};

    while (!quitting) {
        // Event loop...

        // Update camera and flashlight
        updateCamera(&camera, &config, dt, &mouse, screenshot.image, windowSize);
        updateFlashlight(&flashlight, dt);

        // Draw
        drawScreenshot(screenshot.image, &camera, shaderProgram, vao, texture,
                       windowSize, mouse, flashlight);

        glXSwapBuffers(display, win);
        glFinish();

        // Refresh screenshot if needed
    }

    XVisualInfo* visual = glXGetVisualFromFBConfig(display, fbConfig);

    // Cleanup...

    XCloseDisplay(display);
    return 0;
}
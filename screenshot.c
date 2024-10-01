#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef USE_MITSHM
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

typedef struct {
    XImage *image;
#ifdef USE_MITSHM
    XShmSegmentInfo *shminfo;
#endif
} Screenshot;

Screenshot newScreenshot(Display *display, Window window) {
    Screenshot result;
    XWindowAttributes attributes;
    XGetWindowAttributes(display, window, &attributes);

#ifdef USE_MITSHM
    result.shminfo = (XShmSegmentInfo *)malloc(sizeof(XShmSegmentInfo));
    int screen = DefaultScreen(display);
    result.image = XShmCreateImage(
        display,
        DefaultVisual(display, screen),
        DefaultDepthOfScreen(ScreenOfDisplay(display, screen)),
        ZPixmap,
        NULL,
        result.shminfo,
        attributes.width,
        attributes.height
    );

    result.shminfo->shmid = shmget(IPC_PRIVATE,
        result.image->bytes_per_line * result.image->height,
        IPC_CREAT | 0777);

    result.shminfo->shmaddr = shmat(result.shminfo->shmid, 0, 0);
    result.image->data = result.shminfo->shmaddr;
    result.shminfo->readOnly = False;

    XShmAttach(display, result.shminfo);
    XShmGetImage(display, window, result.image, 0, 0, AllPlanes);
#else
    result.image = XGetImage(
        display, window,
        0, 0,
        attributes.width,
        attributes.height,
        AllPlanes,
        ZPixmap
    );
#endif

    return result;
}

void destroyScreenshot(Screenshot *screenshot, Display *display) {
#ifdef USE_MITSHM
    XSync(display, False);
    XShmDetach(display, screenshot->shminfo);
    XDestroyImage(screenshot->image);
    shmdt(screenshot->shminfo->shmaddr);
    shmctl(screenshot->shminfo->shmid, IPC_RMID, 0);
    free(screenshot->shminfo);
#else
    XDestroyImage(screenshot->image);
#endif
}

void refreshScreenshot(Screenshot *screenshot, Display *display, Window window) {
    XWindowAttributes attributes;
    XGetWindowAttributes(display, window, &attributes);

#ifdef USE_MITSHM
    if (XShmGetImage(display, window, screenshot->image, 0, 0, AllPlanes) == 0 ||
        attributes.width != screenshot->image->width ||
        attributes.height != screenshot->image->height) {
        destroyScreenshot(screenshot, display);
        *screenshot = newScreenshot(display, window);
    }
#else
    XImage *refreshedImage = XGetSubImage(
        display, window,
        0, 0,
        screenshot->image->width,
        screenshot->image->height,
        AllPlanes,
        ZPixmap,
        screenshot->image,
        0, 0
    );

    if (refreshedImage == NULL ||
        refreshedImage->width != attributes.width ||
        refreshedImage->height != attributes.height) {
        XImage *newImage = XGetImage(
            display, window,
            0, 0,
            attributes.width,
            attributes.height,
            AllPlanes,
            ZPixmap
        );

        if (newImage != NULL) {
            XDestroyImage(screenshot->image);
            screenshot->image = newImage;
        }
    } else {
        screenshot->image = refreshedImage;
    }
#endif
}

void saveToPPM(XImage *image, const char *filePath) {
    FILE *f = fopen(filePath, "wb");
    if (f == NULL) {
        fprintf(stderr, "Failed to open file for writing\n");
        return;
    }

    fprintf(f, "P6\n%d %d\n255\n", image->width, image->height);

    for (int i = 0; i < image->width * image->height; i++) {
        fputc(image->data[i * 4 + 2], f);
        fputc(image->data[i * 4 + 1], f);
        fputc(image->data[i * 4 + 0], f);
    }

    fclose(f);
}

int main() {
    // Capture and save a screenshot
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Failed to open display\n");
        return 1;
    }

    Window root = DefaultRootWindow(display);
    Screenshot screenshot = newScreenshot(display, root);
    saveToPPM(screenshot.image, "screenshot.ppm");
    destroyScreenshot(&screenshot, display);
}
#include "stdarg.h"
#include "ctype.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "unistd.h"
#include "errno.h"
#include "dirent.h"
#include "unistd.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "X11/Xlib.h"
#include "X11/X.h"
#include "X11/Xutil.h"
#include "X11/extensions/Xrandr.h"
#include "X11/extensions/XInput.h"
#include "X11/cursorfont.h"
#include "math.h"
#include "GL/glew.h"
#include "GL/glx.h"

/* Embed: /Users/mustafa/ocen-lang/ocen/std/./prelude.h */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>

#ifndef OC_NO_BACKTRACE
#include <execinfo.h>
#endif

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

void dump_backtrace() {
#ifndef OC_NO_BACKTRACE
  void *array[40];
  size_t size = backtrace(array, 40);
  char **strings = backtrace_symbols(array, size);
  printf("\nBacktrace:\n");
  for (size_t i = 1; i < size; i++) {
    printf("%s\n", strings[i]);
  }
  free(strings);
#endif
}

#ifdef __APPLE__
  #define oc_trap __builtin_debugtrap
#else
  #define oc_trap __builtin_trap
#endif

void ae_assert_fail(char *dbg_msg, char *msg) {
  printf("--------------------------------------------------------------------------------\n");
  printf("%s\n", dbg_msg);
  if (msg) {
    printf("  Message: %s\n", msg);
  }
  printf("--------------------------------------------------------------------------------\n");
  fflush(stdout);
  dump_backtrace();
  oc_trap();
}


/* Enums */
char *std_fs_SeekMode_dbg(i32 this) {
  switch (this) {
    case SEEK_SET: return "Set";
    case SEEK_CUR: return "Cur";
    case SEEK_END: return "End";
    default: return "<unknown>";
  }
}

char *src_x11_Format_dbg(i32 this) {
  switch (this) {
    case XYBitmap: return "XYBitmap";
    case XYPixmap: return "XYPixmap";
    case ZPixmap: return "ZPixmap";
    default: return "<unknown>";
  }
}

/* Constants */
#define std_math_TAU ((M_PI * 2.0f))
#define src_camera_VELOCITY_THRESHOLD (15.0f)
#define INITIAL_FL_DELTA_RADIUS (250.0f)
#define FL_DELTA_RADIUS_DECELERATION (10.0f)
/* Struct typedefs */
typedef struct std_buffer_Buffer std_buffer_Buffer;
typedef struct std_vec_Vec2__1 std_vec_Vec2__1;
typedef struct src_screenshot_Screenshot src_screenshot_Screenshot;
typedef struct src_camera_Mouse src_camera_Mouse;
typedef struct src_camera_Camera src_camera_Camera;
typedef struct Shader Shader;
typedef struct Flashlight Flashlight;
typedef struct src_config_Config src_config_Config;

/* Struct definitions */
struct std_buffer_Buffer {
  u8 *data;
  u32 size;
  u32 capacity;
};

struct std_vec_Vec2__1 {
  f32 x;
  f32 y;
};

struct src_screenshot_Screenshot {
  XImage *image;
};

struct src_camera_Mouse {
  std_vec_Vec2__1 curr;
  std_vec_Vec2__1 prev;
  i32 drag;
};

struct src_camera_Camera {
  std_vec_Vec2__1 position;
  std_vec_Vec2__1 velocity;
  f32 scale;
  f32 delta_scale;
  std_vec_Vec2__1 scale_pivot;
};

struct Shader {
  char *path;
  std_buffer_Buffer content;
};

struct Flashlight {
  bool enabled;
  f32 shadow;
  f32 radius;
  f32 delta_radius;
};

struct src_config_Config {
  f32 min_scale;
  f32 scroll_speed;
  f32 drag_friction;
  f32 scale_friction;
};


/* function declarations */
void std_panic(char *msg) __attribute__((noreturn));
char *std_format(char *fmt, ...);
char *std_mem_alloc__0(u32 count);
u8 *std_mem_alloc__1(u32 count);
void *std_mem_impl_my_calloc(void *state, u32 size);
void *std_mem_impl_my_realloc(void *state, void *ptr, u32 old_size, u32 size);
void std_mem_impl_my_free(void *state, void *ptr);
FILE *std_fs_File_open(char *path, char *mode);
std_buffer_Buffer std_fs_read_file(char *path);
char *std_libc_get_err(void);
std_buffer_Buffer std_buffer_Buffer_make(u32 capacity);
char *std_buffer_Buffer_str(std_buffer_Buffer this);
src_screenshot_Screenshot src_screenshot_new_screenshot(Display *display, Window window);
void src_screenshot_save_to_ppm(XImage *image, char *file_path);
XWindowAttributes src_x11_Window_get_attributes(Window this, Display *display);
void color_screenshot(XImage *img);
Shader Shader_read(char *path);
u32 new_shader(Shader shader, u32 kind);
u32 new_shader_program(Shader vert, Shader frag);
std_vec_Vec2__1 get_cursor_pos(Display *display);
i32 error_handler(Display *display, XErrorEvent *event);
i32 main(void);
void *std_mem_state_allocator = NULL;
void *(*std_mem_state_alloc_fn)(void *, u32) = std_mem_impl_my_calloc;
void *(*std_mem_state_realloc_fn)(void *, void *, u32, u32) = std_mem_impl_my_realloc;
void (*std_mem_state_free_fn)(void *, void *) = std_mem_impl_my_free;
Shader vertex_shader = {0};
Shader fragment_shader = {0};
src_config_Config src_config_default_config = (src_config_Config){.min_scale=0.01f, .scroll_speed=1.5f, .drag_friction=6.0f, .scale_friction=4.0f};
/* function implementations */

#line 12 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
void std_panic(char *msg) {

#line 13 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  printf("%s""\n", msg);

#line 14 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  dump_backtrace();

#line 15 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  oc_trap();

#line 16 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  exit(1);
}


#line 43 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
char *std_format(char *fmt, ...) {

#line 44 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"

#line 46 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  va_list args = {0};

#line 47 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  va_start(args, fmt);

#line 48 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  u32 size = vsnprintf(NULL, 0, fmt, args);

#line 49 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  va_end(args);

#line 51 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  va_start(args, fmt);

#line 52 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  char *s = std_mem_alloc__0(((u32)(size + 1)));

#line 53 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  vsnprintf(s, (size + 1), fmt, args);

#line 54 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  s[size]='\0';

#line 55 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  va_end(args);

#line 56 "/Users/mustafa/ocen-lang/ocen/std/mod.oc"
  return s;
}


#line 44 "/Users/mustafa/ocen-lang/ocen/std/mem.oc"
char *std_mem_alloc__0(u32 count) {

#line 46 "/Users/mustafa/ocen-lang/ocen/std/mem.oc"
  return ((char *)std_mem_state_alloc_fn(std_mem_state_allocator, (count * ((u32)sizeof(char)))));
}


#line 44 "/Users/mustafa/ocen-lang/ocen/std/mem.oc"
u8 *std_mem_alloc__1(u32 count) {

#line 46 "/Users/mustafa/ocen-lang/ocen/std/mem.oc"
  return ((u8 *)std_mem_state_alloc_fn(std_mem_state_allocator, (count * ((u32)sizeof(u8)))));
}


#line 16 "/Users/mustafa/ocen-lang/ocen/std/mem.oc"
void *std_mem_impl_my_calloc(void *state, u32 size) {

#line 16 "/Users/mustafa/ocen-lang/ocen/std/mem.oc"
  return calloc(size, 1);
}


#line 17 "/Users/mustafa/ocen-lang/ocen/std/mem.oc"
void *std_mem_impl_my_realloc(void *state, void *ptr, u32 old_size, u32 size) {

#line 17 "/Users/mustafa/ocen-lang/ocen/std/mem.oc"
  return realloc(ptr, size);
}


#line 18 "/Users/mustafa/ocen-lang/ocen/std/mem.oc"
void std_mem_impl_my_free(void *state, void *ptr) {

#line 18 "/Users/mustafa/ocen-lang/ocen/std/mem.oc"
  free(ptr);
}


#line 81 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
FILE *std_fs_File_open(char *path, char *mode) {

#line 82 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  FILE *file = fopen(path, mode);

#line 83 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  if (!((bool)file)) {

#line 84 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
    std_panic(std_format("failed to open file: '%s': %s", path, std_libc_get_err()));
  }

#line 86 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  return file;
}


#line 169 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
std_buffer_Buffer std_fs_read_file(char *path) {

#line 170 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  FILE *file = fopen(path, "r");

#line 171 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  if (!((bool)file)) {

#line 171 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
    std_panic(std_format("[-] Failed to open file: %s: %s", path, std_libc_get_err()));
  }

#line 172 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  fseek(file, ((i64)0), SEEK_END);

#line 173 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  u32 size = ((u32)ftell(file));

#line 174 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  fseek(file, ((i64)0), SEEK_SET);

#line 175 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  std_buffer_Buffer data = std_buffer_Buffer_make((size + 1));

#line 176 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  i32 read = fread(data.data, 1, size, file);

#line 177 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  if (((u32)read) != size) {

#line 178 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
    std_panic(std_format("[-] Failed to read from file: %s", path));
  }

#line 180 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  fclose(file);

#line 181 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  data.size=((u32)size);

#line 182 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  data.data[size]=((u8)0);

#line 183 "/Users/mustafa/ocen-lang/ocen/std/fs.oc"
  return data;
}


#line 72 "/Users/mustafa/ocen-lang/ocen/std/libc.oc"
char *std_libc_get_err(void) {

#line 72 "/Users/mustafa/ocen-lang/ocen/std/libc.oc"
  return ((char *)strerror(errno));
}


#line 20 "/Users/mustafa/ocen-lang/ocen/std/buffer.oc"
std_buffer_Buffer std_buffer_Buffer_make(u32 capacity) {

#line 21 "/Users/mustafa/ocen-lang/ocen/std/buffer.oc"
  return (std_buffer_Buffer){.data=std_mem_alloc__1(capacity), .size=0, .capacity=capacity};
}


#line 186 "/Users/mustafa/ocen-lang/ocen/std/buffer.oc"
char *std_buffer_Buffer_str(std_buffer_Buffer this) {

#line 186 "/Users/mustafa/ocen-lang/ocen/std/buffer.oc"
  return ((char *)this.data);
}


#line 8 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
src_screenshot_Screenshot src_screenshot_new_screenshot(Display *display, Window window) {

#line 12 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
  src_screenshot_Screenshot result = {0};

#line 13 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
  XWindowAttributes attributes = src_x11_Window_get_attributes(window, display);

#line 14 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
  result.image=XGetImage(display, window, 0, 0, attributes.width, attributes.height, 0xffffffff, ZPixmap);

#line 21 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
  return result;
}


#line 45 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
void src_screenshot_save_to_ppm(XImage *image, char *file_path) {

#line 49 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
  FILE *f = std_fs_File_open(file_path, "wb");

#line 50 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
  if (f==NULL) {

#line 51 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
    printf("Failed to open file for writing\n""\n");

#line 52 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
    return;
  }

#line 55 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
  fprintf(f, "P6\n%d %d\n255\n", image->width, image->height);

#line 57 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
  for (i32 i = 0; i < (image->width * image->height); i++) {

#line 58 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
    fputc(image->data[((i * 4) + 2)], f);

#line 59 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
    fputc(image->data[((i * 4) + 1)], f);

#line 60 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
    fputc(image->data[((i * 4) + 0)], f);
  }

#line 62 "/Users/mustafa/dev/ocmer/src/screenshot.oc"
  fclose(f);
}


#line 79 "/Users/mustafa/dev/ocmer/src/x11.oc"
XWindowAttributes src_x11_Window_get_attributes(Window this, Display *display) {

#line 80 "/Users/mustafa/dev/ocmer/src/x11.oc"
  XWindowAttributes attributes = {0};

#line 81 "/Users/mustafa/dev/ocmer/src/x11.oc"
  XGetWindowAttributes(display, this, &attributes);

#line 82 "/Users/mustafa/dev/ocmer/src/x11.oc"
  return attributes;
}


#line 11 "src/main.oc"
void color_screenshot(XImage *img) {

#line 12 "src/main.oc"
  for (i32 i = 0; i < img->height; i++) {

#line 13 "src/main.oc"
    for (i32 j = 0; j < img->width; j++) {

#line 14 "src/main.oc"
      u8 u = ((u8)((((f32)i) / ((f32)img->height)) * 255.0f));

#line 15 "src/main.oc"
      u8 v = ((u8)((((f32)j) / ((f32)img->width)) * 255.0f));

#line 17 "src/main.oc"
      i32 off = (((i * img->width) + j) * 4);

#line 18 "src/main.oc"
      img->data[(off + 0)]=((u8)0);

#line 19 "src/main.oc"
      img->data[(off + 1)]=u;

#line 20 "src/main.oc"
      img->data[(off + 2)]=v;
    }
  }
}


#line 30 "src/main.oc"
Shader Shader_read(char *path) {

#line 30 "src/main.oc"
  return (Shader){.path=path, .content=std_fs_read_file(path)};
}


#line 61 "src/main.oc"
u32 new_shader(Shader shader, u32 kind) {

#line 62 "src/main.oc"
  u32 result = glCreateShader(kind);

#line 63 "src/main.oc"
  glShaderSource(result, 1, ((void *)std_buffer_Buffer_str(shader.content)), NULL);

#line 64 "src/main.oc"
  glCompileShader(result);

#line 66 "src/main.oc"
  i32 success;

#line 67 "src/main.oc"
  glGetShaderiv(result, GL_COMPILE_STATUS, &success);

#line 68 "src/main.oc"
  if (success==0) {

#line 69 "src/main.oc"
    char info[512] = {0};

#line 70 "src/main.oc"
    glGetShaderInfoLog(result, 512, NULL, info);

#line 71 "src/main.oc"
    printf("ERROR: Shader compilation failed: %s\n", info);

#line 72 "src/main.oc"
    exit(1);
  }

#line 74 "src/main.oc"
  return result;
}


#line 77 "src/main.oc"
u32 new_shader_program(Shader vert, Shader frag) {

#line 78 "src/main.oc"
  printf("---- new_shader_program""\n");

#line 79 "src/main.oc"
  u32 result = glCreateProgram();

#line 80 "src/main.oc"
  printf("---- << new_shader_program""\n");

#line 81 "src/main.oc"
  u32 vert_id = new_shader(vert, GL_VERTEX_SHADER);

#line 82 "src/main.oc"
  u32 frag_id = new_shader(frag, GL_FRAGMENT_SHADER);

#line 84 "src/main.oc"
  glAttachShader(result, vert_id);

#line 85 "src/main.oc"
  glAttachShader(result, frag_id);

#line 86 "src/main.oc"
  glLinkProgram(result);

#line 87 "src/main.oc"
  glDeleteShader(vert_id);

#line 88 "src/main.oc"
  glDeleteShader(frag_id);

#line 90 "src/main.oc"
  i32 success;

#line 91 "src/main.oc"
  glGetProgramiv(result, GL_LINK_STATUS, &success);

#line 92 "src/main.oc"
  if (success==0) {

#line 93 "src/main.oc"
    char info[512] = {0};

#line 94 "src/main.oc"
    glGetProgramInfoLog(result, 512, NULL, info);

#line 95 "src/main.oc"
    printf("ERROR: Shader program linking failed: %s\n", info);

#line 96 "src/main.oc"
    exit(1);
  }

#line 99 "src/main.oc"
  glUseProgram(result);

#line 100 "src/main.oc"
  return result;
}


#line 126 "src/main.oc"
std_vec_Vec2__1 get_cursor_pos(Display *display) {

#line 127 "src/main.oc"
  Window root = {0};

#line 128 "src/main.oc"
  Window child = {0};

#line 129 "src/main.oc"
  i32 root_x;

#line 130 "src/main.oc"
  i32 root_y;

#line 131 "src/main.oc"
  i32 win_x;

#line 132 "src/main.oc"
  i32 win_y;

#line 133 "src/main.oc"
  u32 mask;

#line 134 "src/main.oc"
  XQueryPointer(display, DefaultRootWindow(display), &root, &child, &root_x, &root_y, &win_x, &win_y, &mask);

#line 142 "src/main.oc"
  return (std_vec_Vec2__1){.x=((f32)root_x), .y=((f32)root_y)};
}


#line 178 "src/main.oc"
i32 error_handler(Display *display, XErrorEvent *event) {

#line 179 "src/main.oc"
  char buf[1024] = {0};

#line 180 "src/main.oc"
  XGetErrorText(display, event->error_code, buf, 1024);

#line 181 "src/main.oc"
  printf("X11 error: %s\n", buf);

#line 182 "src/main.oc"
  return 0;
}


#line 185 "src/main.oc"
i32 main(void) {

#line 186 "src/main.oc"
  vertex_shader=Shader_read("src/vert.glsl");

#line 187 "src/main.oc"
  fragment_shader=Shader_read("src/frag.glsl");

#line 189 "src/main.oc"
  char *boomer_dir = "./";

#line 190 "src/main.oc"
  char *config_file = "./config.cfg";

#line 191 "src/main.oc"
  bool windowed = false;

#line 192 "src/main.oc"
  f32 delay = 0.0f;

#line 194 "src/main.oc"
  src_config_Config config = src_config_default_config;

#line 196 "src/main.oc"
  Display *display = XOpenDisplay(NULL);

#line 197 "src/main.oc"
  if (!((bool)display)) {

#line 198 "src/main.oc"
    printf("Failed to open display\n""\n");

#line 199 "src/main.oc"
    return 1;
  }

#line 201 "src/main.oc"
  Window window = DefaultRootWindow(display);

#line 202 "src/main.oc"
  XSetErrorHandler(error_handler);

#line 204 "src/main.oc"
  Window tracking_window = DefaultRootWindow(display);

#line 206 "src/main.oc"
  XRRScreenConfiguration *screen_config = XRRGetScreenInfo(display, DefaultRootWindow(display));

#line 207 "src/main.oc"
  i32 rate = XRRConfigCurrentRate(screen_config);

#line 208 "src/main.oc"
  printf("Current refresh rate: %d Hz\n\n", rate);

#line 210 "src/main.oc"
  i32 screen = XDefaultScreen(display);

#line 211 "src/main.oc"
  i32 glxMajor;

#line 212 "src/main.oc"
  i32 glxMinor;

#line 214 "src/main.oc"
  if ((glXQueryVersion(display, &glxMajor, &glxMinor)==0 || (glxMajor==1 && (glxMinor < 3))) || (glxMajor < 1)) {

#line 219 "src/main.oc"
    printf("GLX 1.3 or greater is required\n""\n");

#line 220 "src/main.oc"
    return 1;
  }

#line 223 "src/main.oc"
  printf("GLX version: %d.%d\n\n", glxMajor, glxMinor);

#line 224 "src/main.oc"
  printf("GLX extensions: %s\n\n", glXQueryExtensionsString(display, screen));

#line 226 "src/main.oc"
  i32 attrs[5] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, 0};

#line 232 "src/main.oc"
  XVisualInfo *vi = glXChooseVisual(display, screen, attrs);

#line 233 "src/main.oc"
  if (!((bool)vi)) {

#line 234 "src/main.oc"
    printf("Failed to choose visual\n""\n");

#line 235 "src/main.oc"
    return 1;
  }

#line 238 "src/main.oc"
  XSetWindowAttributes swa = {0};

#line 239 "src/main.oc"
  swa.colormap=XCreateColormap(display, DefaultRootWindow(display), vi->visual, 0);

#line 240 "src/main.oc"
  swa.event_mask=((((((ButtonPressMask | ButtonReleaseMask) | KeyPressMask) | KeyReleaseMask) | PointerMotionMask) | ExposureMask) | ClientMessage);

#line 243 "src/main.oc"
  swa.override_redirect=1;

#line 244 "src/main.oc"
  swa.save_under=1;

#line 246 "src/main.oc"
  XWindowAttributes attributes = {0};

#line 247 "src/main.oc"
  XGetWindowAttributes(display, window, &attributes);

#line 248 "src/main.oc"
  Window win = XCreateWindow(display, window, 0, 0, attributes.width, attributes.height, 0, vi->depth, InputOutput, vi->visual, (((CWColormap | CWEventMask) | CWOverrideRedirect) | CWSaveUnder), &swa);

#line 255 "src/main.oc"
  XMapWindow(display, win);

#line 257 "src/main.oc"
  char *wm_name = "boomer";

#line 258 "src/main.oc"
  char *wm_class = "Boomer";

#line 259 "src/main.oc"
  XClassHint hints = (XClassHint){.res_name=wm_name, .res_class=wm_class};

#line 260 "src/main.oc"
  XStoreName(display, win, wm_name);

#line 261 "src/main.oc"
  XSetClassHint(display, win, &hints);

#line 263 "src/main.oc"
  Atom wm_del_msg = XInternAtom(display, "WM_DELETE_WINDOW", 0);

#line 264 "src/main.oc"
  XSetWMProtocols(display, win, &wm_del_msg, 1);

#line 266 "src/main.oc"
  GLXContext glc = glXCreateContext(display, vi, 0, 1);

#line 267 "src/main.oc"
  glXMakeCurrent(display, win, glc);

#line 271 "src/main.oc"
  printf("---- before""\n");

#line 272 "src/main.oc"
  u32 shader_prog = new_shader_program(vertex_shader, fragment_shader);

#line 273 "src/main.oc"
  src_screenshot_Screenshot screenshot = src_screenshot_new_screenshot(display, tracking_window);

#line 274 "src/main.oc"
  printf("---- after""\n");

#line 276 "src/main.oc"
  f32 w = ((f32)screenshot.image->width);

#line 277 "src/main.oc"
  f32 h = ((f32)screenshot.image->height);

#line 289 "src/main.oc"
  u32 vao;

#line 290 "src/main.oc"
  u32 vbo;

#line 291 "src/main.oc"
  u32 ebo;

#line 292 "src/main.oc"
  f32 vertices[4][5] = {{w, 0.0f, 0.0f, 1.0f, 1.0f}, {w, h, 0.0f, 1.0f, 0.0f}, {0.0f, h, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 1.0f}};

#line 298 "src/main.oc"
  u32 indices[6] = {0, 1, 3, 1, 2, 3};

#line 300 "src/main.oc"
  glGenVertexArrays(1, &vao);

#line 301 "src/main.oc"
  glGenBuffers(1, &vbo);

#line 302 "src/main.oc"
  glGenBuffers(1, &ebo);

#line 303 "src/main.oc"

#line 309 "src/main.oc"
  glBindVertexArray(vao);

#line 310 "src/main.oc"
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

#line 311 "src/main.oc"
  glBufferData(GL_ARRAY_BUFFER, ((4 * 5) * 4), ((u8 *)vertices), GL_STATIC_DRAW);

#line 313 "src/main.oc"
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

#line 314 "src/main.oc"
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (6 * 4), ((u8 *)indices), GL_STATIC_DRAW);

#line 316 "src/main.oc"
  i32 stride = (5 * 4);

#line 318 "src/main.oc"
  glVertexAttribPointer(0, 3, GL_FLOAT, 0, stride, NULL);

#line 319 "src/main.oc"
  glEnableVertexAttribArray(0);

#line 321 "src/main.oc"
  glVertexAttribPointer(1, 2, GL_FLOAT, 0, stride, ((u8 *)(3 * 4)));

#line 322 "src/main.oc"
  glEnableVertexAttribArray(1);

#line 324 "src/main.oc"
  u32 texture = 0;

#line 325 "src/main.oc"
  glGenTextures(1, &texture);

#line 326 "src/main.oc"
  glActiveTexture(GL_TEXTURE0);

#line 327 "src/main.oc"
  glBindTexture(GL_TEXTURE_2D, texture);

#line 329 "src/main.oc"
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenshot.image->width, screenshot.image->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, ((u8 *)screenshot.image->data));

#line 333 "src/main.oc"
  glGenerateMipmap(GL_TEXTURE_2D);

#line 335 "src/main.oc"
  glUniform1i(glGetUniformLocation(shader_prog, "tex"), 0);

#line 336 "src/main.oc"
  glEnable(GL_TEXTURE_2D);

#line 338 "src/main.oc"
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

#line 339 "src/main.oc"
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

#line 340 "src/main.oc"
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

#line 341 "src/main.oc"
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

#line 343 "src/main.oc"
  bool quitting = false;

#line 344 "src/main.oc"
  src_camera_Camera camera = {0};

#line 345 "src/main.oc"
  camera.scale=1.0f;

#line 347 "src/main.oc"
  src_camera_Mouse mouse = {0};

#line 348 "src/main.oc"
  std_vec_Vec2__1 pos = get_cursor_pos(display);

#line 349 "src/main.oc"
  mouse.curr=pos;

#line 350 "src/main.oc"
  mouse.prev=pos;

#line 352 "src/main.oc"
  Flashlight flashlight = {0};

#line 353 "src/main.oc"
  flashlight.enabled=false;

#line 354 "src/main.oc"
  flashlight.radius=0.0f;

#line 356 "src/main.oc"
  f32 dt = (1.0f / ((f32)rate));

#line 357 "src/main.oc"
  Window origin_window = {0};

#line 358 "src/main.oc"
  i32 revert_to_return;

#line 359 "src/main.oc"
  XGetInputFocus(display, &origin_window, &revert_to_return);

#line 361 "src/main.oc"
  while (!quitting) {

#line 362 "src/main.oc"
    if (!windowed) {

#line 363 "src/main.oc"
      XSetInputFocus(display, tracking_window, RevertToParent, CurrentTime);
    }

#line 366 "src/main.oc"
    XWindowAttributes wa = {0};

#line 367 "src/main.oc"
    XGetWindowAttributes(display, tracking_window, &wa);

#line 368 "src/main.oc"
    glViewport(0, 0, wa.width, wa.height);
  }

#line 374 "src/main.oc"
  src_screenshot_Screenshot s = src_screenshot_new_screenshot(display, window);

#line 375 "src/main.oc"
  color_screenshot(s.image);

#line 376 "src/main.oc"
  src_screenshot_save_to_ppm(s.image, "screenshot.ppm");

#line 377 "src/main.oc"
  printf("Screenshot saved to screenshot.ppm\n""\n");
  /* defers */

#line 303 "src/main.oc"
  {

#line 304 "src/main.oc"
    glDeleteVertexArrays(1, &vao);

#line 305 "src/main.oc"
    glDeleteBuffers(1, &vbo);

#line 306 "src/main.oc"
    glDeleteBuffers(1, &ebo);
  }
}


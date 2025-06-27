#pragma once
#include <graphics.h>
#include <iostream>
#include "camera.h"
#include <thread>

#pragma comment(lib, "WINMM.lib")
#pragma comment(lib, "MSIMG32.LIB")

struct Rect {
    int x, y;
    int w, h;
};

inline void putimage_ex(IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr) {
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(GetImageHDC(GetWorkingImage()), rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h,
        GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,
        rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), blend_func);
}

inline void putimage_ex(const Camera& camera, IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    const Vector2& pos_camera = camera.get_position();
    AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(rect_dst->x - pos_camera.x), (int)(rect_dst->y - pos_camera.y),
        rect_dst->w, rect_dst->h, GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,
        rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), blend_func);
}

inline void putimage_alpha(int dst_x, int dst_y, IMAGE* img) {
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void putimage_alpha(int dst_x, int dst_y, int width, int height, IMAGE* img, int src_x, int src_y) {
    int w = width > 0 ? width : img->getwidth();
    int h = height > 0 ? height : img->getheight();
    AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h, GetImageHDC(img), src_x, src_y, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void putimage_alpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img) {
    if (img == nullptr) {
        std::cout << "null" << std::endl;
    }
    else {
        int w = img->getwidth();
        int h = img->getheight();
        const Vector2& pos_camera = camera.get_position();
        AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(dst_x - pos_camera.x), (int)(dst_y - pos_camera.y), w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
    }
}

inline void line(const Camera& camera, int x1, int y1, int x2, int y2) {
    const Vector2& pos_camera = camera.get_position();
    line((int)(x1 - pos_camera.x), (int)(y1 - pos_camera.y), (int)(x2 - pos_camera.x), (int)(y2 - pos_camera.y));
}

inline void flip_image(IMAGE* src, IMAGE* dst) {
    int w = src->getwidth();
    int h = src->getheight();
    Resize(dst, w, h);
    DWORD* src_buffer = GetImageBuffer(src);
    DWORD* dst_buffer = GetImageBuffer(dst);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int idx_src = y * w + x;
            int idx_dst = y * w + (w - x - 1);
            dst_buffer[idx_dst] = src_buffer[idx_src];
        }
    }
}

inline void sketch_image(IMAGE* src, IMAGE* dst) {
    int w = src->getwidth();
    int h = src->getheight();
    Resize(dst, w, h);
    DWORD* src_buffer = GetImageBuffer(src);
    DWORD* dst_buffer = GetImageBuffer(dst);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int idx = y * w + x;
            dst_buffer[idx] = BGR(RGB(255, 255, 255)) | (src_buffer[idx] & 0xFF000000);
        }
    }
}

inline void load_audio(LPCTSTR path, LPCTSTR id);
inline void play_audio(LPCTSTR id, bool is_loop);
inline void stop_audio(LPCTSTR id);

inline int range_random(int min_num, int max_num) {
    return min_num + rand() % (max_num - min_num + 1);
}

template<typename T>
T min_ex(std::initializer_list<T> values) {
    T min_val = *values.begin();
    for (const T& v : values) {
        if (v < min_val) {
            min_val = v;
        }
    }
    return min_val;
}
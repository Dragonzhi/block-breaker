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

// 异步播放音频的函数声明
void async_play_audio(LPCTSTR id, bool is_loop = false);

inline void load_audio(LPCTSTR path, LPCTSTR id)
{
    TCHAR str_cmd[512];
    // 确保路径用引号包裹，处理空格问题
    _stprintf_s(str_cmd, _T("open \"%s\" alias %s"), path, id);
    MCIERROR error = mciSendString(str_cmd, NULL, 0, NULL);

    if (error != 0) {
        // 更健壮的错误处理
        TCHAR errMsg[256] = { 0 };
        if (mciGetErrorString(error, errMsg, 256)) {
            std::wcerr << L"加载音频失败: " << id
                << L" | 错误: " << errMsg << std::endl;
        }
        else {
            // 直接输出错误代码
            std::wcerr << L"加载音频失败: " << id
                << L" | 未知错误代码: " << error << std::endl;
        }

        // 添加详细调试信息
        std::wcerr << L"尝试打开: " << path << std::endl;
    }
}
inline void play_audio(LPCTSTR id, bool is_loop = false)
{
    async_play_audio(id, is_loop);
    //std::thread t(async_play_audio, id, is_loop);
    //t.detach(); // 分离线程，让其在后台独立运行
}

inline void stop_audio(LPCTSTR id)
{
    static TCHAR str_cmd[512];
    _stprintf_s(str_cmd, _T("stop %s"), id);
    MCIERROR error = mciSendString(str_cmd, NULL, 0, NULL);
    if (error != 0) {
        std::cerr << "Failed to stop audio. Error code: " << error << std::endl;
    }
}


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
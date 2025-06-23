#include "character_manager.h"
#include "collision_manager.h"
#include "resources_manager.h"
#include "util.h"

#include <chrono>
#include <graphics.h>
#include <iostream>
#include <thread>

using namespace std;

static void draw_background()
{
    static IMAGE* img_background = ResourcesManager::instance()->find_image("background");
    static Rect rect_dst =
    {
        (getwidth() - img_background->getwidth()) / 2,
        (getheight() - img_background->getheight()) / 2,
        img_background->getwidth(),
        img_background->getheight()
    };
    putimage_ex(img_background, &rect_dst);
}


const int FPS = 60;  // 帧率值


int main(int argc, char** argv)
{
    using namespace std::chrono;

    HWND hwnd = initgraph(1280, 720, EW_SHOWCONSOLE);
    SetWindowText(hwnd, _T("Hollow Katana"));

    try
    {
        ResourcesManager::instance()->load();
    }
    catch (const LPCTSTR id)
    {
        TCHAR err_msg[512];
        _stprintf_s(err_msg, _T("无法加载：%s"), id);
        MessageBox(hwnd, err_msg, _T("资源加载失败"), MB_OK | MB_ICONERROR);
        return -1;
    }

    const nanoseconds frame_duration(static_cast<long long>(1e9 / FPS));
    steady_clock::time_point last_tick = steady_clock::now();

    ExMessage msg;
    bool is_quit = false;

    BeginBatchDraw();
    while (!is_quit)
    {
        while (peekmessage(&msg))
        {
            BrickManager::instance()->on_input(msg);
        }

        steady_clock::time_point frame_start = steady_clock::now();
        duration<float> delta = duration<float>(frame_start - last_tick);

        // 处理更新
        BrickManager::instance()->on_update(delta.count());
        CollisionManager::instance()->process_collide();

        setbkcolor(RGB(0, 0, 0));
        cleardevice();

        // 处理绘图
        draw_background();
        BrickManager::instance()->on_render();
        CollisionManager::instance()->on_debug_render();

        FlushBatchDraw();

        last_tick = frame_start;
        nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
        if (sleep_duration > nanoseconds(0))
            std::this_thread::sleep_for(sleep_duration);
    }
    EndBatchDraw();

    return 0;
}
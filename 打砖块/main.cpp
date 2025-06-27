#include "resources_manager.h"
#include "scene_manager.h"
#include "util.h"

#include <chrono>
#include <graphics.h>
#include <iostream>
#include <thread>

int WINDOWS_WIDTH = 1280;
int WINDOWS_HEIGHT = 720;

using namespace std;

const int FPS = 144;  // 帧率值

Camera main_camera;

int main(int argc, char** argv)
{
    using namespace std::chrono;

    HWND hwnd = initgraph(WINDOWS_WIDTH, WINDOWS_HEIGHT, EW_SHOWCONSOLE);
    SetWindowText(hwnd, _T("打砖块"));

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

    nanoseconds frame_duration(static_cast<long long>(1e9 / FPS));
    steady_clock::time_point last_tick = steady_clock::now();

    ExMessage msg;
    bool is_quit = false;

    AddFontResourceEx(_T("resources/Kenney Future.ttf"), FR_PRIVATE, NULL);
    settextstyle(28, 0, _T("Kenney Future"));
    setbkmode(TRANSPARENT);

    BeginBatchDraw();

    SceneManager::instance()->set_current_scene(SceneManager::SceneType::Menu);

    while (!is_quit)
    {
        while (peekmessage(&msg))
        {
            SceneManager::instance()->on_input(msg);
        }

        steady_clock::time_point frame_start = steady_clock::now();
        duration<float> delta = duration<float>(frame_start - last_tick);

        // 处理更新
        SceneManager::instance()->on_update(delta.count());

        setbkcolor(RGB(0, 0, 0));
        cleardevice();

        // 处理绘图
        SceneManager::instance()->on_render(main_camera);

        FlushBatchDraw();

        last_tick = frame_start;

        nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
        if (sleep_duration > nanoseconds(0))
            std::this_thread::sleep_for(sleep_duration);
        
    }
    EndBatchDraw();

    return 0;
}
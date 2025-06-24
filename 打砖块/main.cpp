#include "resources_manager.h"
#include "scene_manager.h"
#include "util.h"

#include <chrono>
#include <graphics.h>
#include <iostream>
#include <thread>


using namespace std;

const int FPS = 144;  // ֡��ֵ

int main(int argc, char** argv)
{
    using namespace std::chrono;

    HWND hwnd = initgraph(1280, 720, EW_SHOWCONSOLE);
    SetWindowText(hwnd, _T("��ש��"));

    try
    {
        ResourcesManager::instance()->load();
    }
    catch (const LPCTSTR id)
    {
        TCHAR err_msg[512];
        _stprintf_s(err_msg, _T("�޷����أ�%s"), id);
        MessageBox(hwnd, err_msg, _T("��Դ����ʧ��"), MB_OK | MB_ICONERROR);
        return -1;
    }
    steady_clock::time_point last_tick;
    nanoseconds frame_duration;

    if (FPS != 1) {
        nanoseconds frame_duration(static_cast<long long>(1e9 / FPS));
        steady_clock::time_point last_tick = steady_clock::now();
    }

    ExMessage msg;
    bool is_quit = false;

    BeginBatchDraw();

    SceneManager::instance()->set_current_scene(SceneManager::SceneType::Game);

    while (!is_quit)
    {
        while (peekmessage(&msg))
        {
            SceneManager::instance()->on_input(msg);
        }

        steady_clock::time_point frame_start = steady_clock::now();
        duration<float> delta = duration<float>(frame_start - last_tick);

        // �������
        SceneManager::instance()->on_update(delta.count());

        setbkcolor(RGB(0, 0, 0));
        cleardevice();

        // �����ͼ
        SceneManager::instance()->on_render();

        FlushBatchDraw();

        last_tick = frame_start;

        if (FPS != 1) {
            nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
            if (sleep_duration > nanoseconds(0))
                std::this_thread::sleep_for(sleep_duration);
        }

    }
    EndBatchDraw();

    return 0;
}
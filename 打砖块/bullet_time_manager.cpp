#include "bullet_time_manager.h"

#include <graphics.h>

BulletTimeManager* BulletTimeManager::manager = nullptr;

BulletTimeManager* BulletTimeManager::instance()
{
    if (!manager)
        manager = new BulletTimeManager();
    return manager;
}

void BulletTimeManager::post_process()
{
    DWORD* buffer = GetImageBuffer();
    int w = getwidth(), h = getheight();
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int idx = y * w + x;
            DWORD color = buffer[idx];
            BYTE r = (BYTE)(GetBValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            BYTE g = (BYTE)(GetGValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            BYTE b = (BYTE)(GetRValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            buffer[idx] = BGR(RGB(r, g, b)) | ((((DWORD)(BYTE)(255)) << 24));
        }
    }
}

void BulletTimeManager::set_status(Status status)
{
    this->status = status;
}

float BulletTimeManager::on_update(float delta)
{
    float delta_progress = SPEED_PROGRESS * delta;
    progress += delta_progress * (status == Status::Entering ? 1 : -1);

    if (progress < 0) progress = 0;
    if (progress > 1.0f) progress = 1.0f;

    return delta * lerp(1.0f, DST_DELTA_FACTOR, progress);
}

BulletTimeManager::BulletTimeManager() = default;

BulletTimeManager::~BulletTimeManager() = default;
#pragma once
class BulletTimeManager
{
public:
    enum class Status
    {
        Entering,
        Exiting
    };

public:
    static BulletTimeManager* instance();

    void post_process();
    void set_status(Status status);

    float on_update(float delta);

private:
    static BulletTimeManager* manager;

private:
    float progress = 0;
    Status status = Status::Exiting;
    const float SPEED_PROGRESS = 2.0f;
    const float DST_DELTA_FACTOR = 0.35f;
    const float DST_COLOR_FACTOR = 0.35f;
private:
    BulletTimeManager();
    ~BulletTimeManager();
    float lerp(float start, float end, float t) {
        return (1 - t) * start + t * end;
    }
};
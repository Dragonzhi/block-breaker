// particle.cpp
#include "particle.h"
#include <graphics.h>
#include <random>

Particle::Particle(const Vector2& position, const Vector2& velocity, float life_time, int r, int g, int b, int alpha, bool is_blink)
    : life_time(life_time), current_life(0.0f), is_active(true) {
    this->position = position;
    this->velocity = velocity;
    // 随机生成颜色
    std::random_device rd;
    std::mt19937 gen(rd());
    //std::uniform_int_distribution<> colorDist(0, 255);
    //r = colorDist(gen);
    //g = colorDist(gen);
    //b = colorDist(gen);
    this->r = r;
    this->g = g;
    this->b = b;
    this->alpha = alpha;
    this->is_blink = is_blink;
    // 随机生成大小
    std::uniform_int_distribution<> sizeDist(1, 3);
    size = sizeDist(gen);
    alpha = 255;
}

Particle::~Particle() {}

void Particle::on_update(float delta) {
    current_life += delta;
    position += velocity * delta;

    // 透明度随时间变化
    alpha = static_cast<int>(255 * (1 - current_life / life_time));
    if (current_life >= life_time) {
        is_active = false;
    }
}

void Particle::on_render(const Camera& camera) {
    if (is_active) {
        Vector2 screen_pos = position - camera.get_position();
        COLORREF color = RGB(r, g, b);
        if(is_blink)
            color = (color & 0x00FFFFFF) | (alpha << 24);
        setfillcolor(color);
        solidcircle((int)screen_pos.x, (int)screen_pos.y, size);
    }
}

void Particle::on_input(const ExMessage& msg) {}

bool Particle::is_alive() const {
    return is_active;
}
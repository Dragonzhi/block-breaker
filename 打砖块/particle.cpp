#include "particle.h"
#include <graphics.h>
#include <random>

// ��̬��������棨�����ظ�������
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> sizeDist(1, 3);

Particle::Particle() {
    reset(Vector2(0, 0), Vector2(0, 0), 0.0f, 0, 0, 0, 0, false);
}

Particle::Particle(const Vector2& position, const Vector2& velocity, float life_time, int r, int g, int b, int alpha, bool is_blink)
    : life_time(life_time), current_life(0.0f), is_active(true) {
    reset(position, velocity, life_time, r, g, b, alpha, is_blink);
}

void Particle::reset(const Vector2& pos, const Vector2& vel, float life,
    int red, int green, int blue, int alpha_val, bool blink) {
    position = pos;
    velocity = vel;
    life_time = life;
    current_life = 0.0f;
    is_active = true;
    r = red;
    g = green;
    b = blue;
    alpha = alpha_val;
    is_blink = blink;
    size = sizeDist(gen);  // ʹ�þ�̬����
}

Particle::~Particle() {}

void Particle::on_update(float delta) {
    current_life += delta;
    position += velocity * delta;

    // ͸������ʱ��仯
    alpha = static_cast<int>(255 * (1 - current_life / life_time));
    if (current_life >= life_time) {
        is_active = false;
    }
}

void Particle::on_render(const Camera& camera) {
    if (is_active) {
        Vector2 screen_pos = position - camera.get_position();

        // ʹ�ø���Ч�Ļ�ͼ��ʽ
        DWORD final_color = is_blink
            ? RGB(r/3, g/3, b/3)  // ����Ч��
            : RGB(r, g, b);

        // ʹ��putpixel����С���Ӹ���
        if (size == 1) {
            putpixel(screen_pos.x, screen_pos.y, final_color);
        }
        // ���ƴ�����
        else {
            setfillcolor(final_color);
            solidcircle(screen_pos.x, screen_pos.y, size);
        }
    }
}

void Particle::on_input(const ExMessage& msg) {}

bool Particle::is_alive() const {
    return is_active;
}
// particle.h
#pragma once

#include "game_object.h"
#include <vector>
#include "vector2.h"

class Particle : public GameObject {
public:
    Particle();
    Particle(const Vector2& position, const Vector2& velocity, float life_time, int r = 255, int g = 255, int b = 255, int alpha = 255, bool is_blink = false);
    ~Particle();

    void on_update(float delta) override;
    void on_render(const Camera& camera) override;
    void on_input(const ExMessage& msg) override;

    bool is_alive() const;

    void set_color(int r, int g, int b, int a) {
        this->r = r;
        this->g = g;
        this->b = b;
        alpha = a;
    }

    void set_blink(bool flag) {
        is_blink = flag;
    }

    void reset(const Vector2& pos, const Vector2& vel, float life,
        int r, int g, int b, int a, bool blink);

private:
    float life_time;
    float current_life;
    bool is_active; // �����Ƿ���
    int r, g, b;    // ������ɫ
    int size;       // ���Ӵ�С
    int alpha;      // ����͸����
    bool is_blink = true;

};
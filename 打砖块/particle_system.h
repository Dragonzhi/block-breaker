// particle_system.h
#pragma once

#include "particle.h"
#include "vector2.h"
#include <vector>

class ParticleSystem {
public:
    static ParticleSystem* instance();
    ParticleSystem();
    ~ParticleSystem();

    void add_particle(const Vector2& position, const Vector2& velocity, float life_time, int r, int g, int b, int a, bool is_blink);
    void on_update(float delta);
    void on_render(const Camera& camera);

private:
    static ParticleSystem* manager;
    std::vector<Particle> particles;
};
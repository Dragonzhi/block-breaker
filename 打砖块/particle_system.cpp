// particle_system.cpp
#include "particle_system.h"

ParticleSystem* ParticleSystem::manager = nullptr;

ParticleSystem* ParticleSystem::instance() {
    if (!manager)
        manager = new ParticleSystem();
    return manager;
}

ParticleSystem::ParticleSystem() {}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::add_particle(const Vector2& position, const Vector2& velocity, float life_time, int r, int g, int b, int a, bool is_blink) {
    if (particles.size() >= MAX_PARTICLES) {
        // ÒÆ³ý×î¾ÉµÄÁ£×Ó
        particles.erase(particles.begin());
    }
    particles.emplace_back(position, velocity, life_time, r, g, b, a, is_blink);
}

void ParticleSystem::on_update(float delta) {
    for (auto it = particles.begin(); it != particles.end();) {
        it->on_update(delta);
        if (!it->is_alive()) {
            it = particles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ParticleSystem::on_render(const Camera& camera) {
    for (auto& particle : particles) {
        particle.on_render(camera);
    }
}
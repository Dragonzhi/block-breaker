// particle_system.cpp
#include "particle_system.h"

ParticleSystem* ParticleSystem::manager = nullptr;

ParticleSystem* ParticleSystem::instance() {
    if (!manager)
        manager = new ParticleSystem();
    return manager;
}

ParticleSystem::ParticleSystem() {
    particle_pool.resize(MAX_PARTICLES);
    free_indices.reserve(MAX_PARTICLES);
    for (size_t i = 0; i < MAX_PARTICLES; ++i) {
        free_indices.push_back(i);
    }
}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::add_particle(const Vector2& position, const Vector2& velocity, float life_time, int r, int g, int b, int a, bool is_blink) {
    if (free_indices.empty()) {
        // 回收最早激活的粒子
        if (!active_particles.empty()) {
            Particle* oldest = active_particles.front();
            active_particles.erase(active_particles.begin());
            *oldest = Particle(position, velocity, life_time, r, g, b, a, is_blink);
            active_particles.push_back(oldest);
        }
        return;
    }

    size_t index = free_indices.back();
    free_indices.pop_back();
    particle_pool[index] = Particle(position, velocity, life_time, r, g, b, a, is_blink);
    active_particles.push_back(&particle_pool[index]);
}

void ParticleSystem::on_update(float delta) {
    for (auto it = active_particles.begin(); it != active_particles.end();) {
        (*it)->on_update(delta);
        if (!(*it)->is_alive()) {
            // 回收索引
            size_t index = *it - &particle_pool[0];
            free_indices.push_back(index);
            it = active_particles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ParticleSystem::on_render(const Camera& camera) {
    for (Particle* p : active_particles) {
        p->on_render(camera);
    }
}
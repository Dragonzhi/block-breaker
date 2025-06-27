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

void ParticleSystem::add_particle(const Vector2& position, const Vector2& velocity, float life_time) {
    particles.emplace_back(position, velocity, life_time);
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
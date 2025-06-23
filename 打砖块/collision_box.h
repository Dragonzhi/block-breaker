#pragma once

#include "vector2.h"
#include "collision_layer.h"

#include <functional>

class CollisionManager;

class CollisionBox
{
    friend class CollisionManager;

public:
    void set_size(const Vector2& size) {
        this->size = size;
    }

    const Vector2& get_size() const {
        return size;
    }

    const Vector2& get_position() const {
        return position;
    }

    void set_position(const Vector2& pos) {
        position = pos;
    }

    void set_enabled(bool flag) {
        enabled = flag;
    }

    void set_on_collide(std::function<void()> callback) {
        on_collide = callback;
    }

    void set_layer_src(CollisionLayer layer) {
        layer_src = layer;
    }

    void set_layer_dst(CollisionLayer layer) {
        layer_dst = layer;
    }

private:
    Vector2 size;
    Vector2 position;
    bool enabled = true;
    std::function<void()> on_collide;
    CollisionLayer layer_src = CollisionLayer::None;
    CollisionLayer layer_dst = CollisionLayer::None;

private:
    CollisionBox() = default;
    ~CollisionBox() = default;
};
#pragma once

#include "vector2.h"
#include "collision_layer.h"
#include "game_object.h"
#include <functional>

class CollisionManager;

class CollisionBox
{
    friend class CollisionManager;

public:
    enum class CollisionBoxType {
        Rectangle,
        Circle
    };
    struct CollisionInfo {
        CollisionBox* src;
        CollisionBox* dst;
        Vector2 normal;
        float penetration;
    };
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

    bool get_enabled()const {
        return enabled;
    }

    void set_enabled(bool flag) {
        enabled = flag;
    }

    void set_on_collide(std::function<void(CollisionBox*, CollisionBox*, const CollisionInfo& info)> callback) {
        on_collide = callback;
    }

    void set_layer_src(CollisionLayer layer) {
        layer_src = layer;
    }

    void set_layer_dst(CollisionLayer layer) {
        layer_dst = layer;
    }

    CollisionLayer get_layer_src() const {
        return layer_src;
    }

    CollisionLayer get_layer_dst() const {
        return layer_dst;
    }

    void set_owner(GameObject* owner) {
        this->owner = owner;
    }

    GameObject* get_owner() const {
        return owner;
    }

    void set_type(CollisionBoxType type) {
        this->type = type;
    }

    CollisionBoxType get_type() const {
        return type;
    }
private:
    GameObject* owner = nullptr;
    Vector2 size;
    Vector2 position;
    bool enabled = true;
    std::function<void(CollisionBox*, CollisionBox*, const CollisionInfo& info)> on_collide;
    CollisionLayer layer_src = CollisionLayer::None;
    CollisionLayer layer_dst = CollisionLayer::None;
    CollisionBoxType type = CollisionBoxType::Rectangle;
private:
    CollisionBox() = default;
    ~CollisionBox() = default;
};
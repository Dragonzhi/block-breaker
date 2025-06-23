#pragma once

#include <graphics.h>

#include "vector2.h"

class GameObject
{
public:
	GameObject() = default;
	~GameObject() = default;

	virtual void on_update(float delta) = 0;
	virtual void on_render() = 0;
	virtual void on_input(const ExMessage& msg) = 0;
	
    void set_position(const Vector2& pos) {
        position = pos;
    }

    const Vector2& get_position() const {
        return position;
    }

    void set_velocity(const Vector2& velocity) {
        this->velocity = velocity;
    }

    const Vector2& get_velocity() const {
        return velocity;
    }

protected:
	Vector2 position;
	Vector2 velocity;

};

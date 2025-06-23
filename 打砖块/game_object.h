#pragma once

#include <graphics.h>

#include "vector2.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void on_update(float delta);
	virtual void on_render();
	virtual void on_input(const ExMessage& msg);
	
protected:
	Vector2 position;
	Vector2 velocity;

};

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}
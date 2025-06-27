#pragma once
#include<graphics.h>
#include "camera.h"

class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual void on_update(float delta) {}
	virtual void on_input(const ExMessage& msg) {}
	virtual void on_enter(){}
	virtual void on_exit() {}
	virtual void on_render(const Camera& camera) {}

private:
};
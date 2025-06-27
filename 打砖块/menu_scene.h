#pragma once
#include "scene.h"
#include "util.h"
#include "button.h"

class MenuScene : public Scene
{
public:
	MenuScene() ;
	~MenuScene() ;

	void on_update(float delta) override ;
	void on_input(const ExMessage& msg) override ;
	void on_enter() override ;
	void on_exit() override ;
	void on_render(const Camera& camera) override ;
private:
	IMAGE* img_background = nullptr;
	Button* button_start = nullptr;
};


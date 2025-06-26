#pragma once
#include "scene.h"
class GameScene : public Scene
{
public:
	GameScene() ;
	~GameScene() ;

	void on_update(float delta) override ;
	void on_input(const ExMessage& msg) override ;
	void on_enter() override ;
	void on_exit() override ;
	void on_render() override ;

	void render_game_overed();

private:
	IMAGE* img_background;
	int AllPoints = 0;

	bool is_game_overed = false;
};


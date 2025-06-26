#pragma once
#include "scene.h"
#include "button.h"

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

	void rest();

	void render_game_overed();

private:
	IMAGE* img_background = nullptr;
	int AllPoints = 0;

	bool is_game_overed = false;
	bool is_end_game_bg_ok = false;
	bool is_debug = false;

	POINT end_game_bg_position;

	IMAGE* image_end_game_bg = nullptr;
	IMAGE* image_end_game_star_left = nullptr;
	IMAGE* image_end_game_star_centre = nullptr;
	IMAGE* image_end_game_star_right = nullptr;

	Button* button_next = nullptr;
	Button* button_home = nullptr;
	Button* button_rest = nullptr;
};


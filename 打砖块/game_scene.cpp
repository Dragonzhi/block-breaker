#include "game_scene.h"
#include "character_manager.h"
#include "collision_manager.h"

GameScene::GameScene() {}
GameScene::~GameScene() {}

void GameScene::on_update(float delta)  {
	CharacterManager::instance()->on_update(delta);
	CollisionManager::instance()->process_collide();
}

void GameScene::on_input(const ExMessage& msg)  {
	CharacterManager::instance()->on_input(msg);
}

void GameScene::on_enter()  {}

void GameScene::on_exit()  {}

void GameScene::on_render()  {
	CharacterManager::instance()->on_render();
	CollisionManager::instance()->on_debug_render();
}
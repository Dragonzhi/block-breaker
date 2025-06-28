#include "character_manager.h"

CharacterManager* CharacterManager::manager = nullptr;

CharacterManager* CharacterManager::instance()
{
    if (!manager)
        manager = new CharacterManager();
    return manager;
}

void CharacterManager::on_input(const ExMessage& msg)
{
    player->on_input(msg);
    for (Ball* ball : balls) {
        ball->on_input(msg);
    }
}

void CharacterManager::on_update(float delta)
{
    player->on_update(delta);
    for (Ball* ball : balls) {
        ball->on_update(delta);
    }
}

void CharacterManager::on_render(const Camera& camera)
{
    player->on_render(camera);
    for (Ball* ball : balls) {
        ball->on_render(camera);
    }
}


CharacterManager::CharacterManager() {
    player = new Paddle();
    // 初始化一个球
    Vector2 temp = { 0,0 };
    balls.push_back(new Ball(0, 0, temp, true));
    for (Ball* ball : balls) {
        ball->set_paddle((Character*)player);
    }
}

CharacterManager::~CharacterManager() {
    delete player;
    for (auto ball : balls) {
        delete ball;
    }
    balls.clear();
}
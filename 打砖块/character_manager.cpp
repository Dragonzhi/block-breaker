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
    ball->on_input(msg);
}

void CharacterManager::on_update(float delta)
{
    player->on_update(delta);
    ball->on_update(delta);
}

void CharacterManager::on_render()
{
    player->on_render();
    ball->on_render();
}

CharacterManager::CharacterManager()
{
    player = new Paddle();
    ball = new Ball();

}

CharacterManager::~CharacterManager()
{
    delete player;
}
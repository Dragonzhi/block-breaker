#include "character_manager.h"
#include "player.h"
#include "bullet_time_manager.h"

BrickManager* BrickManager::manager = nullptr;

BrickManager* BrickManager::instance()
{
    if (!manager)
        manager = new BrickManager();
    return manager;
}

void BrickManager::on_input(const ExMessage& msg)
{
    player->on_input(msg);
}

void BrickManager::on_update(float delta)
{
    player->on_update(delta);
}

void BrickManager::on_render()
{
    BulletTimeManager::instance()->post_process();
    player->on_render();
}

BrickManager::BrickManager()
{
    player = new Player();
}

BrickManager::~BrickManager()
{
    delete player;
    delete enemy;
}
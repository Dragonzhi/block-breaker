#include "brick_manager.h"
#include <random>

BrickManager* BrickManager::manager = nullptr;

BrickManager* BrickManager::instance()
{
    if (manager == nullptr)
    {
        manager = new BrickManager();
    }
    return manager;
}

BrickManager::BrickManager() = default;

BrickManager::~BrickManager()
{
    for (Brick* brick : bricks)
    {
        delete brick;
    }
    bricks.clear();
}

void BrickManager::on_input(const ExMessage& msg)
{
    for (Brick* brick : bricks)
    {
        brick->on_input(msg);
    }
}

void BrickManager::on_update(float delta)
{
    for (Brick* brick : bricks)
    {
        if (!brick->check_is_active()) {
            bricks.erase(std::remove(bricks.begin(), bricks.end(), brick),
                bricks.end());
            delete brick;
        }
        else {
            brick->on_update(delta);
        }
    }
}

void BrickManager::on_render()
{
    for (Brick* brick : bricks)
    {
        brick->on_render();
    }
}

void BrickManager::fillUpperHalfRandomly(int screenWidth, int screenHeight)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(0, screenWidth - 40);
    std::uniform_int_distribution<> yDist(0, screenHeight / 2 - 20);

    const int maxAttempts = 100; // 最大尝试次数
    int attempts = 0;

    while (attempts < maxAttempts)
    {
        int x = xDist(gen);
        int y = yDist(gen);
        Brick* newBrick = new Brick(x, y);

        if (!isOverlapping(newBrick))
        {
            bricks.push_back(newBrick);
        }
        else
        {
            delete newBrick;
        }

        attempts++;
    }
}

bool BrickManager::isOverlapping(const Brick* newBrick)
{
    for (Brick* existingBrick : bricks)
    {
        // 简单的矩形重叠检测
        int newLeft = newBrick->get_position().x;
        int newRight = newLeft + 40;
        int newTop = newBrick->get_position().y;
        int newBottom = newTop + 20;

        int existingLeft = existingBrick->get_position().x;
        int existingRight = existingLeft + 40;
        int existingTop = existingBrick->get_position().y;
        int existingBottom = existingTop + 20;

        if (!(newRight < existingLeft || newLeft > existingRight || newBottom < existingTop || newTop > existingBottom))
        {
            return true;
        }
    }
    return false;
}
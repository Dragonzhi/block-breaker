#pragma once

#include "brick.h"
#include <graphics.h>
#include <vector>

class BrickManager
{
public:
    static BrickManager* instance();
    void on_input(const ExMessage& msg);
    void on_update(float delta);
    void on_render(const Camera& camera);
    void fillUpperHalfRandomly(int screenWidth, int screenHeight);
    void fillUpperHalfRandomly_int(int screenWidth, int screenHeight, int level);
    void fillWithGrid(int screenWidth, int screenHeight, int rows, int cols, int level);
    void fillFromCSV(const std::string& csvPath, int screenWidth, int screenHeight);

    void clearAllBricks();
    void rest(int screenWidth, int screenHeight);

    bool isAllBroken() const {
        if (bricks.empty())
            return true;
        return false;
    }

private:
    static BrickManager* manager;
    std::vector<Brick*> bricks;

private:
    BrickManager();
    ~BrickManager();


    bool isOverlapping(const Brick* newBrick);
};
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
    void on_render();
    void fillUpperHalfRandomly(int screenWidth, int screenHeight);
    void fillUpperHalfRandomly_int(int screenWidth, int screenHeight);
private:
    static BrickManager* manager;
    std::vector<Brick*> bricks;

private:
    BrickManager();
    ~BrickManager();


    bool isOverlapping(const Brick* newBrick);
};
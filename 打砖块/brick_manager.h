#pragma once

#include <graphics.h>

class BrickManager
{
public:
    static BrickManager* instance();

    void on_input(const ExMessage& msg);
    void on_update(float delta);
    void on_render();

private:
    static BrickManager* manager;

private:
    BrickManager();
    ~BrickManager();
};
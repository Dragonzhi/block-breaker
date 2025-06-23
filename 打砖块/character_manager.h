#pragma once

#include "character.h"
#include "ball.h"
#include "paddle.h"

class CharacterManager
{
public:
    static CharacterManager* instance();

    Character* get_player()
    {
        return player;
    }

    void on_input(const ExMessage& msg);
    void on_update(float delta);
    void on_render();

private:
    static CharacterManager* manager;
    Paddle* player = nullptr;
    Ball* ball = nullptr;

private:
    CharacterManager();
    ~CharacterManager();
};
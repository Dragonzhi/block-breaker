#pragma once

#include "character.h"

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
    Character* player = nullptr;

private:
    CharacterManager();
    ~CharacterManager();
};
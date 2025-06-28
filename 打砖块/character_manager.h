#pragma once

#include "character.h"
#include "ball.h"
#include "paddle.h"

class CharacterManager
{
public:
    static CharacterManager* instance();

    Character* get_player() {
        return player;
    }

    std::vector<Ball*>& get_balls() { 
        return balls; 
    }

    void add_ball(int x, int y, const Vector2& velocity, bool is_ready) {
        balls.push_back(new Ball(x, y, velocity, is_ready));
        balls.back()->set_paddle(player);
    }

    void remove_inactive_balls() {
        auto it = balls.begin();
        while (it != balls.end()) {
            if (!(*it)->check_enable()) {
                delete* it;
                it = balls.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void remove_all_balls() {
        for (auto ball : balls) {
            delete ball;
        }
        balls.clear();
    }

    void on_input(const ExMessage& msg);
    void on_update(float delta);
    void on_render(const Camera& camera);

private:
    static CharacterManager* manager;
    Paddle* player = nullptr;
    std::vector<Ball*> balls;

private:
    CharacterManager();
    ~CharacterManager();
};
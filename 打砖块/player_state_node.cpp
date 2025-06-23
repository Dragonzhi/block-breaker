#include "character_manager.h"
#include "paddle.h"
#include "player_state_node.h"
#include <thread>

PlayerDeadState::PlayerDeadState()
{
    timer.set_wait_time(2.0f);    
    timer.set_one_shot(true);     
    timer.set_on_timeout([&]()
        {
            MessageBox(GetHWnd(), _T("不对.....\n这样不行。"), _T("挑战失败！"), MB_OK);
            exit(0);
        });
}

void PlayerDeadState::on_enter()
{
    CharacterManager::instance()->get_player()->set_animation("dead");
    //play_audio(_T("player_dead"), false);
}

void PlayerDeadState::on_update(float delta)
{
    timer.on_update(delta);
}


void PlayerIdleState::on_enter()
{
    CharacterManager::instance()->get_player()->set_animation("idle");
}

void PlayerIdleState::on_update(float delta)
{
    Paddle* player = (Paddle*)CharacterManager::instance()->get_player();

    if (player->get_hp() <= 0)
        player->switch_state("dead");
    else if (player->is_on_floor() && player->get_move_axis() != 0)
        player->switch_state("run");
}

void PlayerMoveState::on_enter()
{
    CharacterManager::instance()->get_player()->set_animation("run");
    //play_audio(_T("player_run"), true);
}

void PlayerMoveState::on_update(float delta)
{
    Paddle* player = (Paddle*)CharacterManager::instance()->get_player();

    if (player->get_hp() <= 0)
        player->switch_state("dead");
    else if (player->get_move_axis() == 0)
        player->switch_state("idle");
}

void PlayerMoveState::on_exit()
{
    /*top_audio(_T("player_run"));*/
}
#pragma once

#include "state_node.h"
#include "timer.h"


// ����״̬
class PlayerDeadState : public StateNode
{
public:
    PlayerDeadState();
    ~PlayerDeadState() = default;

    void on_enter() override;
    void on_update(float delta) override;

private:
    Timer timer;
};

// ����״̬
class PlayerIdleState : public StateNode
{
public:
    PlayerIdleState() = default;
    ~PlayerIdleState() = default;

    void on_enter() override;
    void on_update(float delta) override;
};

// �ƶ�״̬
class PlayerMoveState : public StateNode
{
public:
    PlayerMoveState() = default;
    ~PlayerMoveState() = default;

    void on_enter() override;
    void on_update(float delta) override;
    void on_exit() override;
};
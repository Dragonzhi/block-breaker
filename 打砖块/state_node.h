#pragma once

class StateNode
{
public:
	StateNode() = default;
	~StateNode() = default;

	virtual void on_enter() {}
	virtual void on_update(float delta) {}
	virtual void on_exit() {} 
};


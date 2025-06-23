#pragma once

#include "state_node.h"

#include <string>
#include <graphics.h>
#include <unordered_map>

using namespace std;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void on_update(float delta);

	void set_entry(const string& id);
	void switch_to(const string& id);
	void register_state(const string& id, StateNode* state_node);

private:
	bool need_init = true;
	StateNode* current_state = nullptr;
	unordered_map<string, StateNode*> state_pool;
	 
};


#include "paddle.h"
#include "resources_manager.h"
#include "player_state_node.h"

Paddle::Paddle() {
	is_facing_left = false;
	position = { 250, 550 };
	logic_height = 25;
	hurt_box->set_size({ 100,20 });
	hurt_box->set_layer_src(CollisionLayer::Paddle);
	hurt_box->set_layer_dst(CollisionLayer::Ball);
	
	{
		AnimationGroup& animation_idle = animation_pool["idle"];

		Animation& animation_idle_left = animation_idle.left;
		animation_idle_left.set_interval(0.1f);
		animation_idle_left.set_loop(true);
		animation_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
		animation_idle_left.add_frame(ResourcesManager::instance()->find_image("paddle_blue_left"), 1);

		Animation& animation_idle_right = animation_idle.right;
		animation_idle_right.set_interval(0.1f);
		animation_idle_right.set_loop(true);
		animation_idle_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
		animation_idle_right.add_frame(ResourcesManager::instance()->find_image("paddle_blue_right"), 1);
	}
	//{
	//	AnimationGroup& animation_move = animation_pool["move"];

	//	Animation& animation_move_left = animation_move.left;
	//	animation_move_left.set_interval(0.1f);
	//	animation_move_left.set_loop(true);
	//	animation_move_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
	//	animation_move_left.add_frame(ResourcesManager::instance()->find_image("paddle_blue_move_left"), 1);

	//	Animation& animation_move_right = animation_move.right;
	//	animation_move_right.set_interval(0.1f);
	//	animation_move_right.set_loop(true);
	//	animation_move_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
	//	animation_move_right.add_frame(ResourcesManager::instance()->find_image("paddle_blue_move_right"), 1);
	//}
	// ×´Ì¬»ú
	{
		state_machine.register_state("idle", new PlayerIdleState());
		state_machine.register_state("move", new PlayerMoveState());
		state_machine.register_state("dead", new PlayerDeadState());

		state_machine.set_entry("idle");
	}
}

Paddle::~Paddle() {

}

void Paddle::on_input(const ExMessage& msg) {
	if (hp <= 0) return;

	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (msg.vkcode)
		{
		case 0x41:	//"A"
		case VK_LEFT:
			is_left_key_down = true;
			break;
		case 0x44:	//"D"
		case VK_RIGHT:
			is_right_key_down = true;
			break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		switch (msg.vkcode)
		{
		case 0x41:	//"A"
		case VK_LEFT:
			is_left_key_down = false;
			break;
		case 0x44:	//"D"
		case VK_RIGHT:
			is_right_key_down = false;
			break;
		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		is_shot_key_down = true;
		break;
	case WM_LBUTTONUP:
		is_shot_key_down = false;
		break;
	default:
		break;
	}
}


void Paddle::on_update(float delta) {
	if (hp > 0)
		velocity.x = get_move_axis() * SPEED_MOVE;

	if (get_move_axis() != 0)
		is_facing_left = (get_move_axis() < 0);

	Character::on_update(delta);
}

void Paddle::on_render() {
	Character::on_render();
}
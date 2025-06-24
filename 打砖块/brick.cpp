#include "brick.h"


Brick::Brick(int x, int y, int cnt, int points, Type type) : counts(cnt), points(points) {
	position.x = x;
	position.y = y;
	this->type = type;
	init();
}

Brick::~Brick() {
	CollisionManager::instance()->destroy_collision_box(hurt_box);
}

void Brick::on_update(float delta) {
	animation_brick.on_update(delta);
	animation_brick.set_position(position);
}

void Brick::on_render() {
	animation_brick.on_render();
}

void Brick::init() {
	switch (type)
	{
	case Brick::Normal:
		animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_blue"), 1);
		break;
	case Brick::Streng:
		animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_red"), 1);
		break;
	case Brick::Powerup:
		animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_yellow"), 1);
		break;
	default:
		break;
	}

	animation_brick.set_interval(0.1f); 
	animation_brick.set_loop(true);
	animation_brick.set_anchor_mode(Animation::AnchorMode::Centered);

	hurt_box = CollisionManager::instance()->create_collision_box();
	hurt_box->set_size({ 40, 20 });     // ÉèÖÃÅö×²ºÐ´óÐ¡
	hurt_box->set_layer_src(CollisionLayer::Brick);
	hurt_box->set_layer_dst(CollisionLayer::Ball);

}

void Brick::on_input(const ExMessage& msg) {}
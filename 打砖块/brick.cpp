#include "brick.h"


Brick::Brick(int x, int y, Type type) {
	position.x = x;
	position.y = y;
	this->type = type;

	animation_brick.set_interval(0.1f);
	animation_brick.set_loop(true);
	animation_brick.set_anchor_mode(Animation::AnchorMode::Centered);

	hurt_box = CollisionManager::instance()->create_collision_box();
	hurt_box->set_owner(this);
	hurt_box->set_size({ 60, 30 });     // 设置碰撞盒大小
	hurt_box->set_layer_src(CollisionLayer::Brick);
	hurt_box->set_layer_dst(CollisionLayer::Ball);
	hurt_box->set_on_collide([this](CollisionBox* src, CollisionBox* dst, const CollisionBox::CollisionInfo& info) {
		if (src && src->get_layer_src() == CollisionLayer::Ball) {
			counts--;
			cout << "Brick被击中，剩余生命: " << counts << endl;
			printf("Brick回调被触发！坐标(%.1f,%.1f)\n", position.x, position.y);

			// 手动触发球的反弹（因为球不主动检测砖块）
			Ball* ball = dynamic_cast<Ball*>(src->get_owner());
			if (ball) {
				ball->handle_brick_collision(this->hurt_box, info);
			}

			if (counts <= 0) {
				is_active = false;
				cout << "Brick被摧毁" << endl;
			}
		}
		});
	init();
}

Brick::~Brick() {
	CollisionManager::instance()->destroy_collision_box(hurt_box);
}

void Brick::on_update(float delta) {
	hurt_box->set_position(Vector2(position.x, position.y));
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
		counts = 2;
		points = 2;
		break;
	case Brick::Powerup:
		animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_yellow"), 1);
		counts = 3;
		points = 3;
		break;
	default:
		break;
	}
}

void Brick::on_input(const ExMessage& msg) {}
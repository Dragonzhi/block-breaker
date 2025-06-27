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
			// 手动触发球的反弹（因为球不主动检测砖块）
			Ball* ball = dynamic_cast<Ball*>(src->get_owner());
			if (ball) {
				ball->handle_brick_collision(this->hurt_box, info);
			}
			if (check_can_destoryed()) {
				on_hit();
			}

		}
		});

	timer_invulnerable.set_one_shot(true);
	timer_invulnerable.set_wait_time(0.1f);
	timer_invulnerable.set_on_timeout([this]() {is_invulnerable = false; });



	init();
}

Brick::~Brick() {
	CollisionManager::instance()->destroy_collision_box(hurt_box);
}
void Brick::on_hit() {
	if (!is_active || !can_be_hit()) return; // 双重检查
	is_invulnerable = true;
	counts--;
	cooldown_timer = COOLDOWN_TIME; // 重置冷却时间

	// 日志输出（调试用）
	std::cout << "Brick被击中，剩余生命: " << counts << std::endl;

	if (counts <= 0) {
		// 砖块被摧毁的逻辑
		std::cout << "Brick被摧毁" << std::endl;
		is_active = false;
	}
}

void Brick::on_update(float delta) {
	if (!is_active) return;

	if (cooldown_timer > 0) {
		cooldown_timer -= delta;
		hurt_box->set_enabled(false);
	}
	else {
		hurt_box->set_enabled(true);
		
	}

	if (is_invulnerable) {
		timer_invulnerable.on_update(delta);
	}

	hurt_box->set_position(Vector2(position.x, position.y));

	animation_brick.on_update(delta);
	animation_brick.set_position(position);

	if (is_invulnerable) {
		make_be_hit();
	}
}

void Brick::on_render(const Camera& camera) {
	animation_brick.on_render(camera);
}

void Brick::init() {
	switch (type)
	{
	case Brick::Normal:
		animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_blue"), 1);
		counts = 1;
		points = 1;
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
#include "collision_manager.h"

#include <graphics.h>

CollisionManager* CollisionManager::manager = nullptr;

CollisionManager* CollisionManager::instance() {
	if (!manager) {
		manager = new CollisionManager();
	}
	return manager;
}

CollisionManager::CollisionManager() = default;
CollisionManager::~CollisionManager() = default;

CollisionBox* CollisionManager::create_collision_box() {
	CollisionBox* collision_box = new CollisionBox();

	collision_box_list.push_back(collision_box);

	return collision_box;
}

void CollisionManager::destroy_collision_box(CollisionBox* collision_box) {
	collision_box_list.erase(std::remove(collision_box_list.begin(), collision_box_list.end(), collision_box),
		collision_box_list.end());
	delete collision_box;

}

void CollisionManager::process_collide() {
	for (CollisionBox* collision_box_src : collision_box_list) {
		if (!collision_box_src->enabled || collision_box_src->layer_dst == CollisionLayer::None) {
			continue;
		}
		for (CollisionBox* collision_box_dst : collision_box_list) {
			if (!collision_box_dst->enabled || collision_box_src == collision_box_dst
				|| collision_box_src->layer_dst != collision_box_dst->layer_src) {
				continue;
			}
			bool is_collide_x = (max(collision_box_src->position.x + collision_box_src->size.x / 2,
				collision_box_dst->position.x + collision_box_dst->size.x / 2)
				- min(collision_box_src->position.x - collision_box_src->size.x / 2,
					collision_box_dst->position.x - collision_box_dst->size.x / 2)
				<= collision_box_src->size.x + collision_box_dst->size.x);

			bool is_collide_y = (max(collision_box_src->position.y + collision_box_src->size.y / 2,
				collision_box_dst->position.y + collision_box_dst->size.y / 2)
				- min(collision_box_src->position.y - collision_box_src->size.y / 2,
					collision_box_dst->position.y - collision_box_dst->size.y / 2)
				<= collision_box_src->size.y + collision_box_dst->size.y);

			if (is_collide_x && is_collide_y && collision_box_dst->on_collide) {
				collision_box_dst->on_collide();
				printf("Collide: SRC(%d)->DST(%d)\n",
					(int)collision_box_src->layer_src, (int)collision_box_dst->layer_src);
			}
		}
	}
}

void CollisionManager::on_debug_render() {
	for (CollisionBox* collision_box : collision_box_list) {
		setlinecolor(collision_box->enabled ? RGB(255, 195, 195) : RGB(115, 115, 175));
		rectangle((int)(collision_box->position.x - collision_box->size.x / 2),
			(int)(collision_box->position.y - collision_box->size.y / 2),
			(int)(collision_box->position.x + collision_box->size.x / 2),
			(int)(collision_box->position.y + collision_box->size.y / 2));
	}
}
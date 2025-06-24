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
    //for (CollisionBox* src : collision_box_list) {
    //    if (!src->enabled || src->layer_dst == CollisionLayer::None) {
    //        continue;
    //    }

    //    for (CollisionBox* dst : collision_box_list) {
    //        // 跳过无效检测
    //        if (!dst->enabled || src == dst) {
    //            continue;
    //        }

    //        // 双向检测条件：src想检测dst 且 dst想检测src
    //        bool should_collide = (src->layer_dst == dst->layer_src) &&
    //            (dst->layer_dst == src->layer_src);

    //        // 单向检测条件：src想检测dst 且 dst接受任意碰撞
    //        bool one_way_collide = (src->layer_dst == dst->layer_src) &&
    //            (dst->layer_dst == CollisionLayer::None);

    //        if (!should_collide && !one_way_collide) {
    //            continue;
    //        }

    //        // 精确碰撞检测
    //        float src_left = src->position.x - src->size.x / 2;
    //        float src_right = src->position.x + src->size.x / 2;
    //        float src_top = src->position.y - src->size.y / 2;
    //        float src_bottom = src->position.y + src->size.y / 2;

    //        float dst_left = dst->position.x - dst->size.x / 2;
    //        float dst_right = dst->position.x + dst->size.x / 2;
    //        float dst_top = dst->position.y - dst->size.y / 2;
    //        float dst_bottom = dst->position.y + dst->size.y / 2;

    //        bool is_colliding = src_right > dst_left &&
    //            src_left < dst_right &&
    //            src_bottom > dst_top &&
    //            src_top < dst_bottom;

    //        if (is_colliding) {
    //            // 优先执行dst的回调（被碰撞方的处理）
    //            if (dst->on_collide) {
    //                dst->on_collide(src, dst);
    //                printf("Collide: %s(%d) -> %s(%d)\n",
    //                    get_layer_name(src->layer_src), (int)src->layer_src,
    //                    get_layer_name(dst->layer_src), (int)dst->layer_src);
    //            }

    //            // 如果双向检测，再执行src的回调（主动碰撞方的处理）
    //            if (should_collide && src->on_collide) {
    //                src->on_collide(dst, src);
    //            }
    //        }
    //    }
    //}
    for (auto src : collision_box_list) {
        if (!src->enabled || src->layer_dst == CollisionLayer::None) continue;

        for (auto dst : collision_box_list) {
            if (!dst->enabled || src == dst) continue;

            // 双向检测条件
            if (src->layer_dst == dst->layer_src) {
                // AABB碰撞检测（优化版）
                bool is_colliding =
                    src->position.x + src->size.x / 2 > dst->position.x - dst->size.x / 2 &&
                    src->position.x - src->size.x / 2 < dst->position.x + dst->size.x / 2 &&
                    src->position.y + src->size.y / 2 > dst->position.y - dst->size.y / 2 &&
                    src->position.y - src->size.y / 2 < dst->position.y + dst->size.y / 2;

                if (is_colliding) {
                    // 优先执行被碰撞方的回调
                    if (dst->on_collide) dst->on_collide(src, dst);

                    // 如果src也有回调且需要双向处理（如Paddle）
                    if (src->on_collide && dst->layer_dst == src->layer_src) {
                        src->on_collide(dst, src);
                    }
                }
            }
        }
    }
}

// 辅助函数（需添加到类中）
const char* CollisionManager::get_layer_name(CollisionLayer layer) {
    switch (layer) {
    case CollisionLayer::None: return "None";
    case CollisionLayer::Paddle: return "Paddle";
    case CollisionLayer::Brick: return "Brick";
    case CollisionLayer::Ball: return "Ball";
    default: return "Unknown";
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
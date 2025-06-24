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
    //        // ������Ч���
    //        if (!dst->enabled || src == dst) {
    //            continue;
    //        }

    //        // ˫����������src����dst �� dst����src
    //        bool should_collide = (src->layer_dst == dst->layer_src) &&
    //            (dst->layer_dst == src->layer_src);

    //        // ������������src����dst �� dst����������ײ
    //        bool one_way_collide = (src->layer_dst == dst->layer_src) &&
    //            (dst->layer_dst == CollisionLayer::None);

    //        if (!should_collide && !one_way_collide) {
    //            continue;
    //        }

    //        // ��ȷ��ײ���
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
    //            // ����ִ��dst�Ļص�������ײ���Ĵ���
    //            if (dst->on_collide) {
    //                dst->on_collide(src, dst);
    //                printf("Collide: %s(%d) -> %s(%d)\n",
    //                    get_layer_name(src->layer_src), (int)src->layer_src,
    //                    get_layer_name(dst->layer_src), (int)dst->layer_src);
    //            }

    //            // ���˫���⣬��ִ��src�Ļص���������ײ���Ĵ���
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

            // ˫��������
            if (src->layer_dst == dst->layer_src) {
                // AABB��ײ��⣨�Ż��棩
                bool is_colliding =
                    src->position.x + src->size.x / 2 > dst->position.x - dst->size.x / 2 &&
                    src->position.x - src->size.x / 2 < dst->position.x + dst->size.x / 2 &&
                    src->position.y + src->size.y / 2 > dst->position.y - dst->size.y / 2 &&
                    src->position.y - src->size.y / 2 < dst->position.y + dst->size.y / 2;

                if (is_colliding) {
                    // ����ִ�б���ײ���Ļص�
                    if (dst->on_collide) dst->on_collide(src, dst);

                    // ���srcҲ�лص�����Ҫ˫������Paddle��
                    if (src->on_collide && dst->layer_dst == src->layer_src) {
                        src->on_collide(dst, src);
                    }
                }
            }
        }
    }
}

// ��������������ӵ����У�
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
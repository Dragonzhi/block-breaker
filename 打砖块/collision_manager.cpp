#include "collision_manager.h"
#include "util.h"
#include "brick.h"
#include "ball.h"
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
    auto it = std::find(collision_box_list.begin(), collision_box_list.end(), collision_box);
    if (it != collision_box_list.end()) {
        // �Ƚ�����ײ��
        (*it)->enabled = false;
        collision_box_list.erase(it);
    }
}


bool is_collide_rect_rect(const CollisionBox* rect1, const CollisionBox* rect2, Vector2& normal, float& penetration) {
    // ������α߽�
    float rect1_left = rect1->get_position().x - rect1->get_size().x / 2;
    float rect1_right = rect1->get_position().x + rect1->get_size().x / 2;
    float rect1_top = rect1->get_position().y - rect1->get_size().y / 2;
    float rect1_bottom = rect1->get_position().y + rect1->get_size().y / 2;

    float rect2_left = rect2->get_position().x - rect2->get_size().x / 2;
    float rect2_right = rect2->get_position().x + rect2->get_size().x / 2;
    float rect2_top = rect2->get_position().y - rect2->get_size().y / 2;
    float rect2_bottom = rect2->get_position().y + rect2->get_size().y / 2;

    // ����Ƿ���ײ
    if (rect1_right < rect2_left || rect1_left > rect2_right ||
        rect1_bottom < rect2_top || rect1_top > rect2_bottom) {
        return false;
    }

    // �����ص���
    float overlap_left = rect1_right - rect2_left;
    float overlap_right = rect2_right - rect1_left;
    float overlap_top = rect1_bottom - rect2_top;
    float overlap_bottom = rect2_bottom - rect1_top;

    // �ҳ���С�ص�����
    float min_overlap = min_ex({ overlap_left, overlap_right, overlap_top, overlap_bottom });

    // ȷ�����߷���
    if (min_overlap == overlap_left) {
        normal = Vector2(-1, 0);  // ��������
    }
    else if (min_overlap == overlap_right) {
        normal = Vector2(1, 0);   // ��������
    }
    else if (min_overlap == overlap_top) {
        normal = Vector2(0, -1);  // ��������
    }
    else {
        normal = Vector2(0, 1);   // ��������
    }
    penetration = min_overlap;
    return true;
}

bool is_collide_circle_rect(const CollisionBox* circle, const CollisionBox* rect, Vector2& normal, float& penetration) {

    // ��Ӽ���״̬���
    if (!circle->get_enabled() || !rect->get_enabled()) return false;

    // 1. ������α߽�
    float rect_left = rect->get_position().x - rect->get_size().x / 2;
    float rect_right = rect->get_position().x + rect->get_size().x / 2;
    float rect_top = rect->get_position().y - rect->get_size().y / 2;
    float rect_bottom = rect->get_position().y + rect->get_size().y / 2;

    // 2. �ҵ�Բ����������ĵ�
    float closest_x = max(rect_left, min(circle->get_position().x, rect_right));
    float closest_y = max(rect_top, min(circle->get_position().y, rect_bottom));

    // 3. �������
    float dx = circle->get_position().x - closest_x;
    float dy = circle->get_position().y - closest_y;
    float distance_sq = dx * dx + dy * dy;
    float radius = circle->get_size().x / 2;

    // 4. ����Ƿ���ײ
    if (distance_sq > radius * radius) {
        return false;
    }
    float distance = 0.0f;
    // 5. ������ײ����
    if (distance_sq > 0) {
        distance = std::sqrt(distance_sq);
        normal.x = dx / distance;
        normal.y = dy / distance;
    }
    else {
        // Բ���ھ����ڲ���ʹ��Բ�ĵ��������ĵķ���
        Vector2 to_center = rect->get_position() - circle->get_position();
        if (to_center.x == 0 && to_center.y == 0) {
            normal = Vector2(0, 1);  // Ĭ������
            distance = radius; // ��ȫ�غ�ʱ��Ϊ�뾶
        }
        else {
            normal = to_center.normalize();
            distance = 0; // ��Ϊ��ȫ��͸
        }
    }
    // ���㴩͸���
    penetration = max(0.0f, radius - distance);

    // �ڷ���trueǰ��Ӷ����飺
    if (penetration < 0.5f) { // ���Ӵ�͸��ֵ
        return false;
    }

    return true;
}
bool is_collide_circle_circle(const CollisionBox* circle1, const CollisionBox* circle2, Vector2& normal) {
    float dx = circle2->get_position().x - circle1->get_position().x;
    float dy = circle2->get_position().y - circle1->get_position().y;
    float distance_sq = dx * dx + dy * dy;

    float radius1 = circle1->get_size().x / 2;
    float radius2 = circle2->get_size().x / 2;
    float min_distance = radius1 + radius2;

    if (distance_sq > min_distance * min_distance) {
        return false;
    }

    if (distance_sq > 0) {
        float distance = std::sqrt(distance_sq);
        normal.x = dx / distance;
        normal.y = dy / distance;
    }
    else {
        normal = Vector2(1, 0);  // Բ���غ�ʱ��Ĭ�Ϸ���
    }

    return true;
}

void CollisionManager::process_collide() {
    collision_box_list_temp = collision_box_list;

    for (CollisionBox* src : collision_box_list_temp) {
        if (!src->enabled || src->layer_dst == CollisionLayer::None) {
            continue;
        }

        for (CollisionBox* dst : collision_box_list_temp) {
            if (!dst || !dst->enabled || src == dst) {
                continue;
            }

            // ����Ƿ�Ӧ�ü�������ײ
            if (src->layer_dst != dst->layer_src) {
                continue;
            }

            bool is_collide = false;
            Vector2 collision_normal;
            float penetration = 0.0f;

            // ������״����ѡ���ⷽ��
            if (src->get_type() == CollisionBox::CollisionBoxType::Rectangle &&
                dst->get_type() == CollisionBox::CollisionBoxType::Rectangle) {

                is_collide = is_collide_rect_rect(src, dst, collision_normal, penetration);
            }
            else if (src->get_type() == CollisionBox::CollisionBoxType::Circle &&
                dst->get_type() == CollisionBox::CollisionBoxType::Rectangle) {

                is_collide = is_collide_circle_rect(src, dst, collision_normal, penetration);
            }
            else if (src->get_type() == CollisionBox::CollisionBoxType::Rectangle &&
                dst->get_type() == CollisionBox::CollisionBoxType::Circle) {

                // ������������ת����
                is_collide = is_collide_circle_rect(dst, src, collision_normal, penetration);
                collision_normal = 0.0f - collision_normal;
            }
            else if (src->get_type() == CollisionBox::CollisionBoxType::Circle &&
                dst->get_type() == CollisionBox::CollisionBoxType::Circle) {

                is_collide = is_collide_circle_circle(src, dst, collision_normal);
            }

            if (is_collide) {
                // ������ײ��Ϣ�ṹ
                CollisionBox::CollisionInfo info;
                info.src = src;
                info.dst = dst;
                info.normal = collision_normal;
                info.penetration = penetration; // ��Ӵ�͸���

                // �����ص�������˳������Ϊ (src, dst, info)��
                if (src->on_collide) {
                    printf("-----------��ײ�ص���ʼ--------------\n");
                    src->on_collide(dst, src, info);
                }

                // �������
                printf("Collide: %s(%d) -> %s(%d) with normal (%.2f, %.2f)\n",
                    get_layer_name(src->layer_src), (int)src->layer_src,
                    get_layer_name(dst->layer_src), (int)dst->layer_src,
                    collision_normal.x, collision_normal.y);
                printf("��ײ����: (%.1f,%.1f), ����(%.2f,%.2f), ��͸%.2f\n",
                    info.src->position.x, info.src->position.y,
                    info.normal.x, info.normal.y,
                    info.penetration);
            }
        }
    }
}

// ��������
const char* CollisionManager::get_layer_name(CollisionLayer layer) {
    switch (layer) {
    case CollisionLayer::None: return "None";
    case CollisionLayer::Paddle: return "Paddle";
    case CollisionLayer::Brick: return "Brick";
    case CollisionLayer::Ball: return "Ball";
    default: return "Unknown";
    }
}
void CollisionManager::on_debug_render(const Camera& camera) {
    for (CollisionBox* collision_box : collision_box_list) {
        setlinecolor(collision_box->enabled ? RGB(255, 195, 195) : RGB(115, 115, 175));
        if (collision_box->get_type() == CollisionBox::CollisionBoxType::Rectangle) {
            rectangle((int)(collision_box->position.x - collision_box->size.x / 2),
                (int)(collision_box->position.y - collision_box->size.y / 2),
                (int)(collision_box->position.x + collision_box->size.x / 2),
                (int)(collision_box->position.y + collision_box->size.y / 2));
        }
        else if (collision_box->get_type() == CollisionBox::CollisionBoxType::Circle) {
            circle((int)collision_box->position.x, (int)collision_box->position.y, (int)(collision_box->size.x / 2));
        }
    }
}
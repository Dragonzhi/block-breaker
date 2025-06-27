#pragma once
#include "util.h"
#include "atlas.h"
#include "vector2.h"

#include <vector>
#include <graphics.h>
#include <functional>

class Animation
{
public:
	enum class AnchorMode {
		Centered,
		BottomCentered
	};
public:
	Animation() {
		timer.set_one_shot(false);
		timer.set_on_timeout([&]() {
			idx_frame++;
			if (idx_frame >= frame_list.size()) {
				idx_frame = is_loop ? 0 : frame_list.size() - 1;
				if (!is_loop && on_finished) {
					on_finished();
				}
			}
			});
	}
	~Animation() = default;

	void reset() {
		timer.restart();
		idx_frame = 0;
	}

	void set_loop(bool flag) {
		is_loop = flag;
	}

	void set_interval(float seconds) {
		timer.set_wait_time(seconds);
	}

	void set_position(const Vector2& pos) {
		position = pos;
	}

	size_t get_idx_frame() {
		return idx_frame;
	}

	void on_update(float delta) {
		timer.on_update(delta);
	}

	void on_render() {
		// 调试输出
		//std::cout << "Rendering frame: " << idx_frame << std::endl;
		const Frame& frame = frame_list[idx_frame];
		Rect rect_dst;
		rect_dst.x = (int)position.x - frame.rect_src.w / 2;
		rect_dst.y = (anchor_mode == AnchorMode::Centered)
			? (int)position.y - frame.rect_src.h / 2 : (int)position.y - frame.rect_src.h;
		rect_dst.w = frame.rect_src.w, rect_dst.h = frame.rect_src.h;
		putimage_ex(frame.image, &rect_dst, &frame.rect_src);
	}

	/*void on_draw(const Camera& camera, int x, int y) const {

	}*/

	void set_on_finished(std::function<void()> callback) {
		this->on_finished = callback;
	}

	void set_anchor_mode(AnchorMode new_anchor_mode) {
		anchor_mode = new_anchor_mode;
	}

	void add_frame(IMAGE* image, int num_h)
	{
		int width = image->getwidth();
		int height = image->getheight();
		int width_frame = width / num_h;
		for (int i = 0; i < num_h; i++)
		{
			Rect rect_src;
			rect_src.x = i * width_frame, rect_src.y = 0;
			rect_src.w = width_frame, rect_src.h = height;
			frame_list.emplace_back(image, rect_src);
		}
			
	}

	void add_frame(Atlas* atlas)
	{
		for (int i = 0; i < atlas->get_size(); i++)
		{
			IMAGE* image = atlas->get_image(i);
			int width = image->getwidth();
			int height = image->getheight();
			Rect rect_src;
			rect_src.x = 0, rect_src.y = 0;
			rect_src.w = width, rect_src.h = height;
			frame_list.emplace_back(image, rect_src);
		}
	}	
	
	IMAGE* get_frame() {
		return frame_list[idx_frame].image;
	}
		
private:
	struct Frame {
		Rect rect_src;
		IMAGE* image = nullptr;
		Frame() = default;
		Frame(IMAGE* image, const Rect& rect_src) :
			image(image), rect_src(rect_src) { }
		~Frame() = default;
	};

private:
	Timer timer;		//计时器
	Vector2 position;
	size_t idx_frame = 0;	//帧索引
	bool is_loop = true;//是否循环
	std::function<void()> on_finished;
	std::vector<Frame> frame_list;
	AnchorMode anchor_mode = AnchorMode::Centered;

};


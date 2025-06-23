#pragma once

#include <functional>


class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void on_update(float delta) {
		if (paused) {
			return;
		}

		pass_time += delta;
		if (pass_time >= wait_time) {
			bool can_shot = !one_shot || (one_shot && !shotted);
			shotted = true;
			if (can_shot && callback) {
				callback();
			}
			pass_time -= wait_time;
		}
	}

	void set_on_timeout(std::function<void()> callback) {
		this->callback = callback;
	}

	void restart() {
		pass_time = 0;
		shotted = false;
	}

	void set_wait_time(float val) {
		wait_time = val;
	}

	void set_one_shot(bool flag) {
		shotted = flag;
	}

	void pause() {
		paused = true;
	}

	void resume() {
		paused = false;
	}

private:
	std::function<void()> callback;
	float pass_time = 0;			//�ѹ�ʱ��
	float wait_time = 0;			//�ȴ�ʱ��
	bool paused = false;		//�Ƿ���ͣ
	bool shotted = false;		//�Ƿ񴥷�
	bool one_shot = false;		//���δ���
};
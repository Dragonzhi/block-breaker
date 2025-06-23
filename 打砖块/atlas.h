#pragma once

#include <graphics.h>
#include <vector>

class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

	void load(LPCTSTR path_template, int num)
	{
		img_list.clear();
		img_list.resize(num);

		TCHAR path_file[256];
		for (int i = 0; i < num; i++)
		{
			_stprintf_s(path_file, path_template, i + 1);
			loadimage(&img_list[i], path_file);
		}

	}

	void clear() {
		img_list.clear();
	}

	int get_size() {
		return (int)img_list.size();
	}

	void add_image(const IMAGE& img)
	{
		img_list.push_back(img);
	}

	IMAGE* get_image(int index)
	{
		if (index < 0 || index >= img_list.size())
			return nullptr; // 返回nullptr表示索引越界
		return &img_list[index];
	}

private:
	std::vector<IMAGE> img_list; // 存储图片的容器
};

#pragma once

#include "atlas.h"
#include "util.h"

#include <string>
#include <graphics.h>
#include <unordered_map>

using namespace std;

class ResourcesManager
{
public:
	static ResourcesManager* instance();

	void load();

	Atlas* find_atlas(const string& id) const;
	IMAGE* find_image(const string& id) const;

private:
	ResourcesManager();
	~ResourcesManager();

	void flip_image(IMAGE* src_image, IMAGE* dst_image, int num_h = 1);
	void flip_image(const string& src_id, const string& dst_id, int num_h = 1);
	void flip_atlas(const string& src_id, const string& dst_id);

private:
	static ResourcesManager* manager;

	unordered_map<string, Atlas*> atlas_pool;
	unordered_map<string, IMAGE*> image_pool;
};


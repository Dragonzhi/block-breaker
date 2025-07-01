#include "resources_manager.h"
#include "sound_manager.h"

struct ImageResInfo{
	string id;
	LPCTSTR path;
};

struct AtlasResInfo {
	string id;
	LPCTSTR path;
	int num_frame = 0;
};

static const vector<ImageResInfo> image_info_list = {  
	{ "background",									_T(R"(resources\background.png)")},
	{ "menu_titile",								_T(R"(resources\menu_titile.png)")},
	{ "purple_buff",								_T(R"(resources\purple_buff_background.png)")},
	{ "paddle_blue_right",							_T(R"(resources\paddleBlu.png)") },
	{ "paddle_big",									_T(R"(resources\paddleRed.png)") },
	{ "ball",										_T(R"(resources\ballBlue.png)") },
	{ "ball_big",									_T(R"(resources\ballGrey.png)") },
	{ "brick_blue",									_T(R"(resources\element_blue_rectangle_glossy.png)") },
	{ "brick_red",									_T(R"(resources\element_red_rectangle_glossy.png)") },
	{ "brick_yellow",								_T(R"(resources\element_yellow_rectangle_glossy.png)") },
	{ "brick_green",								_T(R"(resources\element_green_rectangle_glossy.png)") },
	{ "brick_grey",									_T(R"(resources\element_grey_rectangle_glossy.png)") },
	{ "brick_purple",								_T(R"(resources\element_purple_rectangle_glossy.png)") },
	{ "end_game_background",						_T(R"(resources\endGame_back.png)") },
	{ "end_game_background_star_left",				_T(R"(resources\endGame_star_left.png)") },
	{ "end_game_background_star_right",				_T(R"(resources\endGame_star_right.png)") },
	{ "end_game_background_star_centre",			_T(R"(resources\endGame_star_centre.png)") },
	{ "end_game_background_button_Home_hold",		_T(R"(resources\endGameButtonHome_hold.png)") },
	{ "end_game_background_button_Home_press",		_T(R"(resources\endGameButtonHome_press.png)") },
	{ "end_game_background_button_Home_idle",		_T(R"(resources\endGameButtonHome_idle.png)") },
	{ "end_game_background_button_Next_hold",		_T(R"(resources\endGameButtonNext_hold.png)") },
	{ "end_game_background_button_Next_press",		_T(R"(resources\endGameButtonNext_press.png)") },
	{ "end_game_background_button_Next_idle",		_T(R"(resources\endGameButtonNext_idle.png)") },
	{ "end_game_background_button_Rest_hold",		_T(R"(resources\endGameButtonRest_hold.png)") },
	{ "end_game_background_button_Rest_press",		_T(R"(resources\endGameButtonRest_press.png)") },
	{ "end_game_background_button_Rest_idle",		_T(R"(resources\endGameButtonRest_idle.png)") },
};

static const vector<AtlasResInfo> atlas_info_list = {
	/*{ "barb_break",	_T(R"(resources\enemy\barb_break\%d.png)"), 3 }
	*/
};


static inline bool check_image_valid(IMAGE* image) {
	return GetImageBuffer(image);
}


ResourcesManager::ResourcesManager() = default;

ResourcesManager::~ResourcesManager() = default;

ResourcesManager* ResourcesManager::manager = nullptr;

ResourcesManager* ResourcesManager::instance() {
	if (!manager) {
		manager = new ResourcesManager();
	}

	return manager;
}

void ResourcesManager::load()
{
	for (const auto& info : image_info_list)
	{
		IMAGE* image = new IMAGE();
		loadimage(image, info.path);
		if (!check_image_valid(image))
			throw info.path;
		image_pool[info.id] = image;
	}

	for (const auto& info : atlas_info_list)
	{
		Atlas* atlas = new Atlas();
		atlas->load(info.path, info.num_frame);
		for (int i = 0; i < atlas->get_size(); i++)
		{
			IMAGE* image = atlas->get_image(i);
			if (!check_image_valid(image))
				throw info.path;
		}
		atlas_pool[info.id] = atlas;
	}

	flip_image("paddle_blue_right", "paddle_blue_left", 1);

	//flip_atlas("sword_left", "sword_right");
	SoundManager::instance()->loadSound(_T(R"(resources\click-a.wav)"),									 _T("click"));
	SoundManager::instance()->loadSound(_T(R"(resources\mr-23142.wav)"),								 _T("game_bgm"), true);
	SoundManager::instance()->loadSound(_T(R"(resources\adventures-loop-music-226836.wav)"),			 _T("menu_bgm"), true);
	SoundManager::instance()->loadSound(_T(R"(resources\purchase-succesful-ingame-230550.wav)"),		 _T("succesful_sounds"));
	SoundManager::instance()->loadSound(_T(R"(resources\bowling-ball-90863.wav)"),						 _T("ball_paddle"),	false,10);
	SoundManager::instance()->loadSound(_T(R"(resources\winner.wav)"),									 _T("winner_sounds"));
	SoundManager::instance()->loadSound(_T(R"(resources\game_over.wav)"),								 _T("game_over"));
	SoundManager::instance()->loadSound(_T(R"(resources\pressed_audio.wav)"),							 _T("pressed_audio"));
	SoundManager::instance()->loadSound(_T(R"(resources\brick-dropped-on-other-bricks-14722.wav)"),	     _T("ball_brick"), false, 10);
	SoundManager::instance()->loadSound(_T(R"(resources\bricks-104933.wav)"),							 _T("brick_broken"), false, 10);
	SoundManager::instance()->loadSound(_T(R"(resources\ball_down.wav)"),								 _T("ball_down"));
	SoundManager::instance()->loadSound(_T(R"(resources\ball_windows.wav)"),							 _T("ball_windows"), false, 10);

	
	std::cout << "资源加载完成" << std::endl;
}

Atlas* ResourcesManager::find_atlas(const string& id) const{
	const auto& itor = atlas_pool.find(id);
	if (itor == atlas_pool.end())
		return nullptr;
	return
		itor->second;
}

IMAGE* ResourcesManager::find_image(const std::string& id) const{
	const auto& itor = image_pool.find(id);
	if (itor == image_pool.end())
		return nullptr;

	return itor->second;
}

void ResourcesManager::flip_image(IMAGE* src_image, IMAGE* dst_image, int num_h){
	int w = src_image->getwidth();
	int h = src_image->getheight();
	int w_frame = w / num_h;
	Resize(dst_image, w, h);
	DWORD* src_buffer = GetImageBuffer(src_image);
	DWORD* dst_buffer = GetImageBuffer(dst_image);
	for (int i = 0; i < num_h; i++)
	{
		int x_left = i * w_frame;
		int x_right = (i + 1) * w_frame;
		for (int y = 0; y < h; y++)
		{
			for (int x = x_left; x < x_right; x++)
			{
				int idx_src = y * w + x;
				int idx_dst = y * w + x_right - (x - x_left);
				dst_buffer[idx_dst] = src_buffer[idx_src];
			}
		}
	}
}

void ResourcesManager::flip_image(const string& src_id, const string& dst_id, int num_h){
	IMAGE* src_image = image_pool[src_id];
	IMAGE* dst_image = new IMAGE();

	flip_image(src_image, dst_image, num_h);

	image_pool[dst_id] = dst_image;
}

void ResourcesManager::flip_atlas(const std::string& src_id, const std::string& dst_id)
{
	Atlas* src_atlas = atlas_pool[src_id];
	Atlas* dst_atlas = new Atlas();

	for (int i = 0; i < src_atlas->get_size(); i++)
	{
		IMAGE img_flipped;
		flip_image(src_atlas->get_image(i), &img_flipped);
		dst_atlas->add_image(img_flipped);
	}

	atlas_pool[dst_id] = dst_atlas;
}
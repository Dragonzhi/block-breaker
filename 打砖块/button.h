#pragma once
#include <functional>
#include <graphics.h>
#include "util.h"
#include "resources_manager.h"

class Button
{
public:
    Button();
    Button(int x, int y, int width, int height, const char* text = nullptr);
    ~Button() = default;

    void on_update(float delta);
    void on_input(const ExMessage& msg);
    void on_render(const Camera& camera);

    void on_click(function<void()> on_click) {
        func_on_click = on_click;
    }

    void set_position(int x, int y);
    void set_size(int width, int height);
    void set_text(const char* text);
    void set_text_color(COLORREF color);
    void set_background_image(IMAGE* image);
    void set_hover_image(IMAGE* image);
    void set_click_image(IMAGE* image);

    void set_click_audio(LPCTSTR id) {
        audio_id = id;
    }

    Vector2 get_logic_center() const {
        return Vector2(position.x, position.y - height / 2);
    }

    void set_actived(bool flag) {
        if (!flag) {
			hovered = false;
			clicked = false;
			pressed = false;
		}
        is_actived = flag;
    }

    bool is_clicked() const;
    bool is_hovered() const;
    const Vector2& get_position() const;
    int get_width() const;
    int get_height() const;

private:
    const char* button_text;
    COLORREF text_color;

    IMAGE* image_bg = nullptr;
    IMAGE* image_hover = nullptr;
    IMAGE* image_press = nullptr;

    function<void()> func_on_click;
    Vector2 position;
    int width, height;

    bool hovered;
    bool clicked;
    bool pressed;
    bool is_actived = true;

    LPCTSTR audio_id = _T("pressed_audio");
    
};
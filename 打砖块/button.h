#pragma once
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
    void on_render();

    // Setters
    void set_position(int x, int y);
    void set_size(int width, int height);
    void set_text(const char* text);
    void set_text_color(COLORREF color);
    void set_background_color(COLORREF color);
    void set_hover_color(COLORREF color);
    void set_click_color(COLORREF color);

    // Getters
    bool is_clicked() const;
    bool is_hovered() const;
    int get_x() const;
    int get_y() const;
    int get_width() const;
    int get_height() const;

private:
    IMAGE* img_background;
    const char* button_text;
    COLORREF text_color;
    COLORREF bg_color;
    COLORREF hover_color;
    COLORREF click_color;

    int x, y;
    int width, height;

    bool hovered;
    bool clicked;
    bool pressed;
};
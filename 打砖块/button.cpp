#include "button.h"
#include "util.h"


Button::Button() :
    position({ 0,0 }),
    width(100), height(40),
    button_text(nullptr),
    text_color(BLACK),
    hovered(false),
    clicked(false),
    pressed(false)
{

}

Button::Button(int x, int y, int width, int height, const char* text) :
    position({(float)x,(float)y}),
    width(width), height(height),
    button_text(text),
    text_color(BLACK),
    hovered(false),
    clicked(false),
    pressed(false)
{

}

void Button::on_update(float delta)
{
    // Reset clicked state each frame
    if (clicked) {
        this->func_on_click();
        play_audio(audio_id, false);
    }
    clicked = false;
}

void Button::on_input(const ExMessage& msg)
{
    if (msg.message == WM_MOUSEMOVE)
    {
        hovered = (msg.x >= position.x && msg.x <= position.x + width &&
            msg.y >= position.y && msg.y <= position.y + height);
    }
    else if (msg.message == WM_LBUTTONDOWN)
    {
        if (hovered)
        {
            pressed = true;
        }
    }
    else if (msg.message == WM_LBUTTONUP)
    {
        if (pressed && hovered)
        {
            clicked = true;
        }
        pressed = false;
    }
}

void Button::on_render(const Camera& camera)
{
    // Draw background
    IMAGE* current_image = image_bg;
    if (pressed && hovered)
    {
        current_image = image_press;
    }
    else if (hovered)
    {
        current_image = image_hover;
    }
    else {
        current_image = image_bg;
    }
    
    if (current_image) {
        putimage_alpha(camera, position.x, position.y, current_image);
    }

    // 绘制文本
    if (button_text && button_text[0] != '\0')
    {
        settextcolor(RGB(255, 255, 255));
        TCHAR str_cmd[128];
        _stprintf_s(str_cmd, _T("%s"), button_text);

        // 4. 计算居中位置
        int text_x = position.x + width / 2;
        int text_y = position.y + height / 2;

        // 5. 绘制文本
        settextcolor(text_color);
        setbkmode(TRANSPARENT);
        outtextxy(text_x, text_y, str_cmd);
    }
}

// Setters
void Button::set_position(int x, int y)
{
    position.x = x;
    position.y = y;
}

void Button::set_size(int width, int height)
{
    this->width = width;
    this->height = height;
}

void Button::set_text(const char* text)
{
    button_text = text;
}

void Button::set_text_color(COLORREF color)
{
    text_color = color;
}

void Button::set_background_image(IMAGE* image)
{
    this->image_bg = image;
}

void Button::set_hover_image(IMAGE* image)
{
    this->image_hover = image;
}

void Button::set_click_image(IMAGE* image)
{
    this->image_press = image;
}

// Getters
bool Button::is_clicked() const
{
    return clicked;
}

bool Button::is_hovered() const
{
    return hovered;
}

const Vector2& Button::get_position() const {
    return position;
}

int Button::get_width() const
{
    return width;
}

int Button::get_height() const
{
    return height;
}
#include "button.h"
Button::Button() :
    x(0), y(0),
    width(100), height(40),
    button_text(nullptr),
    text_color(BLACK),
    bg_color(RGB(200, 200, 200)),
    hover_color(RGB(180, 180, 180)),
    click_color(RGB(160, 160, 160)),
    hovered(false),
    clicked(false),
    pressed(false)
{
    img_background = ResourcesManager::instance()->find_image("button");
}

Button::Button(int x, int y, int width, int height, const char* text) :
    x(x), y(y),
    width(width), height(height),
    button_text(text),
    text_color(BLACK),
    bg_color(RGB(200, 200, 200)),
    hover_color(RGB(180, 180, 180)),
    click_color(RGB(160, 160, 160)),
    hovered(false),
    clicked(false),
    pressed(false)
{
    img_background = ResourcesManager::instance()->find_image("button");
}

void Button::on_update(float delta)
{
    // Reset clicked state each frame
    clicked = false;
}

void Button::on_input(const ExMessage& msg)
{
    if (msg.message == WM_MOUSEMOVE)
    {
        hovered = (msg.x >= x && msg.x <= x + width &&
            msg.y >= y && msg.y <= y + height);
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

void Button::on_render()
{
    // Draw background
    COLORREF current_color = bg_color;
    if (pressed && hovered)
    {
        current_color = click_color;
    }
    else if (hovered)
    {
        current_color = hover_color;
    }

    if (img_background)
    {
        // Draw using the image if available
        putimage(x, y, width, height, img_background, 0, 0);
    }
    else
    {
        // Fallback to colored rectangle
        setfillcolor(current_color);
        fillrectangle(x, y, x + width, y + height);
    }

    // Draw border (使用更明确的函数)
    setlinecolor(BLACK);
    solidrectangle(x, y, x + width, y + height);  // 如果只需要边框，可以用rectangle

    // 绘制文本
    if (button_text && button_text[0] != '\0')
    {
        // 1. 设置字体样式
        LOGFONT lf;
        gettextstyle(&lf);  // 获取当前字体设置
        lf.lfHeight = 20;   // 字体高度（像素）
        _tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("宋体"));
        settextstyle(&lf);  // 应用新字体

        // 2. 转换字符编码
        TCHAR wtext[256];
        _stprintf_s(wtext, _T("%hs"), button_text);

        // 3. 计算文本尺寸（EasyX标准方式）
        int text_width = textwidth(wtext);  // 获取文本宽度

        // 获取文本高度（EasyX特殊方式）
        int text_height = lf.lfHeight < 0 ? -lf.lfHeight : lf.lfHeight;

        // 4. 计算居中位置
        int text_x = x + (width - text_width) / 2;
        int text_y = y + (height - text_height) / 2;

        // 5. 绘制文本
        settextcolor(text_color);
        setbkmode(TRANSPARENT);
        outtextxy(text_x, text_y, wtext);
    }
    }


// Setters
void Button::set_position(int x, int y)
{
    this->x = x;
    this->y = y;
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

void Button::set_background_color(COLORREF color)
{
    bg_color = color;
}

void Button::set_hover_color(COLORREF color)
{
    hover_color = color;
}

void Button::set_click_color(COLORREF color)
{
    click_color = color;
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

int Button::get_x() const
{
    return x;
}

int Button::get_y() const
{
    return y;
}

int Button::get_width() const
{
    return width;
}

int Button::get_height() const
{
    return height;
}
#pragma once
#include "scene.h"
#include "util.h"
#include "button.h"

class LevelScene : public Scene
{
public:
    LevelScene();
    ~LevelScene();

    void on_update(float delta) override;
    void on_input(const ExMessage& msg) override;
    void on_enter() override;
    void on_exit() override;
    void on_render(const Camera& camera) override;

private:
    IMAGE* img_background = nullptr;
    Button* button_level1 = nullptr;
    Button* button_level2 = nullptr;
    Button* button_level3 = nullptr;
    Button* button_level4 = nullptr;
    Button* button_level5 = nullptr;
    Button* button_level6 = nullptr;
    Button* button_level_random = nullptr;
    Button* button_level_diy = nullptr;

    int spacing ;       //°´Å¥¼ä¾à
};
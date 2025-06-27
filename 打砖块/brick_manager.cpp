#include "brick_manager.h"
#include "score_manager.h"
#include <random>
#include "scene_manager.h"

BrickManager* BrickManager::manager = nullptr;

BrickManager* BrickManager::instance()
{
    if (manager == nullptr)
    {
        manager = new BrickManager();
    }
    return manager;
}

BrickManager::BrickManager() = default;

BrickManager::~BrickManager()
{
    for (Brick* brick : bricks)
    {
        delete brick;
    }
    bricks.clear();
}

void BrickManager::on_input(const ExMessage& msg)
{
    for (Brick* brick : bricks)
    {
        brick->on_input(msg);
    }
}

void BrickManager::on_update(float delta) {
    // ��������ש��
    for (Brick* brick : bricks) {
        brick->on_update(delta);
    }

    // Ȼ���Ƴ�ש��
    bricks.erase(
        std::remove_if(bricks.begin(), bricks.end(),
            [](Brick* brick) {
                if (!brick->check_is_active()) {
                    ScoreManager::instance()->addPoints(brick->get_points());
                    //printf("%d\n", brick->get_points());
                    delete brick;
                    return true;
                }
                return false;
            }),
        bricks.end());
}

void BrickManager::on_render()
{
    for (Brick* brick : bricks)
    {
        brick->on_render();
    }
}

void BrickManager::fillWithGrid(int screenWidth, int screenHeight, int rows, int cols, int level) {
    clearAllBricks(); // ���������ש��

    // ������ȫ���
    if (rows <= 0 || cols <= 0) return;
    rows =  min(rows, 20); // �����������
    cols =  min(cols, 20); // �����������

    // ���㵥��ש�����ѳߴ磨���Ǽ�ࣩ
    const int horizontalSpacing = 5;  // ˮƽ���
    const int verticalSpacing = 5;    // ��ֱ���

    // ��̬����ש���С��������Ӧ��Ļ
    int brickWidth = (screenWidth - (cols + 1) * horizontalSpacing) / cols;
    int brickHeight = (screenHeight / 2 - (rows + 1) * verticalSpacing) / rows;

    // ȷ��ש�鲻��̫С
    brickWidth =  max(brickWidth, 30);
    brickHeight =  max(brickHeight, 15);

    // ������ʼλ�ã�ʹ������У�
    int totalWidth = cols * brickWidth + (cols - 1) * horizontalSpacing;
    int totalHeight = rows * brickHeight + (rows - 1) * verticalSpacing;
    int startX = (screenWidth - totalWidth) / 2 + brickWidth / 2;
    int startY = 30 + (screenHeight / 2 - 30 - totalHeight) / 2; // ���ϰ�������

    // �����������
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, level - 1);

    // ����ש������
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int x = startX + col * (brickWidth + horizontalSpacing);
            int y = startY + row * (brickHeight + verticalSpacing);

            Brick::Type randomType = static_cast<Brick::Type>(typeDist(gen));
            bricks.push_back(new Brick(x, y, randomType));
        }
    }
}

void BrickManager::fillUpperHalfRandomly_int(int screenWidth, int screenHeight, int level)
{
    const int brickWidth = 70;   // ש����
    const int brickHeight = 40;  // ש��߶�
    const int marginX = 10;      // ˮƽ�߾�
    const int marginY = 10;      // ��ֱ�߾�
    const int startY = 30;       // ��ʼY����
    // �����������
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, level - 1); // 0=Normal, 1=Streng, 2=Powerup

    // ����ÿ�п��Է��ö��ٸ�ש��
    int bricksPerRow = (screenWidth - 2 * marginX) / brickWidth;

    // ������Է��ö�����ש�飨���ϰ�����
    int rows = (screenHeight / 2 - startY - marginY) / brickHeight;

    // ����ʵ��ʹ�õ��ܿ�Ⱥ��ܸ߶ȣ��Ա������ʾ
    int totalWidth = bricksPerRow * brickWidth;
    int startX = (screenWidth - totalWidth) / 2 + brickWidth / 2;

    // �����������е�ש��
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < bricksPerRow; ++col)
        {
            int x = startX + col * brickWidth;
            int y = startY + row * brickHeight;
            // ���ѡ��ש������
            Brick::Type randomType = static_cast<Brick::Type>(typeDist(gen));

            bricks.push_back(new Brick(x, y, randomType));
        }
    }
}

void BrickManager::fillUpperHalfRandomly(int screenWidth, int screenHeight)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(30, screenWidth - 60);
    std::uniform_int_distribution<> yDist(30, screenHeight / 2 - 30);

    const int maxAttempts = 250; // ����Դ���
    int attempts = 0;

    while (attempts < maxAttempts)
    {
        int x = xDist(gen);
        int y = yDist(gen);
        Brick* newBrick = new Brick(x, y);

        if (!isOverlapping(newBrick))
        {
            bricks.push_back(newBrick);
        }
        else
        {
            delete newBrick;
        }

        attempts++;
    }
}

bool BrickManager::isOverlapping(const Brick* newBrick)
{
    for (Brick* existingBrick : bricks)
    {
        // �򵥵ľ����ص����
        int width = newBrick->get_collision_box()->get_size().x;
        int height = newBrick->get_collision_box()->get_size().y;

        int newLeft = newBrick->get_position().x;
        int newRight = newLeft + width;
        int newTop = newBrick->get_position().y;
        int newBottom = newTop + height;

        width = existingBrick->get_collision_box()->get_size().x;
        height = existingBrick->get_collision_box()->get_size().y;

        int existingLeft = existingBrick->get_position().x;
        int existingRight = existingLeft + width;
        int existingTop = existingBrick->get_position().y;
        int existingBottom = existingTop + height;

        if (!(newRight < existingLeft || newLeft > existingRight || newBottom < existingTop || newTop > existingBottom))
        {
            return true;
        }
    }
    return false;
}

void BrickManager::clearAllBricks() {
    for (Brick* brick : bricks) {
        delete brick;
    }

    bricks.clear();

    // SoundManager::instance()->playSound("clear");
}

void BrickManager::rest(int screenWidth, int screenHeight) {
    clearAllBricks();
    fillUpperHalfRandomly_int(screenWidth, screenHeight, SceneManager::instance()->get_current_level());
}


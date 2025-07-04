#include "brick_manager.h"
#include "score_manager.h"
#include <random>
#include "scene_manager.h"
#include <iostream>

#include <fstream>
#include <sstream>
#include <string>

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
    // 更新所有砖块
    for (Brick* brick : bricks) {
        brick->on_update(delta);
    }

    // 然后移除砖块
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

void BrickManager::on_render(const Camera& camera)
{
    for (Brick* brick : bricks)
    {
        brick->on_render(camera);
    }
}

void BrickManager::fillWithGrid(int screenWidth, int screenHeight, int rows, int cols, int level) {
    clearAllBricks(); // 先清除现有砖块

    // 参数安全检查
    if (rows <= 0 || cols <= 0) return;
    rows =  min(rows, 20); // 限制最大行数
    cols =  min(cols, 20); // 限制最大列数

    // 计算单个砖块的最佳尺寸（考虑间距）
    const int horizontalSpacing = 5;  // 水平间距
    const int verticalSpacing = 5;    // 垂直间距

    // 动态计算砖块大小以完美适应屏幕
    int brickWidth = (screenWidth - (cols + 1) * horizontalSpacing) / cols;
    int brickHeight = (screenHeight / 2 - (rows + 1) * verticalSpacing) / rows;

    // 确保砖块不会太小
    brickWidth =  max(brickWidth, 30);
    brickHeight =  max(brickHeight, 15);

    // 计算起始位置（使整体居中）
    int totalWidth = cols * brickWidth + (cols - 1) * horizontalSpacing;
    int totalHeight = rows * brickHeight + (rows - 1) * verticalSpacing;
    int startX = (screenWidth - totalWidth) / 2 + brickWidth / 2;
    int startY = 30 + (screenHeight / 2 - 30 - totalHeight) / 2; // 在上半屏居中

    // 随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, level - 1);

    // 生成砖块网格
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
    const int brickWidth = 70;   // 砖块宽度
    const int brickHeight = 40;  // 砖块高度
    const int marginX = 10;      // 水平边距
    const int marginY = 10;      // 垂直边距
    const int startY = 30;       // 起始Y坐标
    // 随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, level - 1); // 0=Normal, 1=Streng, 2=Powerup

    // 计算每行可以放置多少个砖块
    int bricksPerRow = (screenWidth - 2 * marginX) / brickWidth;

    // 计算可以放置多少行砖块（在上半屏）
    int rows = (screenHeight * 2 / (9-level) - startY - marginY) / brickHeight;

    // 计算实际使用的总宽度和总高度，以便居中显示
    int totalWidth = bricksPerRow * brickWidth;
    int startX = (screenWidth - totalWidth) / 2 + brickWidth / 2;

    // 生成整齐排列的砖块
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < bricksPerRow; ++col)
        {
            int x = startX + col * brickWidth;
            int y = startY + row * brickHeight;
            // 随机选择砖块类型
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

    const int maxAttempts = 250; // 最大尝试次数
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
        // 简单的矩形重叠检测
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



void BrickManager::fillFromCSV(const std::string& csvPath, int screenWidth, int screenHeight)
{
    clearAllBricks();
    cout << "csv enter" << endl;
    std::ifstream file(csvPath);
    if (!file.is_open()) return;
    cout << "csv open" << endl;

    std::vector<std::vector<int>> grid;
    std::string line;
    while (std::getline(file, line)) {
        // 检查并去除UTF-8 BOM
        if (!line.empty() && line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF &&
            static_cast<unsigned char>(line[1]) == 0xBB &&
            static_cast<unsigned char>(line[2]) == 0xBF) {
            line = line.substr(3);
        }

        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            // 去除前后空格
            cell.erase(0, cell.find_first_not_of(" \t\r\n"));
            cell.erase(cell.find_last_not_of(" \t\r\n") + 1);
            //std::cout << "cell: [" << cell << "]" << std::endl; // 调试输出
            if (cell.empty()) continue; // 跳过空单元格
            try {
                row.push_back(std::stoi(cell));
            }
            catch (const std::invalid_argument&) {
                // 非法数据，跳过
                cout << "csv 非法数据，跳过: [" << cell << "]" << endl;
                continue;
            }
        }
        if (!row.empty())
            grid.push_back(row);
    }
    file.close();
    cout << "csv read" << endl;

    int rows = static_cast<int>(grid.size());
    if (rows == 0) return;
    cout << "csv rows" << rows << endl;
    //for (int i = 0; i < grid.size(); ++i) {
    //    std::cout << "row " << i << " size: " << grid[i].size() << std::endl;
    //}
    const int maxCols = 18;
    const int brickWidth = 70;
    const int brickHeight = 40;
    const int spacing = 0;
    const int startX = 45;
    const int startY = 30;

    for (int row = 0; row < grid.size(); ++row) {
        for (int col = 0; col < grid[row].size() && col < maxCols; ++col) {
            int val = grid[row][col];
            if (val == 0) continue; // 空气

            Brick::Type type = Brick::Normal;
            if (val == 1) type = Brick::Normal;
            else if (val == 2) type = Brick::Streng;
            else if (val == 3) type = Brick::Powerup;
            else if (val == 4) type = Brick::Double;
            else if (val == 5) type = Brick::Glass;
            else if (val == 6) type = Brick::More;

            int x = startX + col * (brickWidth + spacing);
            int y = startY + row * (brickHeight + spacing);
            bricks.push_back(new Brick(x, y, type));
        }
    }

}

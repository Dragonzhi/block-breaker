#pragma once
#include <string>

class ScoreManager {
private:
    static ScoreManager* manager;

    int score;
    int highScore;
    std::string highScoreFile;

public:
    static ScoreManager* instance();
    ScoreManager(const std::string& highScoreFilePath = "highscore.txt");
    ~ScoreManager();

    int getScore() const;
    int getHighScore() const;

    void addPoints(int points);
    void resetScore();

    void loadHighScore();
    void saveHighScore();

    std::string getScoreString() const;
    std::string getHighScoreString() const;
};

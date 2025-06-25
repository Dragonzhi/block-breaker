#pragma once


#include <string>

class ScoreManager {
private:
    static ScoreManager* manager;

    int score;
    int highScore;
    std::string highScoreFile;

    void loadHighScore();
    void saveHighScore();

public:
    static ScoreManager* instance();
    ScoreManager(const std::string& highScoreFilePath = "highscore.txt");
    ~ScoreManager();

    int getScore() const;
    int getHighScore() const;

    void addPoints(int points);
    void resetScore();

    std::string getScoreString() const;
    std::string getHighScoreString() const;
};

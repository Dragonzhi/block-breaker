#pragma once
#include <string>
#include <vector>

class ScoreManager {
private:
    static ScoreManager* manager;

    int score;
    std::vector<int> highScores;
    std::string highScoreFile;

public:
    static ScoreManager* instance();
    ScoreManager(const std::string& highScoreFilePath = "highscore.txt");
    ~ScoreManager();

    int getScore() const;
    void setScore(int s);

    int getHighScore(int level) const;
    void updateHighScore(int level, int score);
    void addPoints(int points);
    void resetScore();

    void loadHighScores();
    void saveHighScores();

    int getTotalScore() const {
        int sum = 0;
        for (int s : highScores) sum += s;
        return sum;
    }
    std::string getScoreString() const;
    std::string getHighScoreString(int level) const;
};

#include "score_manager.h"
#include <fstream>
#include <sstream>
#include <iostream>

ScoreManager* ScoreManager::manager = nullptr;

ScoreManager* ScoreManager::instance() {
    if (!manager) {
        manager = new ScoreManager();
    }

    return manager;
}
ScoreManager::ScoreManager(const std::string& highScoreFilePath)
    : score(0), highScores(6, 0), highScoreFile(highScoreFilePath) {
    loadHighScores();
}

ScoreManager::~ScoreManager() {
}

void ScoreManager::loadHighScores() {
    FILE* fp = nullptr;
    errno_t err = fopen_s(&fp, "highscores.dat", "rb");
    if (err == 0 && fp != nullptr) {
        fread(highScores.data(), sizeof(int), highScores.size(), fp);
        fclose(fp);
    }
}

void ScoreManager::saveHighScores() {
    FILE* fp = nullptr;
    errno_t err = fopen_s(&fp, "highscores.dat", "wb");
    if (err == 0 && fp != nullptr) {
        fwrite(highScores.data(), sizeof(int), highScores.size(), fp);
        fclose(fp);
    }
}

int ScoreManager::getScore() const {
    return score;
}

void ScoreManager::setScore(int s) {
    score = s;
}

int ScoreManager::getHighScore(int level) const {
    if (level > 0 && level <= highScores.size())
        return highScores[level - 1];
    return 0;
}

void ScoreManager::addPoints(int points) {
    score += points;
    //if (score > highScore) {
    //    highScore = score;
    //}
}


void ScoreManager::updateHighScore(int level, int score) {
    if (level > 0 && level <= highScores.size() && score > highScores[level - 1]) {
        highScores[level - 1] = score;
        saveHighScores();
    }
}

void ScoreManager::resetScore() {
    score = 0;
}

std::string ScoreManager::getScoreString() const {
    return "Score: " + std::to_string(score);
}

std::string ScoreManager::getHighScoreString(int level) const {
    return "High Score: " + std::to_string(highScores[level-1]);
}
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
    : score(0), highScore(0), highScoreFile(highScoreFilePath) {
    loadHighScore();
}

ScoreManager::~ScoreManager() {
    if (score > highScore) {
        highScore = score;
        saveHighScore();
    }
}

void ScoreManager::loadHighScore() {
    std::ifstream file(highScoreFile);
    if (file.is_open()) {
        std::string line;
        if (std::getline(file, line)) {
            std::istringstream iss(line);
            if (iss >> highScore) {
                return;
            }
        }
        file.close();
    }
    highScore = 0;
}

void ScoreManager::saveHighScore() {
    std::ofstream file(highScoreFile);
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

int ScoreManager::getScore() const {
    return score;
}

int ScoreManager::getHighScore() const {
    return highScore;
}

void ScoreManager::addPoints(int points) {
    score += points;
    if (score > highScore) {
        highScore = score;
    }
}

void ScoreManager::resetScore() {
    score = 0;
}

std::string ScoreManager::getScoreString() const {
    return "Score: " + std::to_string(score);
}

std::string ScoreManager::getHighScoreString() const {
    return "High Score: " + std::to_string(highScore);
}
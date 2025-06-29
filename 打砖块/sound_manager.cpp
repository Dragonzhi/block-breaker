// sound_manager.cpp
#include "sound_manager.h"
#include <iostream>
#include <algorithm>

SoundManager* SoundManager::instance_ = nullptr;

SoundManager::SoundManager() {
    // 初始化音频系统
}

SoundManager::~SoundManager() {
    // 关闭所有MCI设备
    for (auto& soundEntry : soundInstances_) {
        for (auto& soundInstance : soundEntry.second) {
            std::wstringstream cmd;
            cmd << L"close " << soundInstance.alias;
            executeMciCommand(cmd.str(), L"关闭音频设备");
        }
    }
    soundInstances_.clear();
}

bool SoundManager::executeMciCommand(const std::wstring& command, const std::wstring& context) {
    wchar_t errorMsg[256] = { 0 };
    MCIERROR err = mciSendString(command.c_str(), NULL, 0, NULL);

    if (err != 0) {
        mciGetErrorString(err, errorMsg, 256);
        std::wcerr << context << L"失败: " << errorMsg
            << L" (命令: " << command << L")" << std::endl;
        return false;
    }
    return true;
}

SoundManager* SoundManager::instance() {
    if (!instance_) {
        instance_ = new SoundManager();
    }
    return instance_;
}

void SoundManager::loadSound(LPCTSTR path, LPCTSTR id, bool isBGM, int instanceCount) {
    std::lock_guard<std::mutex> lock(soundMutex_);
    if (isBGM) {
        // 背景音乐加载逻辑(单实例)
        std::wstring alias = std::wstring(id) + L"_bgm";
        std::wstringstream openCmd;
        openCmd << L"open \"" << path << L"\" type mpegvideo alias " << alias;

        if (executeMciCommand(openCmd.str(), L"打开背景音乐")) {
            bgmInstances_[id] = { alias, false, 0 };
        }
    }
    else {
        // 音效加载逻辑(多实例)
        if (soundInstances_.find(id) != soundInstances_.end()) {
            std::wcout << L"音频已加载: " << id << std::endl;
            return;
        }

        std::vector<SoundInstance> instances;
        instances.reserve(instanceCount);

        for (int i = 0; i < instanceCount; ++i) {
            std::wstringstream alias;
            alias << id << L"_" << nextInstanceId_++;

            std::wstringstream openCmd;
            openCmd << L"open \"" << path << L"\" type mpegvideo alias " << alias.str();

            if (!executeMciCommand(openCmd.str(), L"打开音频文件")) {
                continue;
            }

            instances.push_back({ alias.str(), false, 0 });
        }

        if (!instances.empty()) {
            soundInstances_[id] = std::move(instances);
            std::wcout << L"加载音频: " << id << L" (" << instances.size() << L"个实例)" << std::endl;
        }
        sfxInstances_[id] = std::move(instances);
    }
}

void SoundManager::playSound(LPCTSTR id, bool isBGM, bool loop, int from) {
    std::lock_guard<std::mutex> lock(soundMutex_);

    if (isBGM) {
        // 背景音乐播放逻辑
        auto it = bgmInstances_.find(id);
        if (it == bgmInstances_.end()) return;

        auto& instance = it->second;
        std::wstringstream cmd;
        cmd << L"play " << instance.alias << (loop ? L" repeat" : L"");
        executeMciCommand(cmd.str(), L"播放背景音乐");
        instance.isPlaying = true;
    }
    else {
        auto it = soundInstances_.find(id);
        if (it == soundInstances_.end()) return;

        DWORD currentTime = GetTickCount();

        // 查找空闲实例
        for (auto& instance : it->second) {
            if (!instance.isPlaying) {
                // 设置播放位置
                std::wstringstream seekCmd;
                seekCmd << L"seek " << instance.alias << L" to " << from;
                executeMciCommand(seekCmd.str(), L"定位音频");

                // 播放音频
                std::wstringstream playCmd;
                playCmd << L"play " << instance.alias;
                if (loop) playCmd << L" repeat";
                executeMciCommand(playCmd.str(), L"播放音频");

                instance.isPlaying = true;
                instance.lastPlayTime = currentTime;
                return;
            }
        }

        // 所有实例都在使用中，强制使用第一个实例重新播放
        if (!it->second.empty()) {
            auto& instance = it->second[0];

            // 停止当前播放
            std::wstringstream stopCmd;
            stopCmd << L"stop " << instance.alias;
            executeMciCommand(stopCmd.str(), L"停止音频");

            // 设置播放位置
            std::wstringstream seekCmd;
            seekCmd << L"seek " << instance.alias << L" to " << from;
            executeMciCommand(seekCmd.str(), L"定位音频");

            // 重新播放
            std::wstringstream playCmd;
            playCmd << L"play " << instance.alias;
            if (loop) playCmd << L" repeat";
            executeMciCommand(playCmd.str(), L"播放音频");

            instance.isPlaying = true;
            instance.lastPlayTime = currentTime;
        }
    }
}

// sound_manager.cpp
void SoundManager::stopSound(LPCTSTR id) {
    std::lock_guard<std::mutex> lock(soundMutex_);

    // 先检查是否是背景音乐
    auto bgmIt = bgmInstances_.find(id);
    if (bgmIt != bgmInstances_.end()) {
        auto& instance = bgmIt->second;
        if (instance.isPlaying) {
            std::wstringstream cmd;
            cmd << L"stop " << instance.alias;
            executeMciCommand(cmd.str(), L"停止背景音乐");
            instance.isPlaying = false;
        }
        return;
    }

    // 如果不是背景音乐，检查音效
    auto sfxIt = soundInstances_.find(id);
    if (sfxIt == soundInstances_.end()) return;

    for (auto& instance : sfxIt->second) {
        if (instance.isPlaying) {
            std::wstringstream cmd;
            cmd << L"stop " << instance.alias;
            executeMciCommand(cmd.str(), L"停止音效");
            instance.isPlaying = false;
        }
    }
}

void SoundManager::stopAll() {
    std::lock_guard<std::mutex> lock(soundMutex_);

    // 停止所有背景音乐
    for (auto& bgm : bgmInstances_) {
        if (bgm.second.isPlaying) {
            std::wstringstream cmd;
            cmd << L"stop " << bgm.second.alias;
            executeMciCommand(cmd.str(), L"停止背景音乐");
            bgm.second.isPlaying = false;
        }
    }

    // 停止所有音效
    for (auto& soundEntry : soundInstances_) {
        for (auto& instance : soundEntry.second) {
            if (instance.isPlaying) {
                std::wstringstream cmd;
                cmd << L"stop " << instance.alias;
                executeMciCommand(cmd.str(), L"停止音效");
                instance.isPlaying = false;
            }
        }
    }
}

void SoundManager::setVolume(LPCTSTR id, int volume) {
    std::lock_guard<std::mutex> lock(soundMutex_);

    auto it = soundInstances_.find(id);
    if (it == soundInstances_.end()) return;

    // 为所有实例设置音量
    for (auto& instance : it->second) {
        std::wstringstream cmd;
        cmd << L"setaudio " << instance.alias << L" volume to " << volume;
        executeMciCommand(cmd.str(), L"设置音量");
    }
}

void SoundManager::update() {
    std::lock_guard<std::mutex> lock(soundMutex_);

    DWORD currentTime = GetTickCount();

    for (auto& soundEntry : soundInstances_) {
        for (auto& instance : soundEntry.second) {
            if (!instance.isPlaying) continue;

            // 检查播放状态
            wchar_t status[32] = { 0 };
            std::wstringstream cmd;
            cmd << L"status " << instance.alias << L" mode";
            mciSendString(cmd.str().c_str(), status, 32, NULL);

            // 如果状态不是播放中，则标记为停止
            if (wcscmp(status, L"playing") != 0) {
                instance.isPlaying = false;
            }
            // 超时处理（5秒）
            else if (currentTime - instance.lastPlayTime > 5000) {
                std::wstringstream stopCmd;
                stopCmd << L"stop " << instance.alias;
                executeMciCommand(stopCmd.str(), L"停止超时音频");
                instance.isPlaying = false;
            }
        }
    }
}
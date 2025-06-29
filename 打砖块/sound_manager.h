// sound_manager.h
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include <mmsystem.h>
#include <mutex>
#include <sstream>

#pragma comment(lib, "winmm.lib")

class SoundManager {
private:
    struct SoundInstance {
        std::wstring alias;      // MCI别名
        bool isPlaying = false;   // 播放状态
        DWORD lastPlayTime = 0;   // 最后播放时间
    };

private:
    static SoundManager* instance_;
    std::unordered_map<std::wstring, std::vector<SoundInstance>> soundInstances_;
    std::mutex soundMutex_;
    int nextInstanceId_ = 0;  // 实例ID计数器
    std::unordered_map<std::wstring, SoundInstance> bgmInstances_;  // 背景音乐(单实例)
    std::unordered_map<std::wstring, std::vector<SoundInstance>> sfxInstances_; // 音效(多实例)

    SoundManager();
    ~SoundManager();

    // 执行MCI命令并检查错误
    bool executeMciCommand(const std::wstring& command, const std::wstring& context);

public:
    static SoundManager* instance();

    // 加载音频资源
    void loadSound(LPCTSTR path, LPCTSTR id, bool isBGM = false, int instanceCount = 3);

    // 播放音频
    void playSound(LPCTSTR id, bool isBGM = false, bool loop = false, int from = 0);

    // 停止指定音频
    void stopSound(LPCTSTR id);

    // 停止所有音频
    void stopAll();

    // 设置音量 (0-1000)
    void setVolume(LPCTSTR id, int volume);

    // 更新状态
    void update();
};
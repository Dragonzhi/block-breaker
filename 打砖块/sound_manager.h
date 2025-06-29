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
        std::wstring alias;      // MCI����
        bool isPlaying = false;   // ����״̬
        DWORD lastPlayTime = 0;   // ��󲥷�ʱ��
    };

private:
    static SoundManager* instance_;
    std::unordered_map<std::wstring, std::vector<SoundInstance>> soundInstances_;
    std::mutex soundMutex_;
    int nextInstanceId_ = 0;  // ʵ��ID������
    std::unordered_map<std::wstring, SoundInstance> bgmInstances_;  // ��������(��ʵ��)
    std::unordered_map<std::wstring, std::vector<SoundInstance>> sfxInstances_; // ��Ч(��ʵ��)

    SoundManager();
    ~SoundManager();

    // ִ��MCI���������
    bool executeMciCommand(const std::wstring& command, const std::wstring& context);

public:
    static SoundManager* instance();

    // ������Ƶ��Դ
    void loadSound(LPCTSTR path, LPCTSTR id, bool isBGM = false, int instanceCount = 3);

    // ������Ƶ
    void playSound(LPCTSTR id, bool isBGM = false, bool loop = false, int from = 0);

    // ָֹͣ����Ƶ
    void stopSound(LPCTSTR id);

    // ֹͣ������Ƶ
    void stopAll();

    // �������� (0-1000)
    void setVolume(LPCTSTR id, int volume);

    // ����״̬
    void update();
};
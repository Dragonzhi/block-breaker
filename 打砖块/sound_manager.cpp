// sound_manager.cpp
#include "sound_manager.h"
#include <iostream>
#include <algorithm>

SoundManager* SoundManager::instance_ = nullptr;

SoundManager::SoundManager() {
    // ��ʼ����Ƶϵͳ
}

SoundManager::~SoundManager() {
    // �ر�����MCI�豸
    for (auto& soundEntry : soundInstances_) {
        for (auto& soundInstance : soundEntry.second) {
            std::wstringstream cmd;
            cmd << L"close " << soundInstance.alias;
            executeMciCommand(cmd.str(), L"�ر���Ƶ�豸");
        }
    }
    soundInstances_.clear();
}

bool SoundManager::executeMciCommand(const std::wstring& command, const std::wstring& context) {
    wchar_t errorMsg[256] = { 0 };
    MCIERROR err = mciSendString(command.c_str(), NULL, 0, NULL);

    if (err != 0) {
        mciGetErrorString(err, errorMsg, 256);
        std::wcerr << context << L"ʧ��: " << errorMsg
            << L" (����: " << command << L")" << std::endl;
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
        // �������ּ����߼�(��ʵ��)
        std::wstring alias = std::wstring(id) + L"_bgm";
        std::wstringstream openCmd;
        openCmd << L"open \"" << path << L"\" type mpegvideo alias " << alias;

        if (executeMciCommand(openCmd.str(), L"�򿪱�������")) {
            bgmInstances_[id] = { alias, false, 0 };
        }
    }
    else {
        // ��Ч�����߼�(��ʵ��)
        if (soundInstances_.find(id) != soundInstances_.end()) {
            std::wcout << L"��Ƶ�Ѽ���: " << id << std::endl;
            return;
        }

        std::vector<SoundInstance> instances;
        instances.reserve(instanceCount);

        for (int i = 0; i < instanceCount; ++i) {
            std::wstringstream alias;
            alias << id << L"_" << nextInstanceId_++;

            std::wstringstream openCmd;
            openCmd << L"open \"" << path << L"\" type mpegvideo alias " << alias.str();

            if (!executeMciCommand(openCmd.str(), L"����Ƶ�ļ�")) {
                continue;
            }

            instances.push_back({ alias.str(), false, 0 });
        }

        if (!instances.empty()) {
            soundInstances_[id] = std::move(instances);
            std::wcout << L"������Ƶ: " << id << L" (" << instances.size() << L"��ʵ��)" << std::endl;
        }
        sfxInstances_[id] = std::move(instances);
    }
}

void SoundManager::playSound(LPCTSTR id, bool isBGM, bool loop, int from) {
    std::lock_guard<std::mutex> lock(soundMutex_);

    if (isBGM) {
        // �������ֲ����߼�
        auto it = bgmInstances_.find(id);
        if (it == bgmInstances_.end()) return;

        auto& instance = it->second;
        std::wstringstream cmd;
        cmd << L"play " << instance.alias << (loop ? L" repeat" : L"");
        executeMciCommand(cmd.str(), L"���ű�������");
        instance.isPlaying = true;
    }
    else {
        auto it = soundInstances_.find(id);
        if (it == soundInstances_.end()) return;

        DWORD currentTime = GetTickCount();

        // ���ҿ���ʵ��
        for (auto& instance : it->second) {
            if (!instance.isPlaying) {
                // ���ò���λ��
                std::wstringstream seekCmd;
                seekCmd << L"seek " << instance.alias << L" to " << from;
                executeMciCommand(seekCmd.str(), L"��λ��Ƶ");

                // ������Ƶ
                std::wstringstream playCmd;
                playCmd << L"play " << instance.alias;
                if (loop) playCmd << L" repeat";
                executeMciCommand(playCmd.str(), L"������Ƶ");

                instance.isPlaying = true;
                instance.lastPlayTime = currentTime;
                return;
            }
        }

        // ����ʵ������ʹ���У�ǿ��ʹ�õ�һ��ʵ�����²���
        if (!it->second.empty()) {
            auto& instance = it->second[0];

            // ֹͣ��ǰ����
            std::wstringstream stopCmd;
            stopCmd << L"stop " << instance.alias;
            executeMciCommand(stopCmd.str(), L"ֹͣ��Ƶ");

            // ���ò���λ��
            std::wstringstream seekCmd;
            seekCmd << L"seek " << instance.alias << L" to " << from;
            executeMciCommand(seekCmd.str(), L"��λ��Ƶ");

            // ���²���
            std::wstringstream playCmd;
            playCmd << L"play " << instance.alias;
            if (loop) playCmd << L" repeat";
            executeMciCommand(playCmd.str(), L"������Ƶ");

            instance.isPlaying = true;
            instance.lastPlayTime = currentTime;
        }
    }
}

// sound_manager.cpp
void SoundManager::stopSound(LPCTSTR id) {
    std::lock_guard<std::mutex> lock(soundMutex_);

    // �ȼ���Ƿ��Ǳ�������
    auto bgmIt = bgmInstances_.find(id);
    if (bgmIt != bgmInstances_.end()) {
        auto& instance = bgmIt->second;
        if (instance.isPlaying) {
            std::wstringstream cmd;
            cmd << L"stop " << instance.alias;
            executeMciCommand(cmd.str(), L"ֹͣ��������");
            instance.isPlaying = false;
        }
        return;
    }

    // ������Ǳ������֣������Ч
    auto sfxIt = soundInstances_.find(id);
    if (sfxIt == soundInstances_.end()) return;

    for (auto& instance : sfxIt->second) {
        if (instance.isPlaying) {
            std::wstringstream cmd;
            cmd << L"stop " << instance.alias;
            executeMciCommand(cmd.str(), L"ֹͣ��Ч");
            instance.isPlaying = false;
        }
    }
}

void SoundManager::stopAll() {
    std::lock_guard<std::mutex> lock(soundMutex_);

    // ֹͣ���б�������
    for (auto& bgm : bgmInstances_) {
        if (bgm.second.isPlaying) {
            std::wstringstream cmd;
            cmd << L"stop " << bgm.second.alias;
            executeMciCommand(cmd.str(), L"ֹͣ��������");
            bgm.second.isPlaying = false;
        }
    }

    // ֹͣ������Ч
    for (auto& soundEntry : soundInstances_) {
        for (auto& instance : soundEntry.second) {
            if (instance.isPlaying) {
                std::wstringstream cmd;
                cmd << L"stop " << instance.alias;
                executeMciCommand(cmd.str(), L"ֹͣ��Ч");
                instance.isPlaying = false;
            }
        }
    }
}

void SoundManager::setVolume(LPCTSTR id, int volume) {
    std::lock_guard<std::mutex> lock(soundMutex_);

    auto it = soundInstances_.find(id);
    if (it == soundInstances_.end()) return;

    // Ϊ����ʵ����������
    for (auto& instance : it->second) {
        std::wstringstream cmd;
        cmd << L"setaudio " << instance.alias << L" volume to " << volume;
        executeMciCommand(cmd.str(), L"��������");
    }
}

void SoundManager::update() {
    std::lock_guard<std::mutex> lock(soundMutex_);

    DWORD currentTime = GetTickCount();

    for (auto& soundEntry : soundInstances_) {
        for (auto& instance : soundEntry.second) {
            if (!instance.isPlaying) continue;

            // ��鲥��״̬
            wchar_t status[32] = { 0 };
            std::wstringstream cmd;
            cmd << L"status " << instance.alias << L" mode";
            mciSendString(cmd.str().c_str(), status, 32, NULL);

            // ���״̬���ǲ����У�����Ϊֹͣ
            if (wcscmp(status, L"playing") != 0) {
                instance.isPlaying = false;
            }
            // ��ʱ����5�룩
            else if (currentTime - instance.lastPlayTime > 5000) {
                std::wstringstream stopCmd;
                stopCmd << L"stop " << instance.alias;
                executeMciCommand(stopCmd.str(), L"ֹͣ��ʱ��Ƶ");
                instance.isPlaying = false;
            }
        }
    }
}
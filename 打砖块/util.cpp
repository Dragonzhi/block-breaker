#include "util.h"

// 异步播放音频的函数
void async_play_audio(LPCTSTR id, bool is_loop) {
    TCHAR str_cmd[512];
    TCHAR status[256] = { 0 };  // 确保初始化为空

    // 获取音频状态
    _stprintf_s(str_cmd, _T("status %s mode"), id);
    MCIERROR error = mciSendString(str_cmd, status, 255, NULL);  // 留一个字符给终止符

    if (error != 0) {
        // 增强错误处理
        std::wcerr << L"状态检查失败: " << id;

        TCHAR errMsg[256] = { 0 };
        if (mciGetErrorString(error, errMsg, 256)) {
            std::wcerr << L" | MCI错误: " << errMsg;
        }
        else {
            std::wcerr << L" | 错误代码: " << error;
        }
        std::wcerr << std::endl;
        return;
    }

    // 检查状态是否为可播放状态
    if (_tcscmp(status, _T("stopped")) != 0 &&
        _tcscmp(status, _T("paused")) != 0)
    {
        std::wcerr << L"无效音频状态: " << id
            << L" | 当前状态: " << status << std::endl;
        return;
    }

    // 播放命令
    _stprintf_s(str_cmd, _T("play %s %s from 0"),
        id, is_loop ? _T("repeat") : _T(""));

    error = mciSendString(str_cmd, NULL, 0, NULL);
    if (error != 0) {
        std::wcerr << L"播放失败: " << id;

        TCHAR errMsg[256] = { 0 };
        if (mciGetErrorString(error, errMsg, 256)) {
            std::wcerr << L" | MCI错误: " << errMsg;
        }
        else {
            std::wcerr << L" | 错误代码: " << error;
        }
        std::wcerr << std::endl;
    }
}
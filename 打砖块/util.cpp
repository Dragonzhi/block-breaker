#include "util.h"

// �첽������Ƶ�ĺ���
void async_play_audio(LPCTSTR id, bool is_loop) {
    TCHAR str_cmd[512];
    TCHAR status[256] = { 0 };  // ȷ����ʼ��Ϊ��

    // ��ȡ��Ƶ״̬
    _stprintf_s(str_cmd, _T("status %s mode"), id);
    MCIERROR error = mciSendString(str_cmd, status, 255, NULL);  // ��һ���ַ�����ֹ��

    if (error != 0) {
        // ��ǿ������
        std::wcerr << L"״̬���ʧ��: " << id;

        TCHAR errMsg[256] = { 0 };
        if (mciGetErrorString(error, errMsg, 256)) {
            std::wcerr << L" | MCI����: " << errMsg;
        }
        else {
            std::wcerr << L" | �������: " << error;
        }
        std::wcerr << std::endl;
        return;
    }

    // ���״̬�Ƿ�Ϊ�ɲ���״̬
    if (_tcscmp(status, _T("stopped")) != 0 &&
        _tcscmp(status, _T("paused")) != 0)
    {
        std::wcerr << L"��Ч��Ƶ״̬: " << id
            << L" | ��ǰ״̬: " << status << std::endl;
        return;
    }

    // ��������
    _stprintf_s(str_cmd, _T("play %s %s from 0"),
        id, is_loop ? _T("repeat") : _T(""));

    error = mciSendString(str_cmd, NULL, 0, NULL);
    if (error != 0) {
        std::wcerr << L"����ʧ��: " << id;

        TCHAR errMsg[256] = { 0 };
        if (mciGetErrorString(error, errMsg, 256)) {
            std::wcerr << L" | MCI����: " << errMsg;
        }
        else {
            std::wcerr << L" | �������: " << error;
        }
        std::wcerr << std::endl;
    }
}
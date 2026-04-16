> This document is translated by AI. For the original Chinese version, please refer to the official Chinese documentation.

# macdowsOS Tool UI User Guide

-----------------------------------------------------------------------------------------

## 1. Installation and Running
1. First, download `release.7z` or `release.zip` from the release page.
2. Then extract the file using Windows built-in extraction option or a third-party decompression tool (we recommend using [7-zip](https://www.7-zip.org/)) in File Explorer.
3. After extraction, locate `macdowsOS Tool UI.exe` in the unzipped folder, right-click it, and select **Run as administrator**.

-----------------------------------------------------------------------------------------

## 2. Usage
### 2.1 Precautions
1. You <span style="color:#e60000;">must</span> run the program as administrator. Although macdowsOS Tool does not check the current running privileges and can still launch without administrator rights, when the tool modifies certain system files, it <span style="color:#e60000;">may</span> cause system file corruption due to insufficient permissions.
2. The automatic operation technology used for installing some components of macdowsOS Tool may be recognized as malicious by some antivirus software. It is recommended to exit or disable all antivirus software before installation.
3. The author does not guarantee that macdowsOS Tool will not cause any system issues. The restore point component of the tool has been disabled due to critical bugs. It is <span style="color:#e60000;">strongly recommended</span> that you create a system restore point and back up your system files before installation.

-----------------------------------------------------------------------------------------

### 2.2 Windows to macOS Simulation Function
macdowsOS Tool requires the following resources to implement this function. <span style="color:#e60000;">Attention! The author does not hold the copyright of these resources. All resource packages are collected from the Internet. The author is not responsible for the legality of any third-party files used by users. The inclusion and use of these resource packages in this program are solely for learning and communication about Windows automatic operation principles and C programming. The author assumes no liability for any consequences arising therefrom.</span> If this program infringes your rights and interests, please send an email to aiboqiao250@outlook.com. I will verify it as soon as possible, and the relevant content will be deleted after verification.

| Component Name | Source | Author | Modified |
|--|--|--|--|
| Icons | www.deviantart.com | niivu | No |
| MyDockFinder | www.mydockfinder.com | №じ 尐乄鈊~→☆ | Yes |
| OldNewExplorer | zhutix.com | Tihiy | No |
| StartAllBack | startallback.cn | StartAllBack | No |
| Theme | www.deviantart.com | niivu | Yes |

Possible issues with this function

-----------------------------------------------------------------------------------------

### 2.3 System Repair Function
macdowsOS Tool executes system repair via the command line. The commands are sourced from [Huawei](https://consumer.huawei.com/cn/support/content/zh-cn15906534/).

-----------------------------------------------------------------------------------------

### 2.4 Window Rounding Function
The current window rounding function uses Windows API to create a rounded corner area to clip the window, resulting in limited compatibility. This feature will be optimized in subsequent updates.

The usage instructions are detailed in the configuration file and will not be repeated here.

Possible issues with this function:
> Q. What should I do if the modification does not take effect?
>
> A. Please return to the main interface of macdowsOS Tool to apply the modification.

-----------------------------------------------------------------------------------------

## 3. Troubleshooting
### 3.1 Before Installation
#### <span style="color:#66a3e0;">1. After clicking through the disclaimer and other pop-ups, the following or similar errors occur when launching the main program of macdowsOS Tool</span>

`The program can't start because XXXXX.dll is missing from your computer. Try reinstalling the program to fix this problem.`

`The application has failed to start because its side-by-side configuration is incorrect. Please see the application event log or use the command-line sxstrace.exe tool for more detail.`

`The application failed to initialize properly. Click OK to terminate the application.`

`The application has encountered an unknown software exception.`

`Exception code 0xC0000005: ACCESS_VIOLATION`

`The memory could not be "read".`

`The memory could not be "written".`

#### **Solution:** Install or repair the Microsoft Visual C++ Redistributable Package
[Download Link](https://learn.microsoft.com/zh-cn/cpp/windows/latest-supported-vc-redist?view=msvc-170)

#### <span style="color:#66a3e0;">2. The following or similar errors occur when running macdowsOS Tool</span>

`The code execution cannot proceed because sfml-xxxx.dll was not found. Reinstalling the program may fix this problem.`

#### **Solution:** The graphics dependencies of macdowsOS Tool are corrupted or missing. Please re-download the complete macdowsOS Tool installation package.

### 3.2 During Installation
#### <span style="color:#66a3e0;">1. The progress bar gets stuck for a long time during installation (usually no movement for more than 3 minutes), without any error or pop-up prompt, and the program is unresponsive when clicking the window</span>

#### **Solution:** Send the `log.txt` file in the macdowsOS Tool directory to the author for feedback (or resolve the issue yourself if you can understand the log content).

#### <span style="color:#66a3e0;">2. Any other errors occur</span>
#### **Solution:** Send a screenshot of the error to the author for targeted troubleshooting guidance.

### 3.3 After Installation
#### <span style="color:#66a3e0;">1. File Explorer restarts infinitely after installation, with a line of white text on a black background displayed on the screen during the process</span>
#### **Solution:** This issue occurs because some components installed by macdowsOS Tool are incompatible with your system. Press `Win+I` to open Settings, and uninstall the program named **StartAllBack**.

#### <span style="color:#66a3e0;">2. The login screen flashes after installation / File Explorer restarts infinitely after entering the desktop (no prompt)</span>
#### **Solution:** This issue occurs because the theme installed by macdowsOS Tool is incompatible with your system. Press `Win+I` to open Settings, and switch the theme to the Windows default theme. (If you are stuck on the login screen, it is recommended to restore the system using a previously created restore point.)

#### <span style="color:#66a3e0;">3. All icons turn black/transparent after installation</span>
#### **Solution:** This issue occurs because the operation was interrupted or blocked when macdowsOS Tool was modifying system files, resulting in system file corruption. Use the built-in system repair function of macdowsOS Tool to fix it.

#### <span style="color:#66a3e0;">4. Desktop files are hidden after installation (this is normal behavior), but I am not used to this. How to restore it?</span>
#### **Solution:** Right-click on the desktop → View → Show desktop icons.

#### <span style="color:#66a3e0;">5. I want to change the wallpaper after installation, but it reverts to the macOS wallpaper after a while. What should I do?</span>
#### **Solution:** Right-click the Dock bar to open MyDockFinder Preferences → Wallpaper, set the wallpaper option to None, then set your custom wallpaper.

#### <span style="color:#66a3e0;">6. The Dock bar/Finder bar is too large/too small. What should I do?</span>
#### **Solution:** Right-click the Dock bar to open MyDockFinder Preferences → General → Global Interface Size, this option adjusts the Finder bar. Go to Preferences → Dock → General → Zoom Size, this option adjusts the Dock bar size.

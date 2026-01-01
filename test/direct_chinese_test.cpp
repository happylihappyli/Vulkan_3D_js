// 直接中文输出测试程序
// UTF-8编码支持
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

// UTF-8控制台输出支持函数
void setConsoleUTF8() {
    // 启用控制台UTF-8支持
    SetConsoleOutputCP(65001); // 设置控制台输出为UTF-8
    SetConsoleCP(65001);       // 设置控制台输入为UTF-8
    
    // 设置C++标准库的本地化设置
    setlocale(LC_ALL, "C.UTF-8"); // 使用UTF-8本地化
    
    // 设置控制台字体为支持UTF-8的字体
    CONSOLE_FONT_INFOEX fontInfo = {0};
    fontInfo.cbSize = sizeof(fontInfo);
    fontInfo.FontWeight = FW_NORMAL;
    fontInfo.dwFontSize.X = 12;
    fontInfo.dwFontSize.Y = 16;
    wcscpy_s(fontInfo.FaceName, L"Consolas"); // 使用Consolas字体支持中文
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE) {
        SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    }
    
    // 额外的控制台配置以确保UTF-8输出
    HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // 设置控制台模式
    DWORD mode;
    if (GetConsoleMode(hStdOutput, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hStdOutput, mode);
    }
    
    // 输出测试信息验证编码 - 统一使用宽字符输出
    wprintf(L"控制台已设置为UTF-8编码\n");
    wprintf(L"中文测试: 控制台已成功设置为UTF-8编码\n");
}

int main() {
    // 设置UTF-8编码支持
    setConsoleUTF8();
    
    // 创建Console窗口用于显示信息
    AllocConsole();
    FILE* console_out;
    freopen_s(&console_out, "CONOUT$", "w", stdout);
    freopen_s(&console_out, "CONIN$", "r", stdin);
    
    wprintf(L"=== 中文输出测试程序 ===\n");
    wprintf(L"构建时间: %hs %hs\n", __DATE__, __TIME__);
    wprintf(L"测试目的: 验证中文显示效果\n");
    wprintf(L"===========================================\n");
    
    // 测试各种中文输出
    wprintf(L"\n=== 基础中文测试 ===\n");
    wprintf(L"中文测试: 控制台中文显示测试\n");
    wprintf(L"鼠标移动: 位置(100,200), 偏移(5.5,3.2)\n");
    wprintf(L"相机旋转: 水平角=1.57 俯仰角=0.78\n");
    wprintf(L"缩放: 距离=25.5\n");
    
    wprintf(L"\n=== 点云相关中文测试 ===\n");
    wprintf(L"点云大小: 101 x 101 x 101 = 1,030,301 个点\n");
    wprintf(L"相机距离: 8.0\n");
    wprintf(L"操作说明:\n");
    wprintf(L"  鼠标左键拖动: 旋转视角\n");
    wprintf(L"  鼠标滚轮: 缩放视图\n");
    wprintf(L"  ESC键: 退出程序\n");
    
    wprintf(L"\n=== 性能信息测试 ===\n");
    wprintf(L"FPS: 60.5 | 帧数: 1000\n");
    wprintf(L"程序退出，共渲染 5000 帧\n");
    
    wprintf(L"\n=== 测试完成 ===\n");
    wprintf(L"请检查控制台中的中文是否显示正确\n");
    wprintf(L"如果没有乱码，说明UTF-8编码设置成功\n");
    
    // 等待用户输入
    wprintf(L"\n按任意键退出...\n");
    getchar();
    
    // 关闭Console
    FreeConsole();
    
    return 0;
}
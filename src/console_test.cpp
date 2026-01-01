// UTF-8编码测试程序
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <locale>
#include <io.h>
#include <fcntl.h>

// UTF-8控制台输出支持函数
void setConsoleUTF8() {
    // 创建Console窗口
    AllocConsole();
    
    // 重定向标准输出和错误输出
    FILE* console_out;
    freopen_s(&console_out, "CONOUT$", "w", stdout);
    freopen_s(&console_out, "CONIN$", "r", stdin);
    
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
    
    // 额外的控制台配置
    HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(hStdOutput, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hStdOutput, mode);
    }
    
    // 输出测试信息验证编码
    printf("控制台已设置为UTF-8编码\n");
    
    // 验证中文字符输出
    wprintf(L"中文测试: 控制台已成功设置为UTF-8编码\n");
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 设置UTF-8编码支持
    setConsoleUTF8();
    
    std::cout << "=== UTF-8编码控制台测试程序 ===" << std::endl;
    std::cout << "构建时间: " << __DATE__ << " " << __TIME__ << std::endl;
    std::cout << "程序标题: UTF-8 Console Test" << std::endl;
    std::cout << std::endl;
    
    // 测试中文输出
    std::cout << "中文测试内容:" << std::endl;
    std::cout << "  你好世界！" << std::endl;
    std::cout << "  这是中文显示测试" << std::endl;
    std::cout << "  点云查看器 - UTF-8版本" << std::endl;
    std::cout << std::endl;
    
    // 测试Unicode字符
    std::cout << "Unicode字符测试:" << std::endl;
    std::cout << "  ✓ 检查标记" << std::endl;
    std::cout << "  ✗ 叉号标记" << std::endl;
    std::cout << "  → 箭头标记" << std::endl;
    std::cout << std::endl;
    
    // 显示控制台设置信息
    std::cout << "控制台设置:" << std::endl;
    std::cout << "  代码页: " << GetConsoleOutputCP() << std::endl;
    std::cout << "  输入代码页: " << GetConsoleCP() << std::endl;
    std::cout << std::endl;
    
    std::cout << "程序运行成功！" << std::endl;
    std::cout << "按任意键退出..." << std::endl;
    
    // 等待用户输入
    getchar();
    
    // 关闭Console
    FreeConsole();
    
    return 0;
}
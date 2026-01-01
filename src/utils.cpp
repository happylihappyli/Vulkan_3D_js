#include "utils.h"
#include <iostream>
#include <fstream>
#include <locale>
#include <io.h>
#include <fcntl.h>

// UTF-8控制台输出支持函数
void setConsoleUTF8() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    setlocale(LC_ALL, "C.UTF-8");
    
    CONSOLE_FONT_INFOEX fontInfo = {0};
    fontInfo.cbSize = sizeof(fontInfo);
    fontInfo.FontWeight = FW_NORMAL;
    fontInfo.dwFontSize.X = 12;
    fontInfo.dwFontSize.Y = 16;
    wcscpy_s(fontInfo.FaceName, L"Consolas");
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE) {
        SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    }
    
    HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    
    DWORD mode;
    if (GetConsoleMode(hStdOutput, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hStdOutput, mode);
    }
    
    wprintf(L"控制台已设置为UTF-8编码\n");
    wprintf(L"中文测试: 控制台已成功设置为UTF-8编码\n");
    
    logToFile(L"控制台已设置为UTF-8编码");
    logToFile(L"中文测试: 控制台已成功设置为UTF-8编码");
}

// 文件日志记录函数
void logToFile(const std::wstring& message) {
    std::ofstream logFile("airplane_pointcloud_log.txt", std::ios::app | std::ios::binary);
    if (logFile.is_open()) {
        logFile.seekp(0, std::ios::end);
        if (logFile.tellp() == 0) {
            const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
            logFile.write(reinterpret_cast<const char*>(bom), sizeof(bom));
        }
        
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, message.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string narrowMessage(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, message.c_str(), -1, &narrowMessage[0], size_needed, nullptr, nullptr);
        
        logFile << narrowMessage.c_str() << std::endl;
        logFile.close();
    }
}

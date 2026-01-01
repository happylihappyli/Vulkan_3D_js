#pragma once

#include <string>
#include <windows.h>

// UTF-8控制台输出支持函数
void setConsoleUTF8();

// 文件日志记录函数
void logToFile(const std::wstring& message);

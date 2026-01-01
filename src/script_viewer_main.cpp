#include "script_viewer_app.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    ScriptViewer::ScriptViewerApp app;
    
    std::cout << "=== JavaScript脚本查看器 ===" << std::endl;
    std::cout << "启动应用程序..." << std::endl;
    
    std::string scriptPath;
    if (argc > 1) {
        scriptPath = argv[1];
        std::cout << "加载脚本文件: " << scriptPath << std::endl;
    } else {
        scriptPath = "test/test_model.js";
        std::cout << "自动加载测试脚本: " << scriptPath << std::endl;
    }
    
    app.setInitialScript(scriptPath);
    
    int result = app.run();
    
    if (result == 0) {
        std::cout << "应用程序正常退出" << std::endl;
    } else {
        std::cout << "应用程序异常退出，错误代码: " << result << std::endl;
    }
    
    return result;
}

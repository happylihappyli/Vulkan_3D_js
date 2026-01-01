// QuickJS 引擎包装器实现
// 提供 C++ 友好的 JavaScript 执行接口

#include "quickjs_engine.h"
#include <iostream>
#include <fstream>
#include <sstream>

// 包含 QuickJS 头文件
#include "../quickjs/quickjs.h"
#include "../quickjs/quickjs-libc.h"

// QuickJSEngine 类实现
QuickJSEngine::QuickJSEngine() 
    : m_runtime(nullptr), m_context(nullptr), m_initialized(false) {
}

QuickJSEngine::~QuickJSEngine() {
    cleanup();
}

// 初始化 QuickJS 运行时和上下文
bool QuickJSEngine::initialize() {
    // 创建 QuickJS 运行时
    m_runtime = JS_NewRuntime();
    if (!m_runtime) {
        std::cerr << "无法创建 QuickJS 运行时" << std::endl;
        return false;
    }
    
    // 创建 QuickJS 上下文
    m_context = JS_NewContext(m_runtime);
    if (!m_context) {
        std::cerr << "无法创建 QuickJS 上下文" << std::endl;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    
    // 初始化标准库
    js_std_init_handlers(m_runtime);
    js_std_add_helpers(m_context, -1, nullptr);
    
    m_initialized = true;
    std::cout << "QuickJS 引擎初始化成功" << std::endl;
    return true;
}

// 清理 QuickJS 资源
void QuickJSEngine::cleanup() {
    if (m_context) {
        JS_FreeContext(m_context);
        m_context = nullptr;
    }
    
    if (m_runtime) {
        js_std_free_handlers(m_runtime);
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
    }
    
    m_initialized = false;
}

// 初始化引擎
bool QuickJSEngine::init() {
    if (m_initialized) {
        std::cout << "QuickJS 引擎已经初始化" << std::endl;
        return true;
    }
    
    return initialize();
}

// 执行 JavaScript 代码
bool QuickJSEngine::executeCode(const std::string& code) {
    if (!m_initialized) {
        std::cerr << "QuickJS 引擎未初始化" << std::endl;
        return false;
    }
    
    // 编译并执行 JavaScript 代码
    JSValue val = JS_Eval(m_context, code.c_str(), code.length(), "<eval>", JS_EVAL_TYPE_GLOBAL);
    
    if (JS_IsException(val)) {
        // 获取错误信息
        JSValue exception = JS_GetException(m_context);
        const char* errorStr = JS_ToCString(m_context, exception);
        std::cerr << "JavaScript 执行错误: " << (errorStr ? errorStr : "未知错误") << std::endl;
        if (errorStr) {
            JS_FreeCString(m_context, errorStr);
        }
        JS_FreeValue(m_context, exception);
        JS_FreeValue(m_context, val);
        return false;
    }
    
    JS_FreeValue(m_context, val);
    return true;
}

// 执行 JavaScript 文件
bool QuickJSEngine::executeFile(const std::string& filename) {
    std::cout << "正在加载 JavaScript 文件: " << filename << std::endl;
    
    // 读取文件内容
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开 JavaScript 文件: " << filename << std::endl;
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    
    file.close();
    
    // 执行代码
    return executeCode(code);
}

// 评估 JavaScript 表达式并返回结果
std::string QuickJSEngine::evaluate(const std::string& code) {
    if (!m_initialized) {
        std::cerr << "QuickJS 引擎未初始化" << std::endl;
        return "";
    }
    
    // 评估表达式
    JSValue val = JS_Eval(m_context, code.c_str(), code.length(), "<eval>", JS_EVAL_TYPE_GLOBAL);
    
    if (JS_IsException(val)) {
        // 获取错误信息
        JSValue exception = JS_GetException(m_context);
        const char* errorStr = JS_ToCString(m_context, exception);
        std::string error = errorStr ? errorStr : "未知错误";
        if (errorStr) {
            JS_FreeCString(m_context, errorStr);
        }
        JS_FreeValue(m_context, exception);
        JS_FreeValue(m_context, val);
        return "错误: " + error;
    }
    
    // 将结果转换为字符串
    const char* resultStr = JS_ToCString(m_context, val);
    std::string result = resultStr ? resultStr : "undefined";
    if (resultStr) {
        JS_FreeCString(m_context, resultStr);
    }
    
    JS_FreeValue(m_context, val);
    return result;
}

// 注册 C++ 函数到 JavaScript（简化版）
bool QuickJSEngine::registerFunction(const std::string& name, std::function<void(const std::string&)> func) {
    // 这是一个简化版本，完整实现需要使用 JS_NewCFunction
    // 暂时只打印函数名
    std::cout << "注册函数: " << name << " (简化版本)" << std::endl;
    return true;
}

// QuickJS 引擎包装器
// 提供 C++ 友好的 JavaScript 执行接口

#ifndef QUICKJS_ENGINE_H
#define QUICKJS_ENGINE_H

#include <string>
#include <vector>
#include <memory>
#include <functional>

// 前向声明 QuickJS 类型
struct JSContext;
struct JSRuntime;

// QuickJS 引擎包装器类
class QuickJSEngine {
private:
    JSRuntime* m_runtime;  // QuickJS 运行时
    JSContext* m_context;  // QuickJS 上下文
    bool m_initialized;    // 是否已初始化
    
    // 初始化 QuickJS 运行时和上下文
    bool initialize();
    
    // 清理 QuickJS 资源
    void cleanup();
    
public:
    QuickJSEngine();
    ~QuickJSEngine();
    
    // 初始化引擎
    bool init();
    
    // 执行 JavaScript 代码
    bool executeCode(const std::string& code);
    
    // 执行 JavaScript 文件
    bool executeFile(const std::string& filename);
    
    // 评估 JavaScript 表达式并返回结果
    std::string evaluate(const std::string& code);
    
    // 检查引擎是否已初始化
    bool isInitialized() const { return m_initialized; }
    
    // 获取上下文指针（用于高级操作）
    JSContext* getContext() { return m_context; }
    
    // 注册 C++ 函数到 JavaScript
    bool registerFunction(const std::string& name, std::function<void(const std::string&)> func);
};

#endif // QUICKJS_ENGINE_H

#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>

#ifdef ENABLE_QUICKJS
#include "quickjs.h"
#include "quickjs-libc.h"
#endif

// 读取文件内容到字符串
std::string readFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    printf("=== QuickJS 基本测试程序 ===\n\n");
    
#ifdef ENABLE_QUICKJS
    // 步骤1: 创建 QuickJS 运行时
    printf("步骤1: 创建 QuickJS 运行时...\n");
    JSRuntime* rt = JS_NewRuntime();
    if (!rt) {
        printf("错误: 无法创建 QuickJS 运行时\n");
        return 1;
    }
    // 设置栈大小为2MB，避免JavaScript执行时栈溢出
    JS_SetMaxStackSize(rt, 2 * 1024 * 1024);
    printf("成功: QuickJS 运行时创建成功\n\n");
    
    // 步骤2: 创建 QuickJS 上下文
    printf("步骤2: 创建 QuickJS 上下文...\n");
    JSContext* ctx = JS_NewContext(rt);
    if (!ctx) {
        printf("错误: 无法创建 QuickJS 上下文\n");
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: QuickJS 上下文创建成功\n\n");
    
    // 步骤3: 初始化标准库
    printf("步骤3: 初始化标准库...\n");
    js_std_init_handlers(rt);
    js_std_add_helpers(ctx, -1, nullptr);
    printf("成功: 标准库初始化完成\n\n");
    
    // 步骤4: 执行简单的 JavaScript 测试脚本
    printf("步骤4: 执行简单的 JavaScript 测试脚本...\n");
    
    // 测试1: 简单的算术表达式
    const char* test_code1 = "1 + 2";
    
    printf("测试1: 执行代码: %s\n", test_code1);
    JSValue val1 = JS_Eval(ctx, test_code1, strlen(test_code1), "test1.js", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(val1)) {
        printf("错误: 测试1执行失败\n");
        js_std_dump_error(ctx);
        JS_FreeValue(ctx, val1);
    } else {
        printf("成功: 测试1执行完成\n");
        JS_FreeValue(ctx, val1);
    }
    
    // 测试2: 使用print函数
    const char* test_code2 = "print(123)";
    
    printf("\n测试2: 执行代码: %s\n", test_code2);
    JSValue val2 = JS_Eval(ctx, test_code2, strlen(test_code2), "test2.js", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(val2)) {
        printf("错误: 测试2执行失败\n");
        js_std_dump_error(ctx);
        JS_FreeValue(ctx, val2);
    } else {
        printf("成功: 测试2执行完成\n");
        JS_FreeValue(ctx, val2);
    }
    
    // 测试3: 使用console.log函数
    const char* test_code3 = "console.log(456)";
    
    printf("\n测试3: 执行代码: %s\n", test_code3);
    JSValue val3 = JS_Eval(ctx, test_code3, strlen(test_code3), "test3.js", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(val3)) {
        printf("错误: 测试3执行失败\n");
        js_std_dump_error(ctx);
        JS_FreeValue(ctx, val3);
    } else {
        printf("成功: 测试3执行完成\n");
        JS_FreeValue(ctx, val3);
    }
    
    // 步骤5: 清理资源
    printf("\n步骤5: 清理资源...\n");
    js_std_free_handlers(rt);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    printf("成功: 资源清理完成\n\n");
    
    printf("=== QuickJS 基本测试完成 ===\n");
#else
    printf("QuickJS 未启用\n");
#endif
    
    return 0;
}

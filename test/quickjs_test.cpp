#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#ifdef ENABLE_QUICKJS
#include "quickjs.h"
#endif

// 测试 QuickJS 引擎功能
void testQuickJS() {
#ifdef ENABLE_QUICKJS
    printf("=== QuickJS 引擎测试开始 ===\n");
    
    // 创建 QuickJS 运行时
    JSRuntime* rt = JS_NewRuntime();
    if (!rt) {
        printf("错误: 无法创建 QuickJS 运行时\n");
        return;
    }
    
    // 创建 QuickJS 上下文
    JSContext* ctx = JS_NewContext(rt);
    if (!ctx) {
        printf("错误: 无法创建 QuickJS 上下文\n");
        JS_FreeRuntime(rt);
        return;
    }
    
    printf("QuickJS 运行时和上下文创建成功\n");
    
    // 测试1: 执行简单的 JavaScript 代码
    printf("\n--- 测试1: 基本算术运算 ---\n");
    const char* testCode1 = "var x = 10; var y = 20; x + y;";
    JSValue result1 = JS_Eval(ctx, testCode1, strlen(testCode1), "test1.js", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(result1)) {
        printf("错误: 执行 JavaScript 代码失败\n");
        JS_FreeValue(ctx, result1);
    } else {
        int32_t value;
        if (JS_ToInt32(ctx, &value, result1) == 0) {
            printf("结果: 10 + 20 = %d\n", value);
        }
        JS_FreeValue(ctx, result1);
    }
    
    // 测试2: 执行更复杂的 JavaScript 代码
    printf("\n--- 测试2: 复杂表达式 ---\n");
    const char* testCode2 = "var arr = [1, 2, 3, 4, 5]; var sum = 0; for(var i = 0; i < arr.length; i++) { sum += arr[i]; } sum;";
    JSValue result2 = JS_Eval(ctx, testCode2, strlen(testCode2), "test2.js", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(result2)) {
        printf("错误: 执行 JavaScript 代码失败\n");
        JS_FreeValue(ctx, result2);
    } else {
        int32_t value;
        if (JS_ToInt32(ctx, &value, result2) == 0) {
            printf("结果: 数组求和 [1,2,3,4,5] = %d\n", value);
        }
        JS_FreeValue(ctx, result2);
    }
    
    // 测试3: 从文件加载并执行 JavaScript 代码
    printf("\n--- 测试3: 从文件加载 JavaScript 脚本 ---\n");
    std::ifstream scriptFile("test/test_script.js");
    if (scriptFile.is_open()) {
        std::string scriptContent((std::istreambuf_iterator<char>(scriptFile)),
                                   std::istreambuf_iterator<char>());
        scriptFile.close();
        
        printf("加载脚本文件: test/test_script.js (%zu 字节)\n", scriptContent.length());
        
        JSValue result3 = JS_Eval(ctx, scriptContent.c_str(), scriptContent.length(),
                                   "test_script.js", JS_EVAL_TYPE_GLOBAL);
        if (JS_IsException(result3)) {
            printf("错误: 执行脚本文件失败\n");
            JS_FreeValue(ctx, result3);
        } else {
            printf("脚本文件执行成功\n");
            JS_FreeValue(ctx, result3);
        }
    } else {
        printf("警告: 无法打开脚本文件 test/test_script.js\n");
    }
    
    // 测试4: 测试字符串操作
    printf("\n--- 测试4: 字符串操作 ---\n");
    const char* testCode4 = "var str = 'Hello, World!'; str.toUpperCase();";
    JSValue result4 = JS_Eval(ctx, testCode4, strlen(testCode4), "test4.js", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(result4)) {
        printf("错误: 执行 JavaScript 代码失败\n");
        JS_FreeValue(ctx, result4);
    } else {
        const char* str = JS_ToCString(ctx, result4);
        if (str) {
            printf("结果: %s\n", str);
            JS_FreeCString(ctx, str);
        }
        JS_FreeValue(ctx, result4);
    }
    
    // 测试5: 测试函数调用
    printf("\n--- 测试5: 函数定义和调用 ---\n");
    const char* testCode5 = "function add(a, b) { return a + b; } add(100, 200);";
    JSValue result5 = JS_Eval(ctx, testCode5, strlen(testCode5), "test5.js", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(result5)) {
        printf("错误: 执行 JavaScript 代码失败\n");
        JS_FreeValue(ctx, result5);
    } else {
        int32_t value;
        if (JS_ToInt32(ctx, &value, result5) == 0) {
            printf("结果: add(100, 200) = %d\n", value);
        }
        JS_FreeValue(ctx, result5);
    }
    
    // 清理资源
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    
    printf("\n=== QuickJS 引擎测试完成 ===\n");
#else
    printf("QuickJS 未启用，跳过测试\n");
#endif
}

int main() {
    printf("========================================\n");
    printf("  QuickJS 集成测试程序\n");
    printf("========================================\n\n");
    
    testQuickJS();
    
    printf("\n========================================\n");
    printf("  测试程序结束\n");
    printf("========================================\n");
    
    return 0;
}

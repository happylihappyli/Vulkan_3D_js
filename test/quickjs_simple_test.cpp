#include <cstdio>
#include <cstring>

#ifdef ENABLE_QUICKJS
#include "quickjs.h"
#endif

int main() {
    printf("=== QuickJS 简单测试程序 ===\n\n");
    
#ifdef ENABLE_QUICKJS
    printf("步骤1: 创建 QuickJS 运行时...\n");
    JSRuntime* rt = JS_NewRuntime();
    if (!rt) {
        printf("错误: 无法创建 QuickJS 运行时\n");
        return 1;
    }
    printf("成功: QuickJS 运行时创建成功\n\n");
    
    printf("步骤2: 创建 QuickJS 上下文...\n");
    JSContext* ctx = JS_NewContext(rt);
    if (!ctx) {
        printf("错误: 无法创建 QuickJS 上下文\n");
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: QuickJS 上下文创建成功\n\n");
    
    printf("步骤3: 执行简单的 JavaScript 代码...\n");
    const char* testCode = "1 + 1";
    JSValue result = JS_Eval(ctx, testCode, strlen(testCode), "test.js", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(result)) {
        printf("错误: 执行 JavaScript 代码失败\n");
        JS_FreeValue(ctx, result);
    } else {
        int32_t value;
        if (JS_ToInt32(ctx, &value, result) == 0) {
            printf("成功: 1 + 1 = %d\n\n", value);
        }
        JS_FreeValue(ctx, result);
    }
    
    printf("步骤4: 清理资源...\n");
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    printf("成功: 资源清理完成\n\n");
    
    printf("=== QuickJS 测试完成 ===\n");
#else
    printf("QuickJS 未启用\n");
#endif
    
    return 0;
}

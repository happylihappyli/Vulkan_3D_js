#include <cstdio>
#include <cstring>

#ifdef ENABLE_QUICKJS
#include "quickjs.h"
#endif

int main() {
    printf("=== QuickJS 分步测试程序 ===\n\n");
    
#ifdef ENABLE_QUICKJS
    printf("步骤1: 创建 QuickJS 运行时...\n");
    JSRuntime* rt = JS_NewRuntime();
    if (!rt) {
        printf("错误: 无法创建 QuickJS 运行时\n");
        return 1;
    }
    printf("成功: QuickJS 运行时创建成功\n\n");
    
    printf("步骤2: 创建 QuickJS 上下文 (Raw)...\n");
    JSContext* ctx = JS_NewContextRaw(rt);
    if (!ctx) {
        printf("错误: 无法创建 QuickJS 上下文 (Raw)\n");
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: QuickJS 上下文 (Raw) 创建成功\n\n");
    
    printf("步骤3: 测试 JS_AddIntrinsicBaseObjects...\n");
    int result = JS_AddIntrinsicBaseObjects(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicBaseObjects 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicBaseObjects 完成\n\n");
    
    printf("步骤4: 测试 JS_AddIntrinsicDate...\n");
    result = JS_AddIntrinsicDate(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicDate 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicDate 完成\n\n");
    
    printf("步骤5: 测试 JS_AddIntrinsicEval...\n");
    result = JS_AddIntrinsicEval(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicEval 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicEval 完成\n\n");
    
    printf("步骤6: 测试 JS_AddIntrinsicStringNormalize...\n");
    result = JS_AddIntrinsicStringNormalize(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicStringNormalize 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicStringNormalize 完成\n\n");
    
    printf("步骤7: 测试 JS_AddIntrinsicRegExp...\n");
    result = JS_AddIntrinsicRegExp(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicRegExp 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicRegExp 完成\n\n");
    
    printf("步骤8: 测试 JS_AddIntrinsicJSON...\n");
    result = JS_AddIntrinsicJSON(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicJSON 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicJSON 完成\n\n");
    
    printf("步骤9: 测试 JS_AddIntrinsicProxy...\n");
    result = JS_AddIntrinsicProxy(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicProxy 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicProxy 完成\n\n");
    
    printf("步骤10: 测试 JS_AddIntrinsicMapSet...\n");
    result = JS_AddIntrinsicMapSet(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicMapSet 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicMapSet 完成\n\n");
    
    printf("步骤11: 测试 JS_AddIntrinsicTypedArrays...\n");
    result = JS_AddIntrinsicTypedArrays(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicTypedArrays 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicTypedArrays 完成\n\n");
    
    printf("步骤12: 测试 JS_AddIntrinsicPromise...\n");
    result = JS_AddIntrinsicPromise(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicPromise 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicPromise 完成\n\n");
    
    printf("步骤13: 测试 JS_AddIntrinsicWeakRef...\n");
    result = JS_AddIntrinsicWeakRef(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicWeakRef 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicWeakRef 完成\n\n");
    
    printf("步骤14: 清理资源...\n");
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    printf("成功: 资源清理完成\n\n");
    
    printf("=== QuickJS 测试完成 ===\n");
#else
    printf("QuickJS 未启用\n");
#endif
    
    return 0;
}

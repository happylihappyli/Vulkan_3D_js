#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quickjs.h"

int main(int argc, char **argv) {
    JSRuntime *rt;
    JSContext *ctx;
    JSValue val;
    
    printf("[测试] 开始QuickJS测试\n");
    
    rt = JS_NewRuntime();
    if (!rt) {
        printf("[错误] 无法创建QuickJS运行时\n");
        return 1;
    }
    printf("[测试] QuickJS运行时创建成功\n");
    
    ctx = JS_NewContext(rt);
    if (!ctx) {
        printf("[错误] 无法创建QuickJS上下文\n");
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("[测试] QuickJS上下文创建成功\n");
    
    printf("[测试] 开始添加基础对象\n");
    if (JS_AddIntrinsicBaseObjects(ctx) < 0) {
        printf("[错误] 添加基础对象失败\n");
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("[测试] 基础对象添加成功\n");
    
    printf("[测试] 测试Math.min()函数\n");
    const char *code = "Math.min(1, 2, 3)";
    val = JS_Eval(ctx, code, strlen(code), "<test>", JS_EVAL_TYPE_GLOBAL);
    
    if (JS_IsException(val)) {
        printf("[错误] 执行Math.min()失败\n");
        JSValue exception = JS_GetException(ctx);
        JSValue str = JS_ToString(ctx, exception);
        const char *error_msg = JS_ToCString(ctx, str);
        printf("[错误] 错误信息: %s\n", error_msg);
        JS_FreeCString(ctx, error_msg);
        JS_FreeValue(ctx, str);
        JS_FreeValue(ctx, exception);
    } else {
        printf("[成功] Math.min(1, 2, 3) = ");
        JSValue str = JS_ToString(ctx, val);
        const char *result = JS_ToCString(ctx, str);
        printf("%s\n", result);
        JS_FreeCString(ctx, result);
        JS_FreeValue(ctx, str);
    }
    
    JS_FreeValue(ctx, val);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    
    printf("[测试] 测试完成\n");
    return 0;
}

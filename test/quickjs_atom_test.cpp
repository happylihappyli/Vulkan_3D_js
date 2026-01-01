#include <cstdio>
#include <cstring>

#ifdef ENABLE_QUICKJS
#include "quickjs.h"
#endif

int main() {
    printf("=== QuickJS Atom测试程序 ===\n\n");
    
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
    
    printf("步骤3: 检查基本常量...\n");
    printf("JS_ATOM_NULL = %d\n", JS_ATOM_NULL);
    printf("JS_TAG_OBJECT = %d\n", JS_TAG_OBJECT);
    printf("JS_TAG_NULL = %d\n", JS_TAG_NULL);
    printf("JS_TAG_UNDEFINED = %d\n", JS_TAG_UNDEFINED);
    printf("\n");
    
    printf("步骤4: 测试 JS_AddIntrinsicBaseObjects...\n");
    int result = JS_AddIntrinsicBaseObjects(ctx);
    if (result < 0) {
        printf("错误: JS_AddIntrinsicBaseObjects 失败 (返回 %d)\n", result);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: JS_AddIntrinsicBaseObjects 完成\n\n");
    
    printf("步骤5: 清理资源...\n");
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    printf("成功: 资源清理完成\n\n");
    
    printf("=== QuickJS 测试完成 ===\n");
#else
    printf("QuickJS 未启用\n");
#endif
    
    return 0;
}

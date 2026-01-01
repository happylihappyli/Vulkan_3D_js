#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>

#ifdef ENABLE_QUICKJS
#include "quickjs.h"
#include "quickjs-libc.h"
#include "quickjs_vulkan_bridge.h"
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

int main(int argc, char* argv[]) {
    printf("=== QuickJS 模型加载测试程序 ===\n\n");
    
    // 检查命令行参数
    const char* test_script_file = "test/test_model_full.js";  // 默认测试脚本
    if (argc > 1) {
        test_script_file = argv[1];
        printf("使用指定的测试脚本: %s\n\n", test_script_file);
    } else {
        printf("使用默认测试脚本: %s\n", test_script_file);
        printf("提示: 可以通过命令行参数指定测试脚本\n");
        printf("用法: quickjs_model_test.exe [脚本文件路径]\n\n");
    }
    
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
    
    // 步骤4: 初始化 QuickJS-Vulkan 桥接模块
    printf("步骤4: 初始化 QuickJS-Vulkan 桥接模块...\n");
    if (!initQuickJSVulkanBridge(ctx, rt)) {
        printf("错误: 无法初始化 QuickJS-Vulkan 桥接模块\n");
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    printf("成功: QuickJS-Vulkan 桥接模块初始化完成\n\n");
    
    // 步骤5: 执行 JavaScript 测试脚本
    printf("步骤5: 执行 JavaScript 测试脚本...\n");
    
    // 读取测试脚本文件
    std::string test_script = readFile(test_script_file);
    if (test_script.empty()) {
        printf("警告: 无法读取测试脚本文件 %s，使用内置测试代码\n", test_script_file);
        
        // 使用内置测试代码
        const char* builtin_test_code = 
            "log('=== QuickJS-Vulkan桥接模块测试 ===');"
            "log('测试数学计算功能:');"
            "let result = calculate(10, 20);"
            "log('10 + 20 = ' + result);"
            "log('测试获取未加载模型时的顶点数量:');"
            "let vertexCount = getVertexCount();"
            "log('顶点数量: ' + vertexCount);"
            "log('测试获取未加载模型时的面数量:');"
            "let faceCount = getFaceCount();"
            "log('面数量: ' + faceCount);"
            "log('测试加载不存在的模型文件:');"
            "let success = loadOBJModel('nonexistent.obj');"
            "log('加载结果: ' + success);"
            "log('测试加载存在的模型文件:');"
            "success = loadOBJModel('test/test_model.obj');"
            "log('加载结果: ' + success);"
            "if (success) {"
            "    log('测试获取加载后的顶点数量:');"
            "    vertexCount = getVertexCount();"
            "    log('顶点数量: ' + vertexCount);"
            "    log('测试获取加载后的面数量:');"
            "    faceCount = getFaceCount();"
            "    log('面数量: ' + faceCount);"
            "    log('测试获取顶点数据:');"
            "    let vertices = getVertices();"
            "    if (vertices) {"
            "        log('顶点数据长度: ' + vertices.length);"
            "        if (vertices.length > 0) {"
            "            log('前3个顶点坐标:');"
            "            for (let i = 0; i < Math.min(9, vertices.length); i++) {"
            "                log('  vertices[' + i + '] = ' + vertices[i]);"
            "            }"
            "        }"
            "    }"
            "    log('测试获取法线数据:');"
            "    let normals = getNormals();"
            "    if (normals) {"
            "        log('法线数据长度: ' + normals.length);"
            "        if (normals.length > 0) {"
            "            log('前3个法线坐标:');"
            "            for (let i = 0; i < Math.min(9, normals.length); i++) {"
            "                log('  normals[' + i + '] = ' + normals[i]);"
            "            }"
            "        }"
            "    }"
            "}"
            "log('=== 测试完成 ===');";
        
        test_script = builtin_test_code;
    } else {
        printf("成功: 读取测试脚本文件 %s\n", test_script_file);
    }
    
    // 执行测试脚本
    printf("\n--- 执行测试脚本 ---\n");
    JSValue val = JS_Eval(ctx, test_script.c_str(), test_script.length(), test_script_file, JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(val)) {
        printf("错误: JavaScript 执行失败\n");
        js_std_dump_error(ctx);
        JS_FreeValue(ctx, val);
    } else {
        printf("成功: 测试脚本执行完成\n");
        JS_FreeValue(ctx, val);
    }
    
    // 步骤6: 清理资源
    printf("\n步骤6: 清理资源...\n");
    js_std_free_handlers(rt);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    printf("成功: 资源清理完成\n\n");
    
    printf("=== QuickJS 模型加载测试完成 ===\n");
#else
    printf("QuickJS 未启用\n");
#endif
    
    return 0;
}

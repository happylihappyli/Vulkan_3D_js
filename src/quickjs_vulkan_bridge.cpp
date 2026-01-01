#include "quickjs_vulkan_bridge.h"
#include "model_renderer_3d.h"
#include "model_loader_js.h"
#include "../quickjs/quickjs.h"
#include "../quickjs/quickjs-libc.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// 静态实例指针
QuickJSVulkanBridge* QuickJSVulkanBridge::s_instance = nullptr;

QuickJSVulkanBridge::QuickJSVulkanBridge()
    : m_context(nullptr), m_runtime(nullptr), m_modelRenderer(nullptr) {
}

QuickJSVulkanBridge::~QuickJSVulkanBridge() {
    clearModel();
}

// 初始化桥接模块，注册 JavaScript API
bool QuickJSVulkanBridge::initialize(JSContext* ctx, JSRuntime* rt) {
    m_context = ctx;
    m_runtime = rt;
    
    // 设置实例指针
    setInstance(this);
    
    // 注册所有 JavaScript API
    registerAPIs();
    
    std::cout << "QuickJS-Vulkan 桥接模块初始化成功" << std::endl;
    return true;
}

// 注册所有 JavaScript 函数
void QuickJSVulkanBridge::registerAPIs() {
    if (!m_context) {
        std::cerr << "QuickJS 上下文未初始化" << std::endl;
        return;
    }
    
    // 获取全局对象
    JSValue global_obj = JS_GetGlobalObject(m_context);
    
    // 注册 loadOBJModel 函数
    JSValue load_obj_func = JS_NewCFunction(m_context, js_loadOBJModel, "loadOBJModel", 1);
    JS_SetPropertyStr(m_context, global_obj, "loadOBJModel", load_obj_func);
    
    // 注册 getVertexCount 函数
    JSValue get_vertex_count_func = JS_NewCFunction(m_context, js_getVertexCount, "getVertexCount", 0);
    JS_SetPropertyStr(m_context, global_obj, "getVertexCount", get_vertex_count_func);
    
    // 注册 getFaceCount 函数
    JSValue get_face_count_func = JS_NewCFunction(m_context, js_getFaceCount, "getFaceCount", 0);
    JS_SetPropertyStr(m_context, global_obj, "getFaceCount", get_face_count_func);
    
    // 注册 getVertices 函数
    JSValue get_vertices_func = JS_NewCFunction(m_context, js_getVertices, "getVertices", 0);
    JS_SetPropertyStr(m_context, global_obj, "getVertices", get_vertices_func);
    
    // 注册 getNormals 函数
    JSValue get_normals_func = JS_NewCFunction(m_context, js_getNormals, "getNormals", 0);
    JS_SetPropertyStr(m_context, global_obj, "getNormals", get_normals_func);
    
    // 注册 calculate 函数
    JSValue calculate_func = JS_NewCFunction(m_context, js_calculate, "calculate", 2);
    JS_SetPropertyStr(m_context, global_obj, "calculate", calculate_func);
    
    // 创建 console 对象
    JSValue console_obj = JS_NewObject(m_context);
    
    // 注册 console.log 方法
    JSValue console_log_func = JS_NewCFunction(m_context, js_console_log, "log", 1);
    JS_SetPropertyStr(m_context, console_obj, "log", console_log_func);
    
    // 将 console 对象添加到全局对象
    JS_SetPropertyStr(m_context, global_obj, "console", console_obj);
    
    // 释放全局对象
    JS_FreeValue(m_context, global_obj);
    
    std::cout << "JavaScript API 注册完成" << std::endl;
}

// JavaScript 可调用的函数：加载 OBJ 模型
JSValue QuickJSVulkanBridge::js_loadOBJModel(JSContext* ctx, JSValueConst this_val,
                                                int argc, JSValueConst* argv) {
    if (argc < 1) {
        std::cerr << "loadOBJModel: 需要一个文件名参数" << std::endl;
        return JS_ThrowTypeError(ctx, "需要文件名参数");
    }
    
    // 获取文件名
    const char* filename = JS_ToCString(ctx, argv[0]);
    if (!filename) {
        std::cerr << "loadOBJModel: 无法获取文件名" << std::endl;
        return JS_ThrowTypeError(ctx, "无法获取文件名");
    }
    
    std::cout << "JavaScript 调用: loadOBJModel(\"" << filename << "\") [Direct Renderer]" << std::endl;
    
    if (!s_instance || !s_instance->m_modelRenderer) {
        std::cerr << "loadOBJModel: 渲染器未初始化" << std::endl;
        JS_FreeCString(ctx, filename);
        return JS_FALSE;
    }

    // 直接使用 ModelRenderer3D 加载
    // 这样可以利用 ModelRenderer3D 中更完善的解析逻辑（支持材质和子网格）
    bool success = s_instance->m_modelRenderer->loadOBJModel(filename);
    
    // 释放字符串
    JS_FreeCString(ctx, filename);
    
    if (!success) {
        std::cerr << "loadOBJModel: 模型加载失败" << std::endl;
        return JS_FALSE;
    }
    
    // 从渲染器同步数据回 m_currentModel (为了兼容 getVertices 等 API)
    s_instance->m_currentModel.vertices = s_instance->m_modelRenderer->getVertices();
    s_instance->m_currentModel.indices = s_instance->m_modelRenderer->getIndices();
    // ModelRenderer3D 目前主要处理位置和索引，法线和UV可能需要额外处理，这里暂时清空以免数据不一致
    s_instance->m_currentModel.normals.clear(); 
    s_instance->m_currentModel.texCoords.clear();
        
    std::cout << "模型加载成功 (Direct)，顶点数: " << s_instance->m_currentModel.vertices.size() / 3 
              << ", 面数: " << s_instance->m_currentModel.indices.size() / 3 << std::endl;
        
    return JS_TRUE;
}

// JavaScript 可调用的函数：获取顶点数量
JSValue QuickJSVulkanBridge::js_getVertexCount(JSContext* ctx, JSValueConst this_val,
                                                  int argc, JSValueConst* argv) {
    if (!s_instance) {
        std::cerr << "getVertexCount: 桥接实例未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "桥接实例未初始化");
    }
    
    size_t vertexCount = s_instance->m_currentModel.vertices.size() / 3;
    std::cout << "JavaScript 调用: getVertexCount() = " << vertexCount << std::endl;
    
    return JS_NewInt32(ctx, (int32_t)vertexCount);
}

// JavaScript 可调用的函数：获取面数量
JSValue QuickJSVulkanBridge::js_getFaceCount(JSContext* ctx, JSValueConst this_val,
                                                int argc, JSValueConst* argv) {
    if (!s_instance) {
        std::cerr << "getFaceCount: 桥接实例未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "桥接实例未初始化");
    }
    
    size_t faceCount = s_instance->m_currentModel.indices.size() / 3;
    std::cout << "JavaScript 调用: getFaceCount() = " << faceCount << std::endl;
    
    return JS_NewInt32(ctx, (int32_t)faceCount);
}

// JavaScript 可调用的函数：获取顶点数据
JSValue QuickJSVulkanBridge::js_getVertices(JSContext* ctx, JSValueConst this_val,
                                               int argc, JSValueConst* argv) {
    if (!s_instance) {
        std::cerr << "getVertices: 桥接实例未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "桥接实例未初始化");
    }
    
    const std::vector<float>& vertices = s_instance->m_currentModel.vertices;
    size_t vertexCount = vertices.size();
    
    std::cout << "JavaScript 调用: getVertices() = " << vertexCount << " 个浮点数" << std::endl;
    
    // 创建 JavaScript 数组
    JSValue array = JS_NewArray(ctx);
    
    // 填充顶点数据
    for (size_t i = 0; i < vertexCount; i++) {
        JSValue value = JS_NewFloat64(ctx, vertices[i]);
        JS_SetPropertyUint32(ctx, array, (uint32_t)i, value);
    }
    
    return array;
}

// JavaScript 可调用的函数：获取法线数据
JSValue QuickJSVulkanBridge::js_getNormals(JSContext* ctx, JSValueConst this_val,
                                               int argc, JSValueConst* argv) {
    if (!s_instance) {
        std::cerr << "getNormals: 桥接实例未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "桥接实例未初始化");
    }
    
    const std::vector<float>& normals = s_instance->m_currentModel.normals;
    size_t normalCount = normals.size();
    
    std::cout << "JavaScript 调用: getNormals() = " << normalCount << " 个浮点数" << std::endl;
    
    // 创建 JavaScript 数组
    JSValue array = JS_NewArray(ctx);
    
    // 填充法线数据
    for (size_t i = 0; i < normalCount; i++) {
        JSValue value = JS_NewFloat64(ctx, normals[i]);
        JS_SetPropertyUint32(ctx, array, (uint32_t)i, value);
    }
    
    return array;
}

// JavaScript 可调用的函数：打印日志
JSValue QuickJSVulkanBridge::js_log(JSContext* ctx, JSValueConst this_val,
                                       int argc, JSValueConst* argv) {
    if (argc < 1) {
        std::cerr << "log: 需要一个参数" << std::endl;
        return JS_ThrowTypeError(ctx, "log requires at least one argument");
    }
    
    // 获取字符串参数（JS_ToCString会自动处理类型转换）
    const char* message = JS_ToCString(ctx, argv[0]);
    if (message) {
        std::cout << "[JavaScript] " << message << std::endl;
        JS_FreeCString(ctx, message);
    } else {
        std::cerr << "log: 无法获取字符串内容" << std::endl;
    }
    
    return JS_UNDEFINED;
}

// JavaScript 可调用的函数：console.log 实现
JSValue QuickJSVulkanBridge::js_console_log(JSContext* ctx, JSValueConst this_val,
                                             int argc, JSValueConst* argv) {
    printf("[调试] console.log 被调用，参数个数: %d\n", argc);
    
    // 支持多个参数，用空格分隔
    for (int i = 0; i < argc; i++) {
        const char* message = JS_ToCString(ctx, argv[i]);
        if (message) {
            printf("[调试] 参数 %d: %s\n", i, message);
            std::cout << message;
            if (i < argc - 1) {
                std::cout << " ";
            } else {
                std::cout << std::endl;
            }
            JS_FreeCString(ctx, message);
        }
    }
    
    printf("[调试] console.log 执行完毕\n");
    return JS_UNDEFINED;
}

// JavaScript 可调用的函数：执行数学计算
JSValue QuickJSVulkanBridge::js_calculate(JSContext* ctx, JSValueConst this_val,
                                            int argc, JSValueConst* argv) {
    if (argc < 2) {
        std::cerr << "calculate: 需要两个参数" << std::endl;
        return JS_ThrowTypeError(ctx, "需要两个参数");
    }
    
    double a, b;
    if (JS_ToFloat64(ctx, &a, argv[0]) < 0 || JS_ToFloat64(ctx, &b, argv[1]) < 0) {
        std::cerr << "calculate: 参数必须是数字" << std::endl;
        return JS_ThrowTypeError(ctx, "参数必须是数字");
    }
    
    double result = a + b;
    std::cout << "JavaScript 调用: calculate(" << a << ", " << b << ") = " << result << std::endl;
    
    return JS_NewFloat64(ctx, result);
}

// 清空模型数据
void QuickJSVulkanBridge::clearModel() {
    m_currentModel.vertices.clear();
    m_currentModel.normals.clear();
    m_currentModel.texCoords.clear();
    m_currentModel.indices.clear();
}

// 初始化 QuickJS-Vulkan 桥接模块
bool initQuickJSVulkanBridge(JSContext* ctx, JSRuntime* rt) {
    static QuickJSVulkanBridge bridge;
    return bridge.initialize(ctx, rt);
}

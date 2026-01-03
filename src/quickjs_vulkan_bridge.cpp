#include "quickjs_vulkan_bridge.h"
#include "model_renderer_3d.h"
#include "button_manager.h"
#include "script_viewer_app.h"
#include "../quickjs/quickjs.h"
#include "../quickjs/quickjs-libc.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// 静态实例指针
QuickJSVulkanBridge* QuickJSVulkanBridge::s_instance = nullptr;

QuickJSVulkanBridge::QuickJSVulkanBridge()
    : m_context(nullptr), m_runtime(nullptr), m_modelRenderer(nullptr), m_buttonManager(nullptr), m_app(nullptr) {
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
    
    // 注册 createButton 函数
    JSValue create_button_func = JS_NewCFunction(m_context, js_createButton, "createButton", 1);
    JS_SetPropertyStr(m_context, global_obj, "createButton", create_button_func);
    
    // 注册 setButtonPosition 函数
    JSValue set_button_position_func = JS_NewCFunction(m_context, js_setButtonPosition, "setButtonPosition", 3);
    JS_SetPropertyStr(m_context, global_obj, "setButtonPosition", set_button_position_func);
    
    // 注册 setButtonSize 函数
    JSValue set_button_size_func = JS_NewCFunction(m_context, js_setButtonSize, "setButtonSize", 3);
    JS_SetPropertyStr(m_context, global_obj, "setButtonSize", set_button_size_func);
    
    // 注册 setButtonColor 函数
    JSValue set_button_color_func = JS_NewCFunction(m_context, js_setButtonColor, "setButtonColor", 4);
    JS_SetPropertyStr(m_context, global_obj, "setButtonColor", set_button_color_func);
    
    // 注册 setButtonVisible 函数
    JSValue set_button_visible_func = JS_NewCFunction(m_context, js_setButtonVisible, "setButtonVisible", 2);
    JS_SetPropertyStr(m_context, global_obj, "setButtonVisible", set_button_visible_func);
    
    // 注册 setButtonEnabled 函数
    JSValue set_button_enabled_func = JS_NewCFunction(m_context, js_setButtonEnabled, "setButtonEnabled", 2);
    JS_SetPropertyStr(m_context, global_obj, "setButtonEnabled", set_button_enabled_func);
    
    // 注册 setButtonClickHandler 函数
    JSValue set_button_click_handler_func = JS_NewCFunction(m_context, js_setButtonClickHandler, "setButtonClickHandler", 2);
    JS_SetPropertyStr(m_context, global_obj, "setButtonClickHandler", set_button_click_handler_func);
    
    // 注册 deleteButton 函数
    JSValue delete_button_func = JS_NewCFunction(m_context, js_deleteButton, "deleteButton", 1);
    JS_SetPropertyStr(m_context, global_obj, "deleteButton", delete_button_func);
    
    // 注册 clearAllButtons 函数
    JSValue clear_all_buttons_func = JS_NewCFunction(m_context, js_clearAllButtons, "clearAllButtons", 0);
    JS_SetPropertyStr(m_context, global_obj, "clearAllButtons", clear_all_buttons_func);
    
    // 注册 clearModel 函数
    JSValue clear_model_func = JS_NewCFunction(m_context, js_clearModel, "clearModel", 0);
    JS_SetPropertyStr(m_context, global_obj, "clearModel", clear_model_func);
    
    // 创建 console 对象
    JSValue console_obj = JS_NewObject(m_context);
    
    // 注册 console.log 方法
    JSValue console_log_func = JS_NewCFunction(m_context, js_console_log, "log", 1);
    JS_SetPropertyStr(m_context, console_obj, "log", console_log_func);
    
    // 将 console 对象添加到全局对象
    JS_SetPropertyStr(m_context, global_obj, "console", console_obj);
    
    // 创建 Math 对象
    JSValue math_obj = JS_NewObject(m_context);
    
    // 注册 Math.min 方法
    JSValue math_min_func = JS_NewCFunction(m_context, js_math_min, "min", 2);
    JS_SetPropertyStr(m_context, math_obj, "min", math_min_func);
    
    // 将 Math 对象添加到全局对象
    JS_SetPropertyStr(m_context, global_obj, "Math", math_obj);
    
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
    
    // 构建输出字符串
    std::string output;
    
    // 支持多个参数，用空格分隔
    for (int i = 0; i < argc; i++) {
        const char* message = JS_ToCString(ctx, argv[i]);
        if (message) {
            printf("[调试] 参数 %d: %s\n", i, message);
            output += message;
            if (i < argc - 1) {
                output += " ";
            }
            JS_FreeCString(ctx, message);
        }
    }
    
    // 输出到标准控制台
    std::cout << output << std::endl;
    
    // 输出到UI窗口（如果应用程序指针可用）
    if (s_instance && s_instance->m_app) {
        s_instance->m_app->addLog(output);
    }
    
    return JS_UNDEFINED;
}

// JavaScript 可调用的函数：Math.min
JSValue QuickJSVulkanBridge::js_math_min(JSContext* ctx, JSValueConst this_val,
                                           int argc, JSValueConst* argv) {
    if (argc < 1) {
        std::cerr << "Math.min: 至少需要一个参数" << std::endl;
        return JS_ThrowTypeError(ctx, "至少需要一个参数");
    }
    
    double min_val;
    if (JS_ToFloat64(ctx, &min_val, argv[0]) < 0) {
        std::cerr << "Math.min: 参数必须是数字" << std::endl;
        return JS_ThrowTypeError(ctx, "参数必须是数字");
    }
    
    // 遍历所有参数，找到最小值
    for (int i = 1; i < argc; i++) {
        double val;
        if (JS_ToFloat64(ctx, &val, argv[i]) < 0) {
            std::cerr << "Math.min: 参数必须是数字" << std::endl;
            return JS_ThrowTypeError(ctx, "参数必须是数字");
        }
        if (val < min_val) {
            min_val = val;
        }
    }
    
    return JS_NewFloat64(ctx, min_val);
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

// JavaScript 可调用的函数：创建按钮
JSValue QuickJSVulkanBridge::js_createButton(JSContext* ctx, JSValueConst this_val,
                                                int argc, JSValueConst* argv) {
    if (argc < 1) {
        std::cerr << "createButton: 需要一个文本参数" << std::endl;
        return JS_ThrowTypeError(ctx, "需要一个文本参数");
    }
    
    if (!s_instance || !s_instance->m_buttonManager) {
        std::cerr << "createButton: 按钮管理器未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "按钮管理器未初始化");
    }
    
    const char* text = JS_ToCString(ctx, argv[0]);
    if (!text) {
        std::cerr << "createButton: 无法获取文本" << std::endl;
        return JS_ThrowTypeError(ctx, "无法获取文本");
    }
    
    uint32_t buttonId = s_instance->m_buttonManager->createButton(text);
    JS_FreeCString(ctx, text);
    
    return JS_NewInt32(ctx, (int32_t)buttonId);
}

// JavaScript 可调用的函数：设置按钮位置
JSValue QuickJSVulkanBridge::js_setButtonPosition(JSContext* ctx, JSValueConst this_val,
                                                     int argc, JSValueConst* argv) {
    if (argc < 3) {
        std::cerr << "setButtonPosition: 需要三个参数（按钮ID, x, y）" << std::endl;
        return JS_ThrowTypeError(ctx, "需要三个参数");
    }
    
    if (!s_instance || !s_instance->m_buttonManager) {
        std::cerr << "setButtonPosition: 按钮管理器未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "按钮管理器未初始化");
    }
    
    int32_t buttonId;
    double dx, dy;
    
    if (JS_ToInt32(ctx, &buttonId, argv[0]) < 0) {
        std::cerr << "setButtonPosition: 按钮ID必须是整数" << std::endl;
        return JS_ThrowTypeError(ctx, "按钮ID必须是整数");
    }
    
    if (JS_ToFloat64(ctx, &dx, argv[1]) < 0 || JS_ToFloat64(ctx, &dy, argv[2]) < 0) {
        std::cerr << "setButtonPosition: 位置坐标必须是数字" << std::endl;
        return JS_ThrowTypeError(ctx, "位置坐标必须是数字");
    }
    
    float x = (float)dx;
    float y = (float)dy;
    
    bool result = s_instance->m_buttonManager->setButtonPosition(buttonId, x, y);
    return result ? JS_TRUE : JS_FALSE;
}

// JavaScript 可调用的函数：设置按钮大小
JSValue QuickJSVulkanBridge::js_setButtonSize(JSContext* ctx, JSValueConst this_val,
                                                int argc, JSValueConst* argv) {
    if (argc < 3) {
        std::cerr << "setButtonSize: 需要三个参数（按钮ID, width, height）" << std::endl;
        return JS_ThrowTypeError(ctx, "需要三个参数");
    }
    
    if (!s_instance || !s_instance->m_buttonManager) {
        std::cerr << "setButtonSize: 按钮管理器未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "按钮管理器未初始化");
    }
    
    int32_t buttonId;
    double dwidth, dheight;
    
    if (JS_ToInt32(ctx, &buttonId, argv[0]) < 0) {
        std::cerr << "setButtonSize: 按钮ID必须是整数" << std::endl;
        return JS_ThrowTypeError(ctx, "按钮ID必须是整数");
    }
    
    if (JS_ToFloat64(ctx, &dwidth, argv[1]) < 0 || JS_ToFloat64(ctx, &dheight, argv[2]) < 0) {
        std::cerr << "setButtonSize: 大小必须是数字" << std::endl;
        return JS_ThrowTypeError(ctx, "大小必须是数字");
    }
    
    float width = (float)dwidth;
    float height = (float)dheight;
    
    bool result = s_instance->m_buttonManager->setButtonSize(buttonId, width, height);
    return result ? JS_TRUE : JS_FALSE;
}

// JavaScript 可调用的函数：设置按钮颜色
JSValue QuickJSVulkanBridge::js_setButtonColor(JSContext* ctx, JSValueConst this_val,
                                                  int argc, JSValueConst* argv) {
    if (argc < 4) {
        std::cerr << "setButtonColor: 需要四个参数（按钮ID, r, g, b）" << std::endl;
        return JS_ThrowTypeError(ctx, "需要四个参数");
    }
    
    if (!s_instance || !s_instance->m_buttonManager) {
        std::cerr << "setButtonColor: 按钮管理器未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "按钮管理器未初始化");
    }
    
    int32_t buttonId;
    double dr, dg, db;
    
    if (JS_ToInt32(ctx, &buttonId, argv[0]) < 0) {
        std::cerr << "setButtonColor: 按钮ID必须是整数" << std::endl;
        return JS_ThrowTypeError(ctx, "按钮ID必须是整数");
    }
    
    if (JS_ToFloat64(ctx, &dr, argv[1]) < 0 || JS_ToFloat64(ctx, &dg, argv[2]) < 0 || 
        JS_ToFloat64(ctx, &db, argv[3]) < 0) {
        std::cerr << "setButtonColor: 颜色值必须是数字" << std::endl;
        return JS_ThrowTypeError(ctx, "颜色值必须是数字");
    }
    
    float r = (float)dr;
    float g = (float)dg;
    float b = (float)db;
    
    bool result = s_instance->m_buttonManager->setButtonColor(buttonId, r, g, b);
    return result ? JS_TRUE : JS_FALSE;
}

// JavaScript 可调用的函数：设置按钮可见性
JSValue QuickJSVulkanBridge::js_setButtonVisible(JSContext* ctx, JSValueConst this_val,
                                                   int argc, JSValueConst* argv) {
    if (argc < 2) {
        std::cerr << "setButtonVisible: 需要两个参数（按钮ID, visible）" << std::endl;
        return JS_ThrowTypeError(ctx, "需要两个参数");
    }
    
    if (!s_instance || !s_instance->m_buttonManager) {
        std::cerr << "setButtonVisible: 按钮管理器未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "按钮管理器未初始化");
    }
    
    int32_t buttonId;
    bool visible;
    
    if (JS_ToInt32(ctx, &buttonId, argv[0]) < 0) {
        std::cerr << "setButtonVisible: 按钮ID必须是整数" << std::endl;
        return JS_ThrowTypeError(ctx, "按钮ID必须是整数");
    }
    
    visible = JS_ToBool(ctx, argv[1]);
    
    bool result = s_instance->m_buttonManager->setButtonVisible(buttonId, visible);
    return result ? JS_TRUE : JS_FALSE;
}

// JavaScript 可调用的函数：设置按钮启用状态
JSValue QuickJSVulkanBridge::js_setButtonEnabled(JSContext* ctx, JSValueConst this_val,
                                                   int argc, JSValueConst* argv) {
    if (argc < 2) {
        std::cerr << "setButtonEnabled: 需要两个参数（按钮ID, enabled）" << std::endl;
        return JS_ThrowTypeError(ctx, "需要两个参数");
    }
    
    if (!s_instance || !s_instance->m_buttonManager) {
        std::cerr << "setButtonEnabled: 按钮管理器未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "按钮管理器未初始化");
    }
    
    int32_t buttonId;
    bool enabled;
    
    if (JS_ToInt32(ctx, &buttonId, argv[0]) < 0) {
        std::cerr << "setButtonEnabled: 按钮ID必须是整数" << std::endl;
        return JS_ThrowTypeError(ctx, "按钮ID必须是整数");
    }
    
    enabled = JS_ToBool(ctx, argv[1]);
    
    bool result = s_instance->m_buttonManager->setButtonEnabled(buttonId, enabled);
    return result ? JS_TRUE : JS_FALSE;
}

// JavaScript 可调用的函数：设置按钮点击事件
JSValue QuickJSVulkanBridge::js_setButtonClickHandler(JSContext* ctx, JSValueConst this_val,
                                                         int argc, JSValueConst* argv) {
    if (argc < 2) {
        std::cerr << "setButtonClickHandler: 需要两个参数（按钮ID, handler）" << std::endl;
        return JS_ThrowTypeError(ctx, "需要两个参数");
    }
    
    if (!s_instance || !s_instance->m_buttonManager) {
        std::cerr << "setButtonClickHandler: 按钮管理器未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "按钮管理器未初始化");
    }
    
    int32_t buttonId;
    
    if (JS_ToInt32(ctx, &buttonId, argv[0]) < 0) {
        std::cerr << "setButtonClickHandler: 按钮ID必须是整数" << std::endl;
        return JS_ThrowTypeError(ctx, "按钮ID必须是整数");
    }
    
    // 检查第二个参数是否为函数
    if (!JS_IsFunction(ctx, argv[1])) {
        std::cerr << "setButtonClickHandler: 第二个参数必须是函数" << std::endl;
        return JS_ThrowTypeError(ctx, "第二个参数必须是函数");
    }
    
    // 调用ButtonManager的JavaScript版本方法
    bool result = s_instance->m_buttonManager->setButtonClickHandler(buttonId, argv[1]);
    return result ? JS_TRUE : JS_FALSE;
}

// JavaScript 可调用的函数：删除按钮
JSValue QuickJSVulkanBridge::js_deleteButton(JSContext* ctx, JSValueConst this_val,
                                                int argc, JSValueConst* argv) {
    if (argc < 1) {
        std::cerr << "deleteButton: 需要一个按钮ID参数" << std::endl;
        return JS_ThrowTypeError(ctx, "需要一个按钮ID参数");
    }
    
    if (!s_instance || !s_instance->m_buttonManager) {
        std::cerr << "deleteButton: 按钮管理器未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "按钮管理器未初始化");
    }
    
    int32_t buttonId;
    
    if (JS_ToInt32(ctx, &buttonId, argv[0]) < 0) {
        std::cerr << "deleteButton: 按钮ID必须是整数" << std::endl;
        return JS_ThrowTypeError(ctx, "按钮ID必须是整数");
    }
    
    bool result = s_instance->m_buttonManager->deleteButton(buttonId);
    return result ? JS_TRUE : JS_FALSE;
}

// JavaScript 可调用的函数：清除所有按钮
JSValue QuickJSVulkanBridge::js_clearAllButtons(JSContext* ctx, JSValueConst this_val,
                                                  int argc, JSValueConst* argv) {
    if (!s_instance || !s_instance->m_buttonManager) {
        std::cerr << "clearAllButtons: 按钮管理器未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "按钮管理器未初始化");
    }
    
    s_instance->m_buttonManager->clearAllButtons();
    return JS_TRUE;
}

// JavaScript 可调用的函数：清空模型
JSValue QuickJSVulkanBridge::js_clearModel(JSContext* ctx, JSValueConst this_val,
                                              int argc, JSValueConst* argv) {
    if (!s_instance || !s_instance->m_modelRenderer) {
        std::cerr << "clearModel: 渲染器未初始化" << std::endl;
        return JS_ThrowInternalError(ctx, "渲染器未初始化");
    }
    
    std::cout << "JavaScript 调用: clearModel()" << std::endl;
    
    // 调用渲染器的清空模型方法
    s_instance->m_modelRenderer->clearModel();
    
    // 清空桥接模块中的模型数据
    s_instance->clearModel();
    
    return JS_TRUE;
}

// 初始化 QuickJS-Vulkan 桥接模块
bool initQuickJSVulkanBridge(JSContext* ctx, JSRuntime* rt) {
    static QuickJSVulkanBridge bridge;
    return bridge.initialize(ctx, rt);
}

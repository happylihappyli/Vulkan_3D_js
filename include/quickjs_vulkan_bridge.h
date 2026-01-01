#pragma once

#include <string>
#include <vector>
#include "../quickjs/quickjs.h"

namespace ScriptViewer {
    class ModelRenderer3D;
}

// QuickJS-Vulkan 桥接模块
// 提供 JavaScript 调用 C++ 模型加载功能的接口
class QuickJSVulkanBridge {
private:
    JSContext* m_context;
    JSRuntime* m_runtime;
    ScriptViewer::ModelRenderer3D* m_modelRenderer; // 3D渲染器指针
    
    // 存储加载的模型数据
    struct ModelData {
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> texCoords;
        std::vector<unsigned int> indices;
    };
    
    ModelData m_currentModel;
    
public:
    QuickJSVulkanBridge();
    ~QuickJSVulkanBridge();
    
    // 初始化桥接模块，注册 JavaScript API
    bool initialize(JSContext* ctx, JSRuntime* rt);
    
    // 设置3D渲染器
    void setModelRenderer(ScriptViewer::ModelRenderer3D* renderer) {
        m_modelRenderer = renderer;
    }
    
    // 注册所有 JavaScript 函数
    void registerAPIs();
    
    // JavaScript 可调用的函数：加载 OBJ 模型
    static JSValue js_loadOBJModel(JSContext* ctx, JSValueConst this_val,
                                    int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：获取顶点数量
    static JSValue js_getVertexCount(JSContext* ctx, JSValueConst this_val,
                                      int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：获取面数量
    static JSValue js_getFaceCount(JSContext* ctx, JSValueConst this_val,
                                    int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：获取顶点数据
    static JSValue js_getVertices(JSContext* ctx, JSValueConst this_val,
                                   int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：获取法线数据
    static JSValue js_getNormals(JSContext* ctx, JSValueConst this_val,
                                   int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：打印日志
    static JSValue js_log(JSContext* ctx, JSValueConst this_val,
                           int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：console.log 实现
    static JSValue js_console_log(JSContext* ctx, JSValueConst this_val,
                                    int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：执行数学计算
    static JSValue js_calculate(JSContext* ctx, JSValueConst this_val,
                                 int argc, JSValueConst* argv);
    
    // 获取当前模型数据
    const ModelData& getCurrentModel() const { return m_currentModel; }
    
    // 清空模型数据
    void clearModel();
    
private:
    // 获取桥接实例（用于静态函数访问成员变量）
    static QuickJSVulkanBridge* s_instance;
    
    // 设置桥接实例
    static void setInstance(QuickJSVulkanBridge* instance) {
        s_instance = instance;
    }
};

// 初始化 QuickJS-Vulkan 桥接模块
bool initQuickJSVulkanBridge(JSContext* ctx, JSRuntime* rt);

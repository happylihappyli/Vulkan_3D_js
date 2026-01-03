#pragma once

#include <string>
#include <vector>
#include "../quickjs/quickjs.h"

namespace ScriptViewer {
    class ModelRenderer3D;
    class ButtonManager;
    class ScriptViewerApp;
}

// QuickJS-Vulkan 桥接模块
// 提供 JavaScript 调用 C++ 模型加载功能的接口
class QuickJSVulkanBridge {
private:
    JSContext* m_context;
    JSRuntime* m_runtime;
    ScriptViewer::ModelRenderer3D* m_modelRenderer; // 3D渲染器指针
    ScriptViewer::ButtonManager* m_buttonManager;   // 按钮管理器指针
    ScriptViewer::ScriptViewerApp* m_app;           // 应用程序指针（用于输出到UI）
    
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
    
    // 设置按钮管理器
    void setButtonManager(ScriptViewer::ButtonManager* manager) {
        m_buttonManager = manager;
    }
    
    // 设置应用程序指针
    void setApp(ScriptViewer::ScriptViewerApp* app) {
        m_app = app;
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
    
    // JavaScript 可调用的函数：Math.min
    static JSValue js_math_min(JSContext* ctx, JSValueConst this_val,
                                int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：执行数学计算
    static JSValue js_calculate(JSContext* ctx, JSValueConst this_val,
                                 int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：创建按钮
    static JSValue js_createButton(JSContext* ctx, JSValueConst this_val,
                                     int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：设置按钮位置
    static JSValue js_setButtonPosition(JSContext* ctx, JSValueConst this_val,
                                          int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：设置按钮大小
    static JSValue js_setButtonSize(JSContext* ctx, JSValueConst this_val,
                                     int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：设置按钮颜色
    static JSValue js_setButtonColor(JSContext* ctx, JSValueConst this_val,
                                      int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：设置按钮可见性
    static JSValue js_setButtonVisible(JSContext* ctx, JSValueConst this_val,
                                        int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：设置按钮启用状态
    static JSValue js_setButtonEnabled(JSContext* ctx, JSValueConst this_val,
                                        int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：设置按钮点击事件
    static JSValue js_setButtonClickHandler(JSContext* ctx, JSValueConst this_val,
                                             int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：删除按钮
    static JSValue js_deleteButton(JSContext* ctx, JSValueConst this_val,
                                    int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：清除所有按钮
    static JSValue js_clearAllButtons(JSContext* ctx, JSValueConst this_val,
                                        int argc, JSValueConst* argv);
    
    // JavaScript 可调用的函数：清空模型
    static JSValue js_clearModel(JSContext* ctx, JSValueConst this_val,
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

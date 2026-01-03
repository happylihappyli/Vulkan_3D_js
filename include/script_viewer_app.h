#pragma once

#include <string>
#include <memory>

struct GLFWwindow;
struct JSContext;
struct JSRuntime;

namespace ScriptViewer {

class ModelRenderer3D;
class ButtonManager;

} // namespace ScriptViewer

class QuickJSVulkanBridge;

namespace ScriptViewer {

class ScriptViewerApp {
public:
    ScriptViewerApp();
    ~ScriptViewerApp();
    
    int run();
    
    void setInitialScript(const std::string& path);
    
    // 添加日志输出（供JavaScript console.log使用）
    void addLog(const std::string& message);
    
private:
    GLFWwindow* m_window;
    JSRuntime* m_runtime;
    JSContext* m_context;
    
    int m_width;
    int m_height;
    
    std::string m_scriptPath;
    std::string m_initialScriptPath;
    std::string m_scriptContent;
    std::string m_outputLog;
    
    // 文本框内容
    std::string m_textBoxContent;
    
    bool m_showFileDialog;
    bool m_scriptExecuted;
    
    std::unique_ptr<ModelRenderer3D> m_modelRenderer;
    std::unique_ptr<ButtonManager> m_buttonManager;
    std::unique_ptr<QuickJSVulkanBridge> m_vulkanBridge;
    
    bool initializeWindow();
    bool initializeImGui();
    bool initializeQuickJS();
    bool initialize3DRenderer();
    void cleanup();
    
    void mainLoop();
    void renderUI();
    void render3DView();
    
    void executeScript();
    void loadScriptFile(const std::string& path);
    
    static void glfwErrorCallback(int error, const char* description);
};

} // namespace ScriptViewer

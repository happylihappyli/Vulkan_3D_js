#include "script_viewer_app.h"
#include "quickjs_vulkan_bridge.h"
#include "model_renderer_3d.h"
#include "button_manager.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <commdlg.h>
#include <quickjs.h>
#include <quickjs-libc.h>
#include <cstdio>

namespace ScriptViewer {

ScriptViewerApp::ScriptViewerApp()
    : m_window(nullptr)
    , m_runtime(nullptr)
    , m_context(nullptr)
    , m_width(1280)
    , m_height(720)
    , m_showFileDialog(false)
    , m_scriptExecuted(false)
    , m_modelRenderer(std::make_unique<ModelRenderer3D>())
    , m_buttonManager(std::make_unique<ButtonManager>())
{
}

ScriptViewerApp::~ScriptViewerApp() {
    cleanup();
}

void ScriptViewerApp::setInitialScript(const std::string& path) {
    m_initialScriptPath = path;
}

int ScriptViewerApp::run() {
    if (!initializeWindow()) {
        return -1;
    }
    
    if (!initializeImGui()) {
        cleanup();
        return -1;
    }
    
    if (!initializeQuickJS()) {
        cleanup();
        return -1;
    }
    
    if (!initialize3DRenderer()) {
        cleanup();
        return -1;
    }
    
    if (!m_initialScriptPath.empty()) {
        printf("[调试] 加载初始脚本: %s\n", m_initialScriptPath.c_str());
        loadScriptFile(m_initialScriptPath);
        printf("[调试] 脚本已加载，将在窗口显示后执行\n");
    }
    
    mainLoop();
    
    return 0;
}

bool ScriptViewerApp::initializeWindow() {
    printf("[调试] 开始初始化窗口...\n");
    
    glfwSetErrorCallback(glfwErrorCallback);
    
    printf("[调试] 调用glfwInit()...\n");
    if (!glfwInit()) {
        printf("[调试] glfwInit()失败！\n");
        return false;
    }
    printf("[调试] glfwInit()成功\n");
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    
    printf("[调试] 创建窗口...\n");
    m_window = glfwCreateWindow(m_width, m_height, "JavaScript脚本查看器", nullptr, nullptr);
    if (!m_window) {
        printf("[调试] 窗口创建失败！\n");
        glfwTerminate();
        return false;
    }
    printf("[调试] 窗口创建成功\n");
    
    printf("[调试] 设置OpenGL上下文...\n");
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    
    printf("[调试] 初始化GLAD...\n");
    if (!gladLoadGL()) {
        printf("[调试] GLAD初始化失败！\n");
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }
    printf("[调试] GLAD初始化成功\n");
    
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    printf("[调试] 深度测试已启用\n");
    
    return true;
}

bool ScriptViewerApp::initializeImGui() {
    printf("[调试] 开始初始化ImGui...\n");
    
    IMGUI_CHECKVERSION();
    printf("[调试] ImGui版本检查通过\n");
    
    ImGui::CreateContext();
    printf("[调试] ImGui上下文创建成功\n");
    
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui::StyleColorsDark();
    printf("[调试] ImGui样式设置完成\n");
    
    printf("[调试] 加载中文字体...\n");
    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 2;
    config.PixelSnapH = false;
    
    const char* fontPath = "C:\\Windows\\Fonts\\msyh.ttc";
    io.Fonts->AddFontFromFileTTF(fontPath, 18.0f, &config, io.Fonts->GetGlyphRangesChineseFull());
    printf("[调试] 中文字体加载成功\n");
    
    printf("[调试] 初始化ImGui GLFW后端...\n");
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    printf("[调试] ImGui GLFW后端初始化成功\n");
    
    printf("[调试] 初始化ImGui OpenGL3后端...\n");
    ImGui_ImplOpenGL3_Init("#version 330");
    printf("[调试] ImGui OpenGL3后端初始化成功\n");
    
    return true;
}

bool ScriptViewerApp::initializeQuickJS() {
    printf("[调试] 开始初始化QuickJS...\n");
    
    printf("[调试] 创建QuickJS运行时...\n");
    m_runtime = JS_NewRuntime();
    if (!m_runtime) {
        printf("[调试] QuickJS运行时创建失败！\n");
        return false;
    }
    printf("[调试] QuickJS运行时创建成功\n");
    
    printf("[调试] 创建QuickJS上下文...\n");
    m_context = JS_NewContext(m_runtime);
    if (!m_context) {
        printf("[调试] QuickJS上下文创建失败！\n");
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    printf("[调试] QuickJS上下文创建成功\n");
    
    // 初始化QuickJS标准库
    js_std_init_handlers(m_runtime);
    js_std_add_helpers(m_context, -1, nullptr);
    printf("[调试] QuickJS标准库初始化成功\n");
    
    // 初始化QuickJS内置对象，支持完整的JavaScript语法
    if (JS_AddIntrinsicBaseObjects(m_context) < 0) {
        printf("[调试] QuickJS基础对象初始化失败！\n");
        JS_FreeContext(m_context);
        m_context = nullptr;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    if (JS_AddIntrinsicDate(m_context) < 0) {
        printf("[调试] QuickJS日期对象初始化失败！\n");
        JS_FreeContext(m_context);
        m_context = nullptr;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    if (JS_AddIntrinsicEval(m_context) < 0) {
        printf("[调试] QuickJS eval初始化失败！\n");
        JS_FreeContext(m_context);
        m_context = nullptr;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    if (JS_AddIntrinsicStringNormalize(m_context) < 0) {
        printf("[调试] QuickJS字符串规范化初始化失败！\n");
        JS_FreeContext(m_context);
        m_context = nullptr;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    JS_AddIntrinsicRegExpCompiler(m_context);
    if (JS_AddIntrinsicRegExp(m_context) < 0) {
        printf("[调试] QuickJS正则表达式初始化失败！\n");
        JS_FreeContext(m_context);
        m_context = nullptr;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    if (JS_AddIntrinsicJSON(m_context) < 0) {
        printf("[调试] QuickJS JSON初始化失败！\n");
        JS_FreeContext(m_context);
        m_context = nullptr;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    if (JS_AddIntrinsicProxy(m_context) < 0) {
        printf("[调试] QuickJS Proxy初始化失败！\n");
        JS_FreeContext(m_context);
        m_context = nullptr;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    if (JS_AddIntrinsicMapSet(m_context) < 0) {
        printf("[调试] QuickJS Map/Set初始化失败！\n");
        JS_FreeContext(m_context);
        m_context = nullptr;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    if (JS_AddIntrinsicTypedArrays(m_context) < 0) {
        printf("[调试] QuickJS类型化数组初始化失败！\n");
        JS_FreeContext(m_context);
        m_context = nullptr;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    if (JS_AddIntrinsicPromise(m_context) < 0) {
        printf("[调试] QuickJS Promise初始化失败！\n");
        JS_FreeContext(m_context);
        m_context = nullptr;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    if (JS_AddIntrinsicWeakRef(m_context) < 0) {
        printf("[调试] QuickJS WeakRef初始化失败！\n");
        JS_FreeContext(m_context);
        m_context = nullptr;
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
        return false;
    }
    printf("[调试] QuickJS内置对象初始化成功\n");
    
    // 创建Vulkan桥接对象
    m_vulkanBridge = std::make_unique<QuickJSVulkanBridge>();
    printf("[调试] 初始化Vulkan桥接...\n");
    m_vulkanBridge->initialize(m_context, m_runtime);
    m_vulkanBridge->setModelRenderer(m_modelRenderer.get());
    m_vulkanBridge->setButtonManager(m_buttonManager.get());
    m_vulkanBridge->setApp(this); // 设置应用程序指针，用于输出到UI
    
    // 为ButtonManager设置JSContext（用于调用JavaScript函数）
    m_buttonManager->setJSContext(m_context);
    
    printf("[调试] Vulkan桥接初始化成功\n");
    
    return true;
}

bool ScriptViewerApp::initialize3DRenderer() {
    printf("[调试] 开始初始化3D渲染器...\n");
    
    if (!m_modelRenderer) {
        printf("[调试] 3D渲染器未创建！\n");
        return false;
    }
    
    if (!m_modelRenderer->initialize()) {
        printf("[调试] 3D渲染器初始化失败！\n");
        return false;
    }
    
    printf("[调试] 3D渲染器初始化成功\n");
    return true;
}

void ScriptViewerApp::cleanup() {
    if (m_context) {
        JS_FreeContext(m_context);
        m_context = nullptr;
    }
    
    if (m_runtime) {
        js_std_free_handlers(m_runtime);
        JS_FreeRuntime(m_runtime);
        m_runtime = nullptr;
    }
    
    if (m_window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(m_window);
        glfwTerminate();
        m_window = nullptr;
    }
}

void ScriptViewerApp::mainLoop() {
    printf("[调试] mainLoop()函数开始执行\n");
    
    int frameCount = 0;
    while (!glfwWindowShouldClose(m_window)) {
        frameCount++;
        if (frameCount % 60 == 0) {
            printf("[调试] mainLoop()运行中，帧数: %d\n", frameCount);
        }
        
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // printf("[调试] 调用renderUI()\n");
        renderUI();
        
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(m_window);
        
        if (!m_scriptExecuted && !m_initialScriptPath.empty()) {
            printf("[调试] 执行初始脚本\n");
            executeScript();
            m_scriptExecuted = true;
            printf("[调试] 脚本执行完成，继续运行主循环\n");
        }
    }
    
    printf("[调试] mainLoop()函数结束，总帧数: %d\n", frameCount);
}

void ScriptViewerApp::renderUI() {
    ImGui::SetNextWindowPos(ImVec2(820, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(450, 600), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("JavaScript脚本查看器");
    
    ImGui::Text("选择JavaScript脚本文件:");
    
    if (ImGui::Button("打开文件")) {
        OPENFILENAMEA ofn;
        char szFile[260] = {0};
        
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = glfwGetWin32Window(m_window);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "JavaScript Files (*.js)\0*.js\0All Files (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        
        if (GetOpenFileNameA(&ofn)) {
            loadScriptFile(szFile);
        }
    }
    
    ImGui::Separator();
    
    if (!m_scriptPath.empty()) {
        ImGui::Text("当前文件: %s", m_scriptPath.c_str());
        
        if (ImGui::Button("执行脚本")) {
            executeScript();
        }
        
        ImGui::Separator();
        
        ImGui::Text("脚本内容:");
        if (ImGui::BeginChild("ScriptContent", ImVec2(0, 200), true)) {
            ImGui::TextWrapped("%s", m_scriptContent.c_str());
            ImGui::EndChild();
        }
    }
    
    ImGui::Separator();
    
    ImGui::Text("执行输出:");
    if (ImGui::BeginChild("OutputLog", ImVec2(0, 200), true)) {
        ImGui::TextWrapped("%s", m_outputLog.c_str());
        ImGui::EndChild();
    }
    
    ImGui::Separator();
    
    // 添加文本框
    ImGui::Text("文本输入:");
    char textBoxBuffer[1024] = {0};
    strncpy_s(textBoxBuffer, sizeof(textBoxBuffer), m_textBoxContent.c_str(), _TRUNCATE);
    if (ImGui::InputTextMultiline("##TextBox", textBoxBuffer, sizeof(textBoxBuffer), ImVec2(0, 100))) {
        m_textBoxContent = textBoxBuffer;
    }
    
    ImGui::End();
    
    // 渲染JavaScript创建的按钮
    if (m_buttonManager) {
        // 创建一个专门的窗口来显示按钮
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
        
        ImGui::Begin("JavaScript按钮", nullptr, ImGuiWindowFlags_NoCollapse);
        
        // 渲染所有按钮
        m_buttonManager->render();
        
        ImGui::End();
    }
    
    // 渲染3D视图窗口
    render3DView();
}

void ScriptViewerApp::executeScript() {
    if (!m_context || m_scriptContent.empty()) {
        return;
    }
    
    m_outputLog.clear();
    m_outputLog += "开始执行脚本...\n";
    
    JSValue result = JS_Eval(m_context, m_scriptContent.c_str(), 
                            m_scriptContent.length(), 
                            m_scriptPath.c_str(), 
                            JS_EVAL_TYPE_GLOBAL);
    
    if (JS_IsException(result)) {
        JSValue exception = JS_GetException(m_context);
        const char* error = JS_ToCString(m_context, exception);
        if (error) {
            std::string errorMsg = "错误: " + std::string(error);
            m_outputLog += errorMsg + "\n";
            
            // 输出错误到文件
            std::ofstream errorFile("test/script_error.log");
            if (errorFile.is_open()) {
                errorFile << "脚本执行错误:\n";
                errorFile << "脚本文件: " << m_scriptPath << "\n";
                errorFile << "错误信息: " << error << "\n";
                errorFile << "脚本内容:\n" << m_scriptContent << "\n";
                errorFile.close();
                printf("[调试] 错误信息已写入 test/script_error.log\n");
            }
            
            JS_FreeCString(m_context, error);
        }
        JS_FreeValue(m_context, exception);
    } else {
        m_outputLog += "脚本执行成功！\n";
    }
    
    JS_FreeValue(m_context, result);
}

void ScriptViewerApp::loadScriptFile(const std::string& path) {
    printf("[调试] 开始加载脚本文件: %s\n", path.c_str());
    m_scriptPath = path;
    m_scriptContent.clear();
    
    std::ifstream file(path, std::ios::binary);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        m_scriptContent = buffer.str();
        file.close();
        
        printf("[调试] 脚本文件大小: %zu 字节\n", m_scriptContent.size());
        
        // 跳过UTF-8 BOM标记（如果存在）
        if (m_scriptContent.size() >= 3 && 
            (unsigned char)m_scriptContent[0] == 0xEF &&
            (unsigned char)m_scriptContent[1] == 0xBB &&
            (unsigned char)m_scriptContent[2] == 0xBF) {
            m_scriptContent = m_scriptContent.substr(3);
            printf("[调试] 检测到UTF-8 BOM，已跳过\n");
        }
        
        printf("[调试] 脚本内容: %s\n", m_scriptContent.c_str());
        m_outputLog = "文件加载成功: " + path + "\n";
        printf("[调试] 脚本加载成功\n");
    } else {
        printf("[调试] 无法打开文件: %s\n", path.c_str());
        m_outputLog = "无法打开文件: " + path + "\n";
    }
}

void ScriptViewerApp::render3DView() {
    // printf("[调试] render3DView()函数开始执行\n");
    
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    
    // 设置窗口没有内边距，以便背景图填满
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("3D模型视图", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    ImVec2 winSize = ImGui::GetContentRegionAvail();
    if (winSize.x <= 0 || winSize.y <= 0) {
        ImGui::End();
        ImGui::PopStyleVar();
        return;
    }
    
    // 1. 渲染 3D 场景到纹理
    int width = static_cast<int>(winSize.x);
    int height = static_cast<int>(winSize.y);
    
    if (m_modelRenderer) {
        // 处理鼠标输入 (Camera Control)
        if (ImGui::IsWindowHovered()) {
            ImGuiIO& io = ImGui::GetIO();
            
            // 滚轮缩放
            if (io.MouseWheel != 0.0f) {
                m_modelRenderer->moveCamera(-io.MouseWheel * 1.0f, 0, 0, 0, 0);
            }
            
            // 右键旋转
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
                ImVec2 delta = io.MouseDelta;
                m_modelRenderer->moveCamera(0, delta.y * 0.5f, delta.x * 0.5f, 0, 0);
            }
            
            // 中键平移
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
                ImVec2 delta = io.MouseDelta;
                // 平移速度随距离变化，以保持自然的手感
                float panSpeed = m_modelRenderer->getCameraDist() * 0.002f;
                m_modelRenderer->moveCamera(0, 0, 0, delta.x * panSpeed, -delta.y * panSpeed);
            }
        }
        
        m_modelRenderer->render(width, height);
        
        GLuint textureId = m_modelRenderer->getTextureID();
        if (textureId) {
            // 将光标移到左上角，绘制背景图
            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::Image((void*)(intptr_t)textureId, 
                         winSize, 
                         ImVec2(0, 1), 
                         ImVec2(1, 0));
        }
    }
    
    // 2. 绘制悬浮 UI 控件
    // 恢复 Padding 以便绘制控件
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    
    // 创建一个半透明的浮动面板
    ImGui::SetCursorPos(ImVec2(10, 10)); // 左上角
    ImGui::BeginChild("ControlsPanel", ImVec2(250, 0), true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
    
    // 给面板加一个半透明背景
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 p_min = ImGui::GetItemRectMin();
    ImVec2 p_max = ImGui::GetItemRectMax();
    // 重新计算背景矩形（因为 BeginChild NoBackground 不会画背景，我们自己画一个更好看的）
    p_min = ImGui::GetCursorScreenPos();
    p_max = ImVec2(p_min.x + 250, p_min.y + 350); // 估算高度
    drawList->AddRectFilled(p_min, p_max, IM_COL32(0, 0, 0, 150), 5.0f); // 半透明黑色圆角背景
    
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "控制面板");
    ImGui::Separator();
    
    // 模型加载和清空按钮
    if (ImGui::Button("加载OBJ模型", ImVec2(-1, 0))) {
        OPENFILENAMEA ofn;
        char szFile[260] = {0};
        
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = glfwGetWin32Window(m_window);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "OBJ Files (*.obj)\0*.obj\0All Files (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        
        if (GetOpenFileNameA(&ofn)) {
            if (m_modelRenderer && m_modelRenderer->loadOBJModel(szFile)) {
                m_outputLog += "3D模型加载成功: " + std::string(szFile) + "\n";
            } else {
                m_outputLog += "3D模型加载失败: " + std::string(szFile) + "\n";
            }
        }
    }
    
    // 清空模型按钮
    if (ImGui::Button("清空模型", ImVec2(-1, 0))) {
        if (m_modelRenderer) {
            m_modelRenderer->clearModel();
            m_outputLog += "模型已清空\n";
        }
    }
    
    if (m_modelRenderer) {
        bool wireframe = m_modelRenderer->isWireframeMode();
        if (ImGui::Checkbox("线框模式", &wireframe)) {
            m_modelRenderer->setWireframeMode(wireframe);
        }
        
        ImGui::Separator();
        ImGui::Text("模型变换:");
        
        float rotX = m_modelRenderer->getRotationX();
        float rotY = m_modelRenderer->getRotationY();
        float rotZ = m_modelRenderer->getRotationZ();
        
        ImGui::PushItemWidth(120);
        ImGui::SliderFloat("旋转 X", &rotX, 0.0f, 360.0f);
        ImGui::SliderFloat("旋转 Y", &rotY, 0.0f, 360.0f);
        ImGui::SliderFloat("旋转 Z", &rotZ, 0.0f, 360.0f);
        ImGui::PopItemWidth();
        
        m_modelRenderer->setRotation(rotX, rotY, rotZ);
        
        float scale = m_modelRenderer->getScale();
        ImGui::PushItemWidth(120);
        ImGui::SliderFloat("缩放", &scale, 0.1f, 5.0f);
        ImGui::PopItemWidth();
        m_modelRenderer->setScale(scale);
        
        ImGui::Separator();
        ImGui::Text("摄像机:");
        ImGui::Text("操作说明:\n - 右键拖拽: 旋转\n - 中键拖拽: 平移\n - 滚轮: 缩放");
        
        ImGui::Text("距离: %.1f", m_modelRenderer->getCameraDist());
        ImGui::Text("角度: (%.1f, %.1f)", m_modelRenderer->getCameraRotX(), m_modelRenderer->getCameraRotY());
        
        if (m_modelRenderer->isModelLoaded()) {
            ImGui::Separator();
            ImGui::Text("模型信息:");
            ImGui::Text("顶点: %zu, 面: %zu", 
                m_modelRenderer->getVertexCount(), 
                m_modelRenderer->getFaceCount());
            ImGui::Text("材质数: %zu, 子网格数: %zu", 
                m_modelRenderer->getMaterialCount(),
                m_modelRenderer->getSubMeshCount());
            
            ImGui::Separator();
            if (ImGui::CollapsingHeader("材质编辑", ImGuiTreeNodeFlags_DefaultOpen)) {
                size_t matCount = m_modelRenderer->getMaterialCount();
                for (int i = 0; i < matCount; ++i) {
                    Material* mat = m_modelRenderer->getMaterial(i);
                    if (mat) {
                        ImGui::PushID(i);
                        if (ImGui::TreeNode(mat->name.c_str())) {
                            ImGui::ColorEdit3("颜色(Diffuse)", mat->diffuse);
                            // 暂时不编辑 Ambient 和 Specular，以免界面太乱
                            ImGui::TreePop();
                        }
                        ImGui::PopID();
                    }
                }
            }
        }
    }
    
    ImGui::EndChild();
    ImGui::PopStyleVar(); // Pop child padding
    
    ImGui::End();
    ImGui::PopStyleVar(); // Pop window padding
}

void ScriptViewerApp::glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW错误 %d: %s\n", error, description);
}

void ScriptViewerApp::addLog(const std::string& message) {
    m_outputLog += message + "\n";
}

} // namespace ScriptViewer

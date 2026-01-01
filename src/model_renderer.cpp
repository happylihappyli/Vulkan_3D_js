#include "model_renderer.h"
#include <cstdio>
#include "utils.h"
#include "gl_extensions.h"

ModelRenderer::ModelRenderer() 
    : m_modelVBO(0), m_modelVAO(0), m_modelEBO(0), 
      m_modelRenderingInitialized(false), m_modelWireframeMode(true), 
      m_modelScale(1.0f), m_modelRotationX(0.0f), m_modelRotationY(0.0f),
      m_jsModelLoaded(false) {
    
    m_modelLoader = std::make_unique<ModelLoaderJS>();
    
#ifdef ENABLE_QUICKJS
    // 初始化 QuickJS 引擎
    m_jsEngine = std::make_unique<QuickJSEngine>();
    if (m_jsEngine->init()) {
        wprintf(L"QuickJS 引擎初始化成功\n");
        logToFile(L"QuickJS 引擎初始化成功");
    } else {
        wprintf(L"QuickJS 引擎初始化失败\n");
        logToFile(L"QuickJS 引擎初始化失败");
    }
#endif
    
    initGLExtensions();
}

ModelRenderer::~ModelRenderer() {
    cleanupModelResources();
}

bool ModelRenderer::loadJSModel(const std::string& filename) {
    if (!m_modelLoader) {
        wprintf(L"模型加载器未初始化\n");
        return false;
    }
    
    wprintf(L"正在加载OBJ模型文件: %hs\n", filename.c_str());
    logToFile(L"正在加载OBJ模型文件: " + std::wstring(filename.begin(), filename.end()));
    
    if (m_modelLoader->loadOBJModel(filename)) {
        m_jsModelLoaded = true;
        wprintf(L"OBJ模型加载成功\n");
        logToFile(L"OBJ模型加载成功");
        
        if (!initModelRendering()) {
            wprintf(L"模型渲染初始化失败\n");
            logToFile(L"模型渲染初始化失败");
            return false;
        }
        
        return true;
    } else {
        wprintf(L"OBJ模型加载失败\n");
        logToFile(L"OBJ模型加载失败");
        return false;
    }
}

bool ModelRenderer::initModelRendering() {
    if (!m_jsModelLoaded || !m_modelLoader) {
        wprintf(L"模型未加载或模型加载器未初始化\n");
        return false;
    }
    
    auto vertices = m_modelLoader->getVertices();
    auto indices = m_modelLoader->getIndices();
    
    if (vertices.empty() || indices.empty()) {
        wprintf(L"模型数据为空，无法初始化渲染\n");
        return false;
    }
    
    wprintf(L"初始化模型渲染: 顶点数=%zu, 面数=%zu\n", vertices.size()/3, indices.size()/3);
    
    glGenVertexArrays(1, &m_modelVAO);
    glBindVertexArray(m_modelVAO);
    
    glGenBuffers(1, &m_modelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_modelVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glGenBuffers(1, &m_modelEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_modelEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    
    m_modelRenderingInitialized = true;
    wprintf(L"模型渲染初始化完成\n");
    return true;
}

void ModelRenderer::renderModel() {
    if (!m_jsModelLoaded || !m_modelRenderingInitialized) {
        return;
    }
    
    glPushMatrix();
    
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(m_modelRotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(m_modelRotationY, 0.0f, 1.0f, 0.0f);
    glScalef(m_modelScale, m_modelScale, m_modelScale);
    
    if (m_modelWireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(0.0f, 1.0f, 0.0f);
        glLineWidth(1.5f);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3f(0.0f, 0.8f, 1.0f);
        glEnable(GL_LIGHTING);
    }
    
    glBindVertexArray(m_modelVAO);
    glDrawElements(GL_TRIANGLES, m_modelLoader->getIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glPopMatrix();
}

void ModelRenderer::toggleModelWireframeMode() {
    m_modelWireframeMode = !m_modelWireframeMode;
    wprintf(L"模型渲染模式: %hs\n", m_modelWireframeMode ? "线框模式" : "实体模式");
    logToFile(m_modelWireframeMode ? L"模型渲染模式: 线框模式" : L"模型渲染模式: 实体模式");
}

void ModelRenderer::adjustModelScale(float delta) {
    m_modelScale += delta;
    if (m_modelScale < 0.1f) m_modelScale = 0.1f;
    if (m_modelScale > 10.0f) m_modelScale = 10.0f;
    wprintf(L"模型缩放: %.2f\n", m_modelScale);
}

void ModelRenderer::adjustModelRotation(float deltaX, float deltaY) {
    m_modelRotationX += deltaX;
    m_modelRotationY += deltaY;
    wprintf(L"模型旋转: X=%.1f, Y=%.1f\n", m_modelRotationX, m_modelRotationY);
}

bool ModelRenderer::executeJSCode(const std::string& code) {
#ifdef ENABLE_QUICKJS
    if (!m_jsEngine) {
        wprintf(L"QuickJS 引擎未初始化\n");
        return false;
    }
    
    wprintf(L"执行 JavaScript 代码:\n%hs\n", code.c_str());
    logToFile(L"执行 JavaScript 代码");
    
    bool success = m_jsEngine->executeCode(code);
    if (success) {
        wprintf(L"JavaScript 代码执行成功\n");
        logToFile(L"JavaScript 代码执行成功");
    } else {
        wprintf(L"JavaScript 代码执行失败\n");
        logToFile(L"JavaScript 代码执行失败");
    }
    
    return success;
#else
    wprintf(L"QuickJS 未启用，无法执行 JavaScript 代码\n");
    return false;
#endif
}

// 加载并执行 JavaScript 脚本文件
bool ModelRenderer::loadJSScript(const std::string& filename) {
#ifdef ENABLE_QUICKJS
    if (!m_jsEngine) {
        wprintf(L"QuickJS 引擎未初始化\n");
        return false;
    }
    
    wprintf(L"正在加载 JavaScript 脚本文件: %hs\n", filename.c_str());
    logToFile(L"正在加载 JavaScript 脚本文件: " + std::wstring(filename.begin(), filename.end()));
    
    bool success = m_jsEngine->executeFile(filename);
    if (success) {
        wprintf(L"JavaScript 脚本执行成功\n");
        logToFile(L"JavaScript 脚本执行成功");
    } else {
        wprintf(L"JavaScript 脚本执行失败\n");
        logToFile(L"JavaScript 脚本执行失败");
    }
    
    return success;
#else
    wprintf(L"QuickJS 未启用，无法加载 JavaScript 脚本\n");
    return false;
#endif
}

bool ModelRenderer::isModelLoaded() const {
    return m_jsModelLoaded;
}

void ModelRenderer::cleanupModelResources() {
    if (m_modelRenderingInitialized) {
        if (glDeleteBuffers) {
            glDeleteBuffers(1, &m_modelVBO);
            glDeleteBuffers(1, &m_modelEBO);
        }
        if (glDeleteVertexArrays) {
            glDeleteVertexArrays(1, &m_modelVAO);
        }
        m_modelRenderingInitialized = false;
    }
}

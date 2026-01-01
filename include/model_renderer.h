#pragma once

#include <memory>
#include <string>
#include <windows.h>
#include <GL/gl.h>
#include "pointcloud.h"
#include "model_loader_js.h"

#ifdef ENABLE_QUICKJS
#include "quickjs_engine.h"
#endif

class ModelRenderer {
private:
    GLuint m_modelVBO;
    GLuint m_modelVAO;
    GLuint m_modelEBO;
    bool m_modelRenderingInitialized;
    bool m_modelWireframeMode;
    float m_modelScale;
    float m_modelRotationX;
    float m_modelRotationY;
    
    std::unique_ptr<ModelLoaderJS> m_modelLoader;
    bool m_jsModelLoaded;
    
#ifdef ENABLE_QUICKJS
    std::unique_ptr<QuickJSEngine> m_jsEngine;  // QuickJS 引擎
#endif
    
    bool initModelRendering();
    void cleanupModelResources();
    
public:
    ModelRenderer();
    ~ModelRenderer();
    
    bool loadJSModel(const std::string& filename);
    void renderModel();
    
    void toggleModelWireframeMode();
    void adjustModelScale(float delta);
    void adjustModelRotation(float deltaX, float deltaY);
    
    bool executeJSCode(const std::string& code);
    bool loadJSScript(const std::string& filename);  // 加载 JS 脚本文件
    bool isModelLoaded() const;
};

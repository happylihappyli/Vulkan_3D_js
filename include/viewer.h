#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "camera.h"
#include "pointcloud.h"
#include "model_renderer.h"

class PointCloudViewer {
private:
    HDC m_hdc;
    HGLRC m_hglrc;
    HWND m_hwnd;
    Camera m_camera;
    PointCloud m_pointCloud;
    bool m_mouseDown;
    int m_lastMouseX, m_lastMouseY;
    int m_windowWidth, m_windowHeight;
    float m_lastDistance;
    
    GLuint m_vbo;
    GLuint m_vao;
    bool m_gpuOptimized;
    
    ModelRenderer m_modelRenderer;
    
    bool initGPURendering();
    void updateGPUBuffer();
    void renderGPUOptimized();
    void renderTraditional();
    void cleanupGPUResources();
    
public:
    PointCloudViewer(HWND hwnd);
    ~PointCloudViewer();
    
    bool initialize();
    void cleanup();
    void updateViewport(int width, int height);
    void render();
    
    void onMouseDown(int x, int y);
    void onMouseUp();
    void onMouseMove(int x, int y);
    void onMouseWheel(int delta);
    
    bool loadJSModel(const std::string& filename);
    void toggleModelWireframeMode();
    void adjustModelScale(float delta);
    void adjustModelRotation(float deltaX, float deltaY);
    bool executeJSCode(const std::string& code);
    bool loadJSScript(const std::string& filename);  // 加载 JS 脚本文件
};

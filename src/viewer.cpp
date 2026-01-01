#include "viewer.h"
#include <cstdio>
#include "utils.h"
#include "gl_extensions.h"

PointCloudViewer::PointCloudViewer(HWND hwnd) 
    : m_hwnd(hwnd), m_hdc(nullptr), m_hglrc(nullptr), m_vbo(0), m_vao(0), m_gpuOptimized(false) {
    m_mouseDown = false;
    m_lastMouseX = 0;
    m_lastMouseY = 0;
    m_windowWidth = 800;
    m_windowHeight = 600;
    m_lastDistance = m_camera.getDistance();
    
    wprintf(L"点云查看器创建完成，原始点数: %zu\n", m_pointCloud.getTotalPoints());
    wprintf(L"相机初始距离: %.1f\n", m_camera.getDistance());
    
    logToFile(L"点云查看器创建完成，原始点数: " + std::to_wstring(m_pointCloud.getTotalPoints()));
    logToFile(L"相机初始距离: " + std::to_wstring(m_camera.getDistance()));
}

PointCloudViewer::~PointCloudViewer() {
    cleanupGPUResources();
}

bool PointCloudViewer::initGPURendering() {
    if (!initGLExtensions()) {
        wprintf(L"警告: GPU优化渲染不可用，将使用传统渲染模式\n");
        logToFile(L"警告: GPU优化渲染不可用，将使用传统渲染模式");
        return false;
    }
    
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    wprintf(L"GPU优化渲染初始化成功\n");
    logToFile(L"GPU优化渲染初始化成功");
    return true;
}

void PointCloudViewer::updateGPUBuffer() {
    if (!m_gpuOptimized) return;
    
    std::vector<float> gpuData;
    auto visiblePoints = m_pointCloud.getVisiblePoints();
    gpuData.reserve(visiblePoints.size() * 6);
    
    for (const auto& point : visiblePoints) {
        gpuData.push_back(point.x);
        gpuData.push_back(point.y);
        gpuData.push_back(point.z);
        gpuData.push_back(point.r);
        gpuData.push_back(point.g);
        gpuData.push_back(point.b);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<ptrdiff_t>(gpuData.size() * sizeof(float)), gpuData.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointCloudViewer::renderGPUOptimized() {
    if (!m_gpuOptimized) {
        renderTraditional();
        return;
    }
    
    updateGPUBuffer();
    
    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, m_pointCloud.getVisiblePoints().size());
    glBindVertexArray(0);
}

void PointCloudViewer::renderTraditional() {
    glBegin(GL_POINTS);
    for (const auto& point : m_pointCloud.getVisiblePoints()) {
        glColor3f(point.r, point.g, point.b);
        glVertex3f(point.x, point.y, point.z);
    }
    glEnd();
}

void PointCloudViewer::cleanupGPUResources() {
    if (m_gpuOptimized) {
        if (glDeleteBuffers) {
            glDeleteBuffers(1, &m_vbo);
        }
        if (glDeleteVertexArrays) {
            glDeleteVertexArrays(1, &m_vao);
        }
        m_gpuOptimized = false;
    }
}

bool PointCloudViewer::initialize() {
    wprintf(L"Starting OpenGL initialization...\n");
    logToFile(L"Starting OpenGL initialization...");
    
    m_hdc = GetDC(m_hwnd);
    if (!m_hdc) {
        wprintf(L"Failed to get device context\n");
        return false;
    }
    
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    
    int pixelFormat = ChoosePixelFormat(m_hdc, &pfd);
    if (!pixelFormat) {
        wprintf(L"Failed to choose pixel format\n");
        ReleaseDC(m_hwnd, m_hdc);
        return false;
    }
    
    if (!SetPixelFormat(m_hdc, pixelFormat, &pfd)) {
        wprintf(L"Failed to set pixel format\n");
        ReleaseDC(m_hwnd, m_hdc);
        return false;
    }
    
    m_hglrc = wglCreateContext(m_hdc);
    if (!m_hglrc) {
        wprintf(L"Failed to create OpenGL context\n");
        ReleaseDC(m_hwnd, m_hdc);
        return false;
    }
    
    if (!wglMakeCurrent(m_hdc, m_hglrc)) {
        wprintf(L"Failed to set OpenGL context\n");
        wglDeleteContext(m_hglrc);
        ReleaseDC(m_hwnd, m_hdc);
        return false;
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(0.5f);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    
    RECT rect;
    if (GetClientRect(m_hwnd, &rect)) {
        m_windowWidth = rect.right - rect.left;
        m_windowHeight = rect.bottom - rect.top;
    }
    
    updateViewport(m_windowWidth, m_windowHeight);
    
    if (initGPURendering()) {
        m_gpuOptimized = true;
        wprintf(L"使用GPU优化渲染模式\n");
        logToFile(L"使用GPU优化渲染模式");
    } else {
        wprintf(L"使用传统渲染模式\n");
        logToFile(L"使用传统渲染模式");
    }
    
    wprintf(L"OpenGL initialization complete!\n");
    
    return true;
}

void PointCloudViewer::cleanup() {
    cleanupGPUResources();
    if (m_hglrc) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_hglrc);
        m_hglrc = nullptr;
    }
    
    if (m_hdc) {
        ReleaseDC(m_hwnd, m_hdc);
        m_hdc = nullptr;
    }
}

void PointCloudViewer::updateViewport(int width, int height) {
    if (width > 0 && height > 0) {
        glViewport(0, 0, width, height);
        m_windowWidth = width;
        m_windowHeight = height;
    }
}

void PointCloudViewer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    float currentDistance = m_camera.getDistance();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = (m_windowWidth > 0) ? static_cast<float>(m_windowWidth) / m_windowHeight : 1.33f;
    gluPerspective(60.0, aspectRatio, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    float camPos[3], camTarget[3], camUp[3];
    m_camera.getPosition(camPos);
    m_camera.getTarget(camTarget);
    m_camera.getUp(camUp);
    
    gluLookAt(
        camPos[0], camPos[1], camPos[2],
        camTarget[0], camTarget[1], camTarget[2],
        camUp[0], camUp[1], camUp[2]
    );
    
    float pointSize = 1.5f;
    if (currentDistance > 50.0f) {
        pointSize = 1.0f;
    } else if (currentDistance > 20.0f) {
        pointSize = 1.2f;
    } else {
        pointSize = 1.5f;
    }
    glPointSize(pointSize);
    
    m_pointCloud.optimizePointCloud(camPos, camTarget, camUp, currentDistance);
    
    if (m_gpuOptimized) {
        renderGPUOptimized();
    } else {
        renderTraditional();
    }
    
    SwapBuffers(m_hdc);
}

void PointCloudViewer::onMouseDown(int x, int y) {
    m_mouseDown = true;
    m_lastMouseX = x;
    m_lastMouseY = y;
}

void PointCloudViewer::onMouseUp() {
    m_mouseDown = false;
}

void PointCloudViewer::onMouseMove(int x, int y) {
    if (m_mouseDown) {
        float dx = static_cast<float>(x - m_lastMouseX);
        float dy = static_cast<float>(y - m_lastMouseY);
        
        if (fabs(dx) > 0.1f || fabs(dy) > 0.1f) {
            wprintf(L"鼠标移动: 位置(%d,%d), 偏移(%f,%f)\n", x, y, dx, dy);
            m_camera.rotate(dx, dy);
            m_lastMouseX = x;
            m_lastMouseY = y;
        }
    }
}

void PointCloudViewer::onMouseWheel(int delta) {
    if (delta > 0x7FFF) {
        delta = delta - 0x10000;
    }
    m_camera.zoom(static_cast<float>(delta));
}

bool PointCloudViewer::loadJSModel(const std::string& filename) {
    return m_modelRenderer.loadJSModel(filename);
}

void PointCloudViewer::toggleModelWireframeMode() {
    m_modelRenderer.toggleModelWireframeMode();
}

void PointCloudViewer::adjustModelScale(float delta) {
    m_modelRenderer.adjustModelScale(delta);
}

void PointCloudViewer::adjustModelRotation(float deltaX, float deltaY) {
    m_modelRenderer.adjustModelRotation(deltaX, deltaY);
}

bool PointCloudViewer::executeJSCode(const std::string& code) {
    return m_modelRenderer.executeJSCode(code);
}

// 加载并执行 JavaScript 脚本文件
bool PointCloudViewer::loadJSScript(const std::string& filename) {
    return m_modelRenderer.loadJSScript(filename);
}

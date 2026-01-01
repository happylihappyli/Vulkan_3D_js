// 点云查看器主头文件
#pragma once

#include <vector>
#include <string>
#include <memory>

// 前向声明
struct GLFWwindow;

namespace PointCloudViewer {

// 点云数据结构
struct Point {
    float x, y, z;     // 位置
    float r, g, b;     // 颜色
    float intensity;   // 强度
};

// 点云数据类
class PointCloud {
public:
    PointCloud();
    ~PointCloud();
    
    // 从文件加载点云数据
    bool loadFromFile(const std::string& filename);
    
    // 从内存加载点云数据
    void loadFromMemory(const std::vector<Point>& points);
    
    // 获取点云数据
    const std::vector<Point>& getPoints() const { return m_points; }
    
    // 获取点云边界
    void getBounds(float& minX, float& minY, float& minZ, 
                   float& maxX, float& maxY, float& maxZ) const;
    
    // 获取点云中心
    void getCenter(float& centerX, float& centerY, float& centerZ) const;
    
private:
    std::vector<Point> m_points;
    float m_minX, m_minY, m_minZ;
    float m_maxX, m_maxY, m_maxZ;
    bool m_boundsCalculated;
    
    void calculateBounds();
};

// 相机类
class Camera {
public:
    Camera();
    
    // 设置相机位置和方向
    void setPosition(float x, float y, float z);
    void setTarget(float x, float y, float z);
    void setUpVector(float x, float y, float z);
    
    // 相机控制
    void rotate(float deltaX, float deltaY);
    void zoom(float delta);
    void pan(float deltaX, float deltaY);
    
    // 获取视图矩阵
    void getViewMatrix(float* matrix) const;
    
    // 获取投影矩阵
    void getProjectionMatrix(float* matrix, float aspectRatio) const;
    
private:
    float m_position[3];
    float m_target[3];
    float m_up[3];
    float m_distance;
    float m_yaw, m_pitch;
    float m_fov;
    float m_nearPlane, m_farPlane;
    
    void updateVectors();
};

// Vulkan渲染器类
class VulkanRenderer {
public:
    VulkanRenderer();
    ~VulkanRenderer();
    
    // 初始化Vulkan
    bool initialize(GLFWwindow* window);
    
    // 清理资源
    void cleanup();
    
    // 渲染点云
    void render(const PointCloud& pointCloud, const Camera& camera);
    
    // 重新创建交换链（窗口大小改变时）
    void recreateSwapChain();
    
private:
    // Vulkan对象
    struct VulkanObjects;
    std::unique_ptr<VulkanObjects> m_vulkan;
    
    // 初始化函数
    bool createInstance();
    bool createSurface(GLFWwindow* window);
    bool pickPhysicalDevice();
    bool createLogicalDevice();
    bool createSwapChain();
    bool createRenderPass();
    bool createGraphicsPipeline();
    bool createFramebuffers();
    bool createCommandPool();
    bool createVertexBuffer();
    bool createCommandBuffers();
    bool createSyncObjects();
    
    // 辅助函数
    uint32_t findMemoryType(uint32_t typeFilter, uint32_t properties);
};

// 主应用程序类
class Application {
public:
    Application();
    ~Application();
    
    // 运行应用程序
    int run();
    
private:
    GLFWwindow* m_window;
    std::unique_ptr<VulkanRenderer> m_renderer;
    std::unique_ptr<PointCloud> m_pointCloud;
    std::unique_ptr<Camera> m_camera;
    
    // 窗口尺寸
    int m_width, m_height;
    
    // 鼠标状态
    bool m_mousePressed;
    double m_lastMouseX, m_lastMouseY;
    
    // 初始化函数
    bool initializeWindow();
    bool initializeVulkan();
    bool loadPointCloud();
    
    // 回调函数
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    
    // 主循环
    void mainLoop();
    void cleanup();
};

} // namespace PointCloudViewer
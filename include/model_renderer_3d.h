#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <memory>

namespace ScriptViewer {

// 3D模型数据结构
struct SubMesh {
    std::string name;
    size_t indexStart;
    size_t indexCount;
    int materialIndex;
};

struct Material {
    std::string name;
    float ambient[3];
    float diffuse[3];
    float specular[3];
    
    Material() {
        ambient[0] = 0.2f; ambient[1] = 0.2f; ambient[2] = 0.2f;
        diffuse[0] = 0.8f; diffuse[1] = 0.8f; diffuse[2] = 0.8f;
        specular[0] = 0.0f; specular[1] = 0.0f; specular[2] = 0.0f;
    }
};

struct ModelData {
    std::vector<float> vertices;     // 顶点数据 (x, y, z)
    std::vector<float> normals;      // 法线数据
    std::vector<unsigned int> indices; // 所有子网格合并后的索引
    std::vector<SubMesh> subMeshes;    // 子网格信息
    std::vector<Material> materials;   // 材质列表
    
    void clear() {
        vertices.clear();
        normals.clear();
        indices.clear();
        subMeshes.clear();
        materials.clear();
    }
};

// 3D渲染器类，用于在ImGui窗口中渲染3D模型
class ModelRenderer3D {
public:
    ModelRenderer3D();
    ~ModelRenderer3D();
    
    // 初始化渲染器
    bool initialize();
    
    // 清理资源
    void cleanup();
    
    // 加载OBJ模型
    bool loadOBJModel(const std::string& filename);
    
    // 清空模型
    void clearModel();
    
    // 渲染模型到 FBO
    void render(int width, int height);
    
    // 获取渲染纹理ID
    GLuint getTextureID() const { return m_fboTexture; }
    
    // 调整 Framebuffer 大小
    void resizeFramebuffer(int width, int height);

    // 检查模型是否已加载
    bool isModelLoaded() const { return m_modelLoaded; }
    
    // 获取模型信息
    size_t getVertexCount() const { return m_modelData.vertices.size() / 3; }
    size_t getFaceCount() const { return m_modelData.indices.size() / 3; }
    size_t getMaterialCount() const { return m_modelData.materials.size(); }
    size_t getSubMeshCount() const { return m_modelData.subMeshes.size(); }
    
    // 获取/设置材质颜色
    Material* getMaterial(int index) {
        if (index >= 0 && index < m_modelData.materials.size()) {
            return &m_modelData.materials[index];
        }
        return nullptr;
    }
    
    // 设置渲染模式
    void setWireframeMode(bool wireframe) { m_wireframeMode = wireframe; }
    bool isWireframeMode() const { return m_wireframeMode; }
    
    // 模型变换
    void setRotation(float x, float y, float z) {
        m_rotationX = x;
        m_rotationY = y;
        m_rotationZ = z;
    }
    void setScale(float scale) { m_scale = scale; }
    void setPosition(float x, float y, float z) {
        m_positionX = x;
        m_positionY = y;
        m_positionZ = z;
    }
    
    // 获取模型变换参数
    float getRotationX() const { return m_rotationX; }
    float getRotationY() const { return m_rotationY; }
    float getRotationZ() const { return m_rotationZ; }
    float getScale() const { return m_scale; }
    
    // 获取模型数据
    const std::vector<float>& getVertices() const { return m_modelData.vertices; }
    const std::vector<unsigned int>& getIndices() const { return m_modelData.indices; }

    // 设置模型数据（从JavaScript桥接模块传递）
    void setModelData(const std::vector<float>& vertices, 
                      const std::vector<unsigned int>& indices) {
        m_modelData.vertices = vertices;
        m_modelData.indices = indices;
        
        // 创建默认材质和子网格
        m_modelData.materials.clear();
        m_modelData.subMeshes.clear();
        
        Material defaultMat;
        defaultMat.name = "Default";
        m_modelData.materials.push_back(defaultMat);
        
        SubMesh subMesh;
        subMesh.name = "Default";
        subMesh.indexStart = 0;
        subMesh.indexCount = indices.size();
        subMesh.materialIndex = 0;
        m_modelData.subMeshes.push_back(subMesh);
        
        m_modelLoaded = !vertices.empty() && !indices.empty();
        initGLResources();
    }
    
    // 摄像机控制
    void setCamera(float dist, float rotX, float rotY, float panX, float panY) {
        m_camDist = dist;
        m_camRotX = rotX;
        m_camRotY = rotY;
        m_camPanX = panX;
        m_camPanY = panY;
    }
    
    // 移动摄像机
    void moveCamera(float dDist, float dRotX, float dRotY, float dPanX, float dPanY) {
        m_camDist += dDist;
        if (m_camDist < 0.1f) m_camDist = 0.1f;
        m_camRotX += dRotX;
        m_camRotY += dRotY;
        m_camPanX += dPanX;
        m_camPanY += dPanY;
    }
    
    float getCameraDist() const { return m_camDist; }
    float getCameraRotX() const { return m_camRotX; }
    float getCameraRotY() const { return m_camRotY; }
    float getCameraPanX() const { return m_camPanX; }
    float getCameraPanY() const { return m_camPanY; }

private:
    // OpenGL资源
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    
    // Framebuffer 资源
    GLuint m_fbo;
    GLuint m_fboTexture;
    GLuint m_rbo;
    int m_fboWidth;
    int m_fboHeight;
    
    // 模型数据
    ModelData m_modelData;
    bool m_modelLoaded;
    
    // 渲染参数
    bool m_wireframeMode;
    float m_rotationX;
    float m_rotationY;
    float m_rotationZ;
    float m_scale;
    float m_positionX;
    float m_positionY;
    float m_positionZ;
    
    // 摄像机参数
    float m_camDist;
    float m_camRotX;
    float m_camRotY;
    float m_camPanX;
    float m_camPanY;
    
    // 初始化OpenGL资源
    bool initGLResources();
    
    // 清理OpenGL资源
    void cleanupGLResources();
    
    // 解析OBJ文件
    bool parseOBJFile(const std::string& filename);
    
    // 设置投影和视图矩阵
    void setupMatrices(int width, int height);
};

} // namespace ScriptViewer

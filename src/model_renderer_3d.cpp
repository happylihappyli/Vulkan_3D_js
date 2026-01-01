#include "model_renderer_3d.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace ScriptViewer {

ModelRenderer3D::ModelRenderer3D()
    : m_vao(0)
    , m_vbo(0)
    , m_ebo(0)
    , m_fbo(0)
    , m_fboTexture(0)
    , m_rbo(0)
    , m_fboWidth(0)
    , m_fboHeight(0)
    , m_modelLoaded(false)
    , m_wireframeMode(false)
    , m_rotationX(0.0f)
    , m_rotationY(0.0f)
    , m_rotationZ(0.0f)
    , m_scale(1.0f)
    , m_positionX(0.0f)
    , m_positionY(0.0f)
    , m_positionZ(0.0f)
    , m_camDist(5.0f)
    , m_camRotX(20.0f)
    , m_camRotY(45.0f)
    , m_camPanX(0.0f)
    , m_camPanY(0.0f)
{
}

ModelRenderer3D::~ModelRenderer3D() {
    cleanup();
}

// 初始化渲染器
bool ModelRenderer3D::initialize() {
    printf("[调试] 初始化3D渲染器...\n");
    
    // 初始化随机数种子
    srand((unsigned int)time(NULL));
    
    // 生成OpenGL对象
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    
    printf("[调试] 3D渲染器初始化成功\n");
    return true;
}

// 清理资源
void ModelRenderer3D::cleanup() {
    cleanupGLResources();
    m_modelData.clear();
    m_modelLoaded = false;
}

// 清理OpenGL资源
void ModelRenderer3D::cleanupGLResources() {
    if (m_fbo) {
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }
    if (m_fboTexture) {
        glDeleteTextures(1, &m_fboTexture);
        m_fboTexture = 0;
    }
    if (m_rbo) {
        glDeleteRenderbuffers(1, &m_rbo);
        m_rbo = 0;
    }
    if (m_ebo) {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }
    if (m_vbo) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
}

// 调整 Framebuffer 大小
void ModelRenderer3D::resizeFramebuffer(int width, int height) {
    if (width <= 0 || height <= 0) return;
    if (width == m_fboWidth && height == m_fboHeight && m_fbo != 0) return;
    
    m_fboWidth = width;
    m_fboHeight = height;
    
    // 如果已存在，先删除
    if (m_fbo) {
        glDeleteFramebuffers(1, &m_fbo);
        glDeleteTextures(1, &m_fboTexture);
        glDeleteRenderbuffers(1, &m_rbo);
    }
    
    // 创建 Framebuffer
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    
    // 创建纹理附件
    glGenTextures(1, &m_fboTexture);
    glBindTexture(GL_TEXTURE_2D, m_fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTexture, 0);
    
    // 创建渲染缓冲附件（用于深度和模板测试）
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    
    // 检查 Framebuffer 完整性
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("[错误] Framebuffer 不完整！\n");
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    printf("[调试] Framebuffer 大小调整为: %dx%d\n", width, height);
}

// 初始化OpenGL资源
bool ModelRenderer3D::initGLResources() {
    if (m_modelData.vertices.empty() || m_modelData.indices.empty()) {
        printf("[错误] 模型数据为空，无法初始化OpenGL资源\n");
        return false;
    }
    
    printf("[调试] 初始化OpenGL资源: 顶点数=%zu, 面数=%zu\n", 
           m_modelData.vertices.size() / 3, 
           m_modelData.indices.size() / 3);
    
    // 绑定VAO
    glBindVertexArray(m_vao);
    
    // 设置顶点缓冲
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 
                 m_modelData.vertices.size() * sizeof(float), 
                 m_modelData.vertices.data(), 
                 GL_STATIC_DRAW);
    
    // 设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // 设置索引缓冲
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 m_modelData.indices.size() * sizeof(unsigned int), 
                 m_modelData.indices.data(), 
                 GL_STATIC_DRAW);
    
    // 解绑VAO
    glBindVertexArray(0);
    
    printf("[调试] OpenGL资源初始化完成\n");
    return true;
}

// 加载OBJ模型
bool ModelRenderer3D::loadOBJModel(const std::string& filename) {
    printf("[调试] 加载OBJ模型: %s\n", filename.c_str());
    
    // 清空之前的模型数据
    m_modelData.clear();
    m_modelLoaded = false;
    
    // 解析OBJ文件
    if (!parseOBJFile(filename)) {
        printf("[错误] OBJ模型加载失败\n");
        return false;
    }
    
    printf("[调试] 模型加载成功: 顶点数=%zu, 面数=%zu\n", 
           m_modelData.vertices.size() / 3, 
           m_modelData.indices.size() / 3);
    
    // 初始化OpenGL资源
    if (!initGLResources()) {
        printf("[错误] OpenGL资源初始化失败\n");
        return false;
    }
    
    m_modelLoaded = true;
    return true;
}

// 解析OBJ文件
bool ModelRenderer3D::parseOBJFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        printf("[错误] 无法打开文件: %s\n", filename.c_str());
        return false;
    }
    
    // 清空旧数据
    m_modelData.clear();
    
    // 添加默认材质
    Material defaultMat;
    defaultMat.name = "Default";
    m_modelData.materials.push_back(defaultMat);
    
    // 初始化当前子网格
    SubMesh currentSubMesh;
    currentSubMesh.name = "Default";
    currentSubMesh.indexStart = 0;
    currentSubMesh.indexCount = 0;
    currentSubMesh.materialIndex = 0;
    
    std::vector<float> tempVertices;
    std::vector<float> tempNormals;
    // std::vector<unsigned int> vertexIndices; // 不再需要单独存储，直接存入 m_modelData.indices
    
    std::string line;
    while (std::getline(file, line)) {
        // 跳过空行和注释
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        
        // 调试用：只打印非顶点/面的行，或者特定的行
        if (type == "usemtl" || type == "g" || type == "o" || line.find("usemtl") != std::string::npos) {
             printf("[DEBUG] Line: '%s', Type: '%s'\n", line.c_str(), type.c_str());
        }
        
        if (type == "usemtl") {
            std::string matName;
            iss >> matName;
            
            printf("[调试] 解析OBJ材质 usemtl: %s\n", matName.c_str());
            
            // 如果当前子网格有数据，先保存它
            if (currentSubMesh.indexCount > 0) {
                m_modelData.subMeshes.push_back(currentSubMesh);
                
                // 开始新的子网格
                currentSubMesh.indexStart = m_modelData.indices.size();
                currentSubMesh.indexCount = 0;
            }
            
            currentSubMesh.name = matName;
            
            // 查找材质是否已存在
            bool found = false;
            for (size_t i = 0; i < m_modelData.materials.size(); ++i) {
                if (m_modelData.materials[i].name == matName) {
                    currentSubMesh.materialIndex = (int)i;
                    found = true;
                    break;
                }
            }
            
            // 如果不存在，创建新材质
            if (!found) {
                Material newMat;
                newMat.name = matName;
                // 默认颜色：给每个材质一个不同的随机/哈希颜色，方便区分
                int seed = m_modelData.materials.size() + 1;
                // 使用 golden ratio conjugate 确保颜色分布均匀
                float h = fmod(seed * 0.618033988749895f, 1.0f);
                
                // HSV to RGB conversion (S=0.8, V=0.9)
                float s = 0.8f;
                float v = 0.9f;
                float c = v * s;
                float x = c * (1 - fabs(fmod(h * 6, 2) - 1));
                float m = v - c;
                
                float r, g, b;
                if (h < 1.0f/6.0f)      { r = c; g = x; b = 0; }
                else if (h < 2.0f/6.0f) { r = x; g = c; b = 0; }
                else if (h < 3.0f/6.0f) { r = 0; g = c; b = x; }
                else if (h < 4.0f/6.0f) { r = 0; g = x; b = c; }
                else if (h < 5.0f/6.0f) { r = x; g = 0; b = c; }
                else                    { r = c; g = 0; b = x; }
                
                newMat.diffuse[0] = r + m;
                newMat.diffuse[1] = g + m;
                newMat.diffuse[2] = b + m;
                
                m_modelData.materials.push_back(newMat);
                currentSubMesh.materialIndex = (int)(m_modelData.materials.size() - 1);
            }
            
        } else if (type == "v") {
            // 顶点数据
            float x, y, z;
            iss >> x >> y >> z;
            tempVertices.push_back(x);
            tempVertices.push_back(y);
            tempVertices.push_back(z);
        } else if (type == "vn") {
            // 法线数据
            float nx, ny, nz;
            iss >> nx >> ny >> nz;
            tempNormals.push_back(nx);
            tempNormals.push_back(ny);
            tempNormals.push_back(nz);
        } else if (type == "f") {
            // 面数据
            std::string faceData;
            while (iss >> faceData) {
                // 解析顶点索引 (格式: v/vt/vn 或 v//vn 或 v)
                size_t pos1 = faceData.find('/');
                
                unsigned int vertexIndex = 0;
                if (pos1 == std::string::npos) {
                    // 格式: v
                    vertexIndex = std::stoul(faceData);
                } else {
                    // 格式: v/vt/vn 或 v//vn
                    vertexIndex = std::stoul(faceData.substr(0, pos1));
                }
                
                // OBJ索引从1开始，转换为从0开始
                vertexIndex--;
                
                // 添加到全局索引列表
                m_modelData.indices.push_back(vertexIndex);
                
                // 更新当前子网格索引计数
                currentSubMesh.indexCount++;
            }
        }
    }
    
    // 保存最后一个子网格
    if (currentSubMesh.indexCount > 0) {
        m_modelData.subMeshes.push_back(currentSubMesh);
    }
    
    file.close();
    
    // 如果没有任何子网格（例如没有面），或者只有默认的但没数据
    if (m_modelData.indices.empty()) {
        printf("[错误] 没有找到面数据\n");
        return false;
    }
    
    // 构建最终的顶点数据
    // 注意：现在的实现是直接用 tempVertices 作为 m_modelData.vertices
    // 但索引是基于 tempVertices 的。
    // 我们之前的实现是重新构建了 vertices 吗？
    // 之前的实现逻辑：
    // for i in vertexIndices:
    //    push tempVertices[vIndex]
    //    indices[i] = i
    // 这实际上是展开了所有顶点（非索引绘制模式，或者说唯一索引模式）。
    // 这种方式虽然浪费内存，但处理法线和纹理坐标（尤其是当它们索引不一致时）比较方便。
    // 但现在的 parseOBJFile 逻辑，我是直接 push index。
    // 如果我直接用 tempVertices 作为 vertices，那么索引就是直接指向 tempVertices。
    // 这要求 OBJ 里的 f v1 v2 v3 是共享顶点的。
    // 标准 OBJ 是共享的。
    // 但是，如果 v 和 vn 索引不一致（例如硬边），OpenGL 的 glDrawElements 只能接受一个索引。
    // 所以通常需要重新构建顶点缓冲：每个 (v, vt, vn) 组合变成一个唯一的顶点。
    // 鉴于目前没有法线和纹理支持，且之前的代码是展开顶点的，我这里如果直接用索引，会有问题吗？
    // 之前的代码：
    // for (size_t i = 0; i < vertexIndices.size(); i++) ... vertices.push_back(...)
    // indices[i] = i;
    // 这样做的结果是 vertices 数量等于 indices 数量。每个面都有独立的顶点。
    // 这样虽然简单，但完全没有利用索引缓冲的优势（除了作为 glDrawElements 的参数）。
    // 为了保持兼容性并快速实现，我应该沿用这种“展开”策略吗？
    // 如果我沿用展开策略，那么 SubMesh 的 indexStart 和 indexCount 仍然有效。
    // 只是 vertices 数组会很大。
    
    // 让我们修改逻辑以匹配旧的展开策略，因为这样最安全。
    // 重新构建 vertices 数组。
    
    std::vector<float> finalVertices;
    std::vector<unsigned int> finalIndices;
    
    // 这里的 m_modelData.indices 目前存储的是原始 OBJ 的顶点索引
    // 我们需要把它转换成展开后的索引 (0, 1, 2, ...)
    // 并构建展开后的 vertices
    
    for (size_t i = 0; i < m_modelData.indices.size(); ++i) {
        unsigned int vIndex = m_modelData.indices[i];
        if (vIndex * 3 + 2 < tempVertices.size()) {
            finalVertices.push_back(tempVertices[vIndex * 3]);
            finalVertices.push_back(tempVertices[vIndex * 3 + 1]);
            finalVertices.push_back(tempVertices[vIndex * 3 + 2]);
        } else {
             // 错误处理：索引越界
             finalVertices.push_back(0); finalVertices.push_back(0); finalVertices.push_back(0);
        }
        finalIndices.push_back((unsigned int)i);
    }
    
    m_modelData.vertices = finalVertices;
    m_modelData.indices = finalIndices;
    
    return true;
}

// 设置投影和视图矩阵
void ModelRenderer3D::setupMatrices(int width, int height) {
    // printf("[调试] 设置投影和视图矩阵: 宽度=%d, 高度=%d\n", width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // 使用透视投影
    float aspectRatio = (width > 0) ? static_cast<float>(width) / height : 1.0f;
    
    // 手动实现 gluPerspective: fovy, aspect, zNear, zFar
    float fovy = 45.0f;
    float zNear = 0.1f;
    float zFar = 1000.0f;
    float fH = tan(fovy / 360.0f * 3.14159f) * zNear;
    float fW = fH * aspectRatio;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // 摄像机变换
    // 1. 移动到距离位置 (Zoom)
    glTranslatef(0.0f, 0.0f, -m_camDist);
    
    // 2. 旋转 (Orbit)
    glRotatef(m_camRotX, 1.0f, 0.0f, 0.0f);
    glRotatef(m_camRotY, 0.0f, 1.0f, 0.0f);
    
    // 3. 平移 (Pan)
    glTranslatef(m_camPanX, m_camPanY, 0.0f);
    
    // printf("[调试] 摄像机: Dist=%.1f, Rot=(%.1f, %.1f), Pan=(%.1f, %.1f)\n", 
    //        m_camDist, m_camRotX, m_camRotY, m_camPanX, m_camPanY);
}

// 渲染模型到 FBO
void ModelRenderer3D::render(int width, int height) {
    // 确保 Framebuffer 大小正确
    resizeFramebuffer(width, height);
    if (!m_fbo) return;

    // printf("[调试] render()函数被调用: m_modelLoaded=%d, SubMeshes=%zu\n", m_modelLoaded, m_modelData.subMeshes.size());
    
    // 绑定 Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, width, height);
    
    // 清除 Framebuffer 内容
    // 恢复深灰色背景，因为我们现在有全屏网格和坐标轴，不再需要紫色背景来调试FBO可见性
    glClearColor(0.15f, 0.15f, 0.17f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // 确保切换回固定管线
    glUseProgram(0);
    
    // 保存当前OpenGL状态
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
    
    // 禁用可能干扰的状态
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE); // 禁用面剔除
    glDisable(GL_SCISSOR_TEST); // 禁用剪裁测试
    
    // 禁用光照（因为测试模型可能没有法线，使用纯色填充）
    glDisable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    // glEnable(GL_COLOR_MATERIAL);
    // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // 设置简单的光照
    /*
    GLfloat lightPos[] = { 10.0f, 10.0f, 10.0f, 1.0f };
    GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    */
    
    // 检查 FBO 状态
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("[错误] 渲染时 FBO 不完整: 0x%x\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }
    
    // 设置投影和视图矩阵
    setupMatrices(width, height);
    
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    
    // 绘制一个明显的调试立方体（带颜色）
    // 为了不被透视投影的近裁剪面裁掉，我们把立方体画在原点，依赖 setupMatrices 的摄像机变换来观看
    glPushMatrix();
    // 注意：这里不需要 glLoadIdentity()，因为我们需要保留 setupMatrices 设置的视图矩阵（摄像机位置）
    // 只需要平移到世界坐标原点即可（其实已经是原点了，除非 setupMatrices 之后又动了）
    // 这里的 glLoadIdentity 和 translate 之前是为了配合固定 Ortho 相机，现在不需要了
    
    // 绘制彩色坐标轴 (World Origin)
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(10.0f, 0.0f, 0.0f); // X
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f); // Y
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 10.0f); // Z
    glEnd();
    
    // 绘制网格地面
    glLineWidth(1.0f);
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINES);
    for(int i=-10; i<=10; ++i) {
        glVertex3f((float)i, 0.0f, -10.0f);
        glVertex3f((float)i, 0.0f, 10.0f);
        glVertex3f(-10.0f, 0.0f, (float)i);
        glVertex3f(10.0f, 0.0f, (float)i);
    }
    glEnd();

    glPopMatrix();
    
    if (m_modelLoaded) {
        // 设置渲染模式
        if (m_wireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        
        // 应用模型变换
        glPushMatrix();
        
        // 调整模型位置，使其在相机前方
        glTranslatef(m_positionX, m_positionY, m_positionZ);
        
        // 应用旋转
        glRotatef(m_rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(m_rotationY, 0.0f, 1.0f, 0.0f);
        glRotatef(m_rotationZ, 0.0f, 0.0f, 1.0f);
        
        // 应用缩放
        glScalef(m_scale, m_scale, m_scale);
        
        // printf("[调试] 模型变换: 位置=(%.2f,%.2f,%.2f), 旋转=(%.1f,%.1f,%.1f), 缩放=%.2f\n",
        //        m_positionX, m_positionY, m_positionZ,
        //        m_rotationX, m_rotationY, m_rotationZ,
        //        m_scale);
        
        // 绑定VAO并绘制
        glBindVertexArray(m_vao);
        
        if (m_vao != 0) {
            // 如果没有子网格信息（兼容旧数据），则绘制所有
            if (m_modelData.subMeshes.empty()) {
                glColor3f(1.0f, 1.0f, 1.0f);
                glDrawElements(GL_TRIANGLES, 
                               static_cast<GLsizei>(m_modelData.indices.size()), 
                               GL_UNSIGNED_INT, 
                               0);
            } else {
                // 遍历所有子网格进行绘制
                int subMeshIdx = 0;
                for (const auto& subMesh : m_modelData.subMeshes) {
                    // 设置材质颜色
                    if (subMesh.materialIndex >= 0 && subMesh.materialIndex < m_modelData.materials.size()) {
                        const Material& mat = m_modelData.materials[subMesh.materialIndex];
                        glColor3fv(mat.diffuse);
                    } else {
                        // 默认材质
                        glColor3f(0.8f, 0.8f, 0.8f);
                    }
                    
                    // 绘制子网格
                    // 由于我们已经展开了顶点 (Unrolled)，indices 是连续的 0,1,2...
                    // 所以可以直接用 glDrawArrays，这样更稳健
                    glDrawArrays(GL_TRIANGLES, 
                                 (GLint)subMesh.indexStart, 
                                 (GLsizei)subMesh.indexCount);
                                 
                    subMeshIdx++;
                }
            }
        }
        
        glBindVertexArray(0);
        glPopMatrix();
    }
    
    // 恢复OpenGL状态
    glPopAttrib();
    
    // 解绑 Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace ScriptViewer

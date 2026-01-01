#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
测试QuickJS模型加载器的使用方法
展示如何在C++中使用ModelLoaderJS类加载和渲染OBJ模型
"""

import os

def create_quickjs_usage_guide():
    """创建QuickJS模型加载器使用指南"""
    
    guide_content = """# QuickJS模型加载器使用指南

## 概述
ModelLoaderJS是一个C++类，用于加载OBJ格式的3D模型文件。虽然名称中包含"JS"，但实际上它是一个纯C++实现的OBJ文件解析器。

## 类定义
```cpp
class ModelLoaderJS {
private:
    std::vector<float> m_vertices;     // 顶点数据
    std::vector<float> m_normals;      // 法线数据
    std::vector<float> m_texCoords;    // 纹理坐标
    std::vector<unsigned int> m_indices; // 面索引
    
public:
    ModelLoaderJS();
    
    // 加载OBJ模型文件
    bool loadOBJModel(const std::string& filename);
    
    // 获取顶点数据
    std::vector<float> getVertices() const;
    
    // 获取法线数据
    std::vector<float> getNormals() const;
    
    // 获取纹理坐标
    std::vector<float> getTexCoords() const;
    
    // 获取面索引
    std::vector<unsigned int> getIndices() const;
    
    // 获取顶点数量
    size_t getVertexCount() const;
    
    // 获取面数量
    size_t getFaceCount() const;
    
    // 清空模型数据
    void clear();
};
```

## 基本使用方法

### 1. 包含头文件
```cpp
#include "quickjs_wrapper.h"
```

### 2. 创建加载器实例
```cpp
ModelLoaderJS loader;
```

### 3. 加载OBJ模型
```cpp
bool success = loader.loadOBJModel("path/to/model.obj");
if (!success) {
    // 处理加载失败
    return;
}
```

### 4. 获取模型数据
```cpp
// 获取顶点数据（每个顶点3个浮点数：x, y, z）
std::vector<float> vertices = loader.getVertices();

// 获取法线数据（每个法线3个浮点数：nx, ny, nz）
std::vector<float> normals = loader.getNormals();

// 获取纹理坐标（每个纹理坐标2个浮点数：u, v）
std::vector<float> texCoords = loader.getTexCoords();

// 获取面索引（每个面3个索引，组成三角形）
std::vector<unsigned int> indices = loader.getIndices();
```

### 5. 获取统计信息
```cpp
size_t vertexCount = loader.getVertexCount();
size_t faceCount = loader.getFaceCount();

std::cout << "顶点数量: " << vertexCount << std::endl;
std::cout << "面数量: " << faceCount << std::endl;
```

## 完整示例：加载并渲染OBJ模型

```cpp
#include "quickjs_wrapper.h"
#include <GL/gl.h>
#include <GL/glu.h>

void loadAndRenderModel(const std::string& filename) {
    // 创建模型加载器
    ModelLoaderJS loader;
    
    // 加载OBJ模型
    if (!loader.loadOBJModel(filename)) {
        std::cerr << "无法加载模型: " << filename << std::endl;
        return;
    }
    
    // 获取模型数据
    std::vector<float> vertices = loader.getVertices();
    std::vector<float> normals = loader.getNormals();
    std::vector<unsigned int> indices = loader.getIndices();
    
    // 创建OpenGL缓冲区
    GLuint vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    // 上传顶点数据到GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), 
                 vertices.data(), GL_STATIC_DRAW);
    
    // 上传索引数据到GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 indices.data(), GL_STATIC_DRAW);
    
    // 渲染模型
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    
    // 清理资源
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
```

## OBJ文件格式支持

ModelLoaderJS支持以下OBJ文件元素：

- **顶点坐标 (v)**: `v x y z [w]`
- **法线向量 (vn)**: `vn i j k`
- **纹理坐标 (vt)**: `vt u v [w]`
- **面定义 (f)**: `f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3`

### 示例OBJ文件
```
# 立方体模型
v -1.0 -1.0  1.0
v  1.0 -1.0  1.0
v  1.0  1.0  1.0
v -1.0  1.0  1.0

vn  0.0  0.0  1.0

vt 0.0 0.0
vt 1.0 0.0
vt 1.0 1.0
vt 0.0 1.0

f 1/1/1 2/2/1 3/3/1 4/4/1
```

## 在OpenGL点云查看器中使用

在当前的OpenGL点云查看器项目中，ModelRenderer类已经集成了ModelLoaderJS：

```cpp
// 在ModelRenderer类中
class ModelRenderer {
private:
    std::unique_ptr<ModelLoaderJS> m_modelLoader;
    // ... 其他成员变量
    
public:
    // 加载OBJ模型
    bool loadJSModel(const std::string& filename);
    
    // 渲染模型
    void renderModel();
};
```

### 使用方法

1. **按J键加载模型**:
   - 程序会尝试加载 `test/sample.obj` 文件

2. **按M键切换渲染模式**:
   - 在线框模式和实体模式之间切换

3. **使用方向键调整模型**:
   - 上/下方向键：放大/缩小模型
   - 左/右方向键：旋转模型

## 注意事项

1. **索引从1开始**: OBJ文件的索引从1开始，ModelLoaderJS会自动转换为从0开始的C++索引。

2. **面分解**: 四边形面会被自动分解为两个三角形。

3. **内存管理**: ModelLoaderJS使用std::vector管理内存，不需要手动释放。

4. **线程安全**: ModelLoaderJS不是线程安全的，不要在多线程环境中共享同一个实例。

## 扩展功能

如果需要添加更多功能，可以考虑：

1. **支持更多OBJ元素**: 材质文件(.mtl)、曲线、曲面等
2. **优化加载性能**: 使用多线程解析、内存映射文件等
3. **错误处理**: 更详细的错误信息和恢复机制
4. **模型验证**: 检查模型数据的有效性

## 总结

ModelLoaderJS提供了一个简单易用的C++接口来加载OBJ模型文件。通过这个类，你可以轻松地在OpenGL应用程序中加载和渲染3D模型。
"""
    
    guide_path = "test/quickjs_usage_guide.md"
    with open(guide_path, 'w', encoding='utf-8') as f:
        f.write(guide_content)
    
    print(f"使用指南已创建: {guide_path}")
    return guide_path

def main():
    print("=" * 60)
    print("QuickJS模型加载器使用方法测试")
    print("=" * 60)
    
    # 创建使用指南
    guide_path = create_quickjs_usage_guide()
    
    print("\n主要使用步骤:")
    print("-" * 60)
    print("1. 包含头文件: #include \"quickjs_wrapper.h\"")
    print("2. 创建实例: ModelLoaderJS loader;")
    print("3. 加载模型: loader.loadOBJModel(\"model.obj\");")
    print("4. 获取数据: vertices = loader.getVertices();")
    print("5. 渲染模型: 使用OpenGL渲染顶点数据")
    print("-" * 60)
    
    print("\n在当前项目中的使用:")
    print("-" * 60)
    print("- ModelRenderer类已集成ModelLoaderJS")
    print("- 按J键加载OBJ模型")
    print("- 按M键切换渲染模式")
    print("- 使用方向键调整模型")
    print("-" * 60)
    
    print("\n测试完成！")
    print("=" * 60)

if __name__ == "__main__":
    main()

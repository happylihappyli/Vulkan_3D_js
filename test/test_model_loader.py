#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
测试模型加载器功能
演示如何使用ModelLoaderJS类加载OBJ模型
"""

import os
import sys

# 测试OBJ文件内容
test_obj_content = """# 测试立方体模型
# 顶点坐标
v -1.0 -1.0  1.0
v  1.0 -1.0  1.0
v  1.0  1.0  1.0
v -1.0  1.0  1.0
v -1.0 -1.0 -1.0
v  1.0 -1.0 -1.0
v  1.0  1.0 -1.0
v -1.0  1.0 -1.0

# 法线向量
vn  0.0  0.0  1.0
vn  0.0  0.0 -1.0
vn  0.0  1.0  0.0
vn  0.0 -1.0  0.0
vn  1.0  0.0  0.0
vn -1.0  0.0  0.0

# 纹理坐标
vt 0.0 0.0
vt 1.0 0.0
vt 1.0 1.0
vt 0.0 1.0

# 面定义（使用顶点/纹理坐标/法线索引）
f 1/1/1 2/2/1 3/3/1 4/4/1
f 8/1/2 7/2/2 6/3/2 5/4/2
f 4/1/3 3/2/3 7/3/3 8/4/3
f 5/1/4 6/2/4 2/3/4 1/4/4
f 6/1/5 7/2/5 3/3/5 2/4/5
f 1/1/6 4/2/6 8/3/6 5/4/6
"""

def create_test_obj_file():
    """创建测试用的OBJ文件"""
    test_dir = "test"
    if not os.path.exists(test_dir):
        os.makedirs(test_dir)
    
    test_obj_path = os.path.join(test_dir, "test_cube.obj")
    
    with open(test_obj_path, 'w', encoding='utf-8') as f:
        f.write(test_obj_content)
    
    print(f"测试OBJ文件已创建: {test_obj_path}")
    return test_obj_path

def test_model_loader():
    """测试模型加载器功能"""
    print("=" * 60)
    print("测试模型加载器功能")
    print("=" * 60)
    
    # 创建测试OBJ文件
    obj_file = create_test_obj_file()
    
    print(f"\nOBJ文件内容预览:")
    print("-" * 40)
    with open(obj_file, 'r', encoding='utf-8') as f:
        lines = f.readlines()
        for i, line in enumerate(lines[:15], 1):
            print(f"{i:3d}: {line.rstrip()}")
    print("-" * 40)
    
    # 分析OBJ文件
    print("\nOBJ文件分析:")
    print("-" * 40)
    vertex_count = 0
    normal_count = 0
    texcoord_count = 0
    face_count = 0
    
    with open(obj_file, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if line.startswith('v '):
                vertex_count += 1
            elif line.startswith('vn '):
                normal_count += 1
            elif line.startswith('vt '):
                texcoord_count += 1
            elif line.startswith('f '):
                face_count += 1
    
    print(f"顶点数量: {vertex_count}")
    print(f"法线数量: {normal_count}")
    print(f"纹理坐标数量: {texcoord_count}")
    print(f"面数量: {face_count}")
    print("-" * 40)
    
    # 模拟C++ ModelLoaderJS的使用
    print("\n模拟C++ ModelLoaderJS的使用:")
    print("-" * 40)
    print("""
    // C++代码示例:
    ModelLoaderJS loader;
    
    // 加载OBJ模型
    bool success = loader.loadOBJModel("test/test_cube.obj");
    
    if (success) {
        // 获取模型数据
        std::vector<float> vertices = loader.getVertices();
        std::vector<float> normals = loader.getNormals();
        std::vector<float> texCoords = loader.getTexCoords();
        std::vector<unsigned int> indices = loader.getIndices();
        
        // 获取统计信息
        size_t vertexCount = loader.getVertexCount();
        size_t faceCount = loader.getFaceCount();
        
        // 使用数据进行渲染...
    }
    """)
    print("-" * 40)
    
    print("\n测试完成！")
    print("=" * 60)

if __name__ == "__main__":
    test_model_loader()

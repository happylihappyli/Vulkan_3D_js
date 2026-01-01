#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

class ModelLoaderJS {
private:
    std::vector<float> m_vertices;     // 顶点数据
    std::vector<float> m_normals;      // 法线数据
    std::vector<float> m_texCoords;    // 纹理坐标
    std::vector<unsigned int> m_indices; // 面索引
    
    bool parseOBJFile(const std::string& filename);
    void parseLine(const std::string& line, 
                   std::vector<float>& vertices, 
                   std::vector<float>& normals, 
                   std::vector<float>& texCoords,
                   std::vector<unsigned int>& indices);
    
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

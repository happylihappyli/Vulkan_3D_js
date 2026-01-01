#include "model_loader_js.h"
#include <iostream>
#include <sstream>
#include <algorithm>

ModelLoaderJS::ModelLoaderJS() {
    // 构造函数初始化
}

bool ModelLoaderJS::loadOBJModel(const std::string& filename) {
    // 清空之前的数据
    clear();
    
    // 解析OBJ文件
    return parseOBJFile(filename);
}

bool ModelLoaderJS::parseOBJFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开OBJ文件: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    std::vector<float> tempVertices;
    std::vector<float> tempNormals;
    std::vector<float> tempTexCoords;
    
    while (std::getline(file, line)) {
        // 跳过空行和注释
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // 解析每一行
        parseLine(line, tempVertices, tempNormals, tempTexCoords, m_indices);
    }
    
    file.close();
    
    // 将临时数据转换为最终格式
    m_vertices = tempVertices;
    m_normals = tempNormals;
    m_texCoords = tempTexCoords;
    
    std::cout << "OBJ文件加载成功: " << filename << std::endl;
    std::cout << "顶点数: " << getVertexCount() << std::endl;
    std::cout << "面数: " << getFaceCount() << std::endl;
    
    return true;
}

void ModelLoaderJS::parseLine(const std::string& line,
                               std::vector<float>& vertices,
                               std::vector<float>& normals,
                               std::vector<float>& texCoords,
                               std::vector<unsigned int>& indices) {
    std::istringstream iss(line);
    std::string type;
    iss >> type;
    
    if (type == "v") {
        // 顶点坐标
        float x, y, z;
        iss >> x >> y >> z;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    } else if (type == "vn") {
        // 法线向量
        float nx, ny, nz;
        iss >> nx >> ny >> nz;
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);
    } else if (type == "vt") {
        // 纹理坐标
        float u, v;
        iss >> u >> v;
        texCoords.push_back(u);
        texCoords.push_back(v);
    } else if (type == "f") {
        // 面定义
        std::string faceToken;
        while (iss >> faceToken) {
            // 解析顶点/纹理/法线索引
            std::replace(faceToken.begin(), faceToken.end(), '/', ' ');
            std::istringstream faceIss(faceToken);
            
            unsigned int vIndex = 0, vtIndex = 0, vnIndex = 0;
            faceIss >> vIndex;
            faceIss >> vtIndex;
            faceIss >> vnIndex;
            
            // OBJ文件索引从1开始，转换为从0开始
            if (vIndex > 0) {
                indices.push_back(vIndex - 1);
            }
        }
    }
}

std::vector<float> ModelLoaderJS::getVertices() const {
    return m_vertices;
}

std::vector<float> ModelLoaderJS::getNormals() const {
    return m_normals;
}

std::vector<float> ModelLoaderJS::getTexCoords() const {
    return m_texCoords;
}

std::vector<unsigned int> ModelLoaderJS::getIndices() const {
    return m_indices;
}

size_t ModelLoaderJS::getVertexCount() const {
    return m_vertices.size() / 3;
}

size_t ModelLoaderJS::getFaceCount() const {
    return m_indices.size() / 3;
}

void ModelLoaderJS::clear() {
    m_vertices.clear();
    m_normals.clear();
    m_texCoords.clear();
    m_indices.clear();
}

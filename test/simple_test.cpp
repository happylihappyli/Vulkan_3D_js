
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Point3D {
    float x, y, z;
    float r, g, b;
};

int main() {
    std::srand(std::time(nullptr));
    
    const int TOTAL_POINTS = 10000000;
    std::vector<Point3D> points;
    
    std::cout << "开始生成飞机点云，目标点数: " << TOTAL_POINTS << std::endl;
    
    // 简单的点云生成测试
    for (int i = 0; i < 1000; i++) {  // 只生成1000个点用于测试
        Point3D p;
        p.x = (std::rand() / (float)RAND_MAX - 0.5f) * 20.0f;
        p.y = (std::rand() / (float)RAND_MAX - 0.5f) * 8.0f;
        p.z = (std::rand() / (float)RAND_MAX - 0.5f) * 30.0f;
        p.r = 0.9f; p.g = 0.9f; p.b = 0.9f;
        points.push_back(p);
    }
    
    std::cout << "点云生成完成! 实际点数: " << points.size() << std::endl;
    std::cout << "飞机尺寸: 长度=20.0, 翼展=30.0, 高度=8.0" << std::endl;
    
    return 0;
}

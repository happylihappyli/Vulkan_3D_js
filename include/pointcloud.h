#pragma once

#include <vector>
#include <cstdint>

struct Point3D {
    float x, y, z;
    float r, g, b;
};

class PointCloud {
private:
    std::vector<Point3D> m_pointCloud;
    std::vector<Point3D> m_visiblePoints;
    
public:
    PointCloud();
    
    void generateAirplanePointCloud();
    void optimizePointCloud(float camPos[3], float camTarget[3], float camUp[3], float distance);
    
    const std::vector<Point3D>& getPointCloud() const;
    const std::vector<Point3D>& getVisiblePoints() const;
    size_t getTotalPoints() const;
};

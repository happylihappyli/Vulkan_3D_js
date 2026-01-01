#include "pointcloud.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include "utils.h"

PointCloud::PointCloud() {
    generateAirplanePointCloud();
}

void PointCloud::generateAirplanePointCloud() {
    const int TOTAL_POINTS = 100000;
    
    wprintf(L"开始生成最终版飞机形状点云，目标点数: %d\n", TOTAL_POINTS);
    logToFile(L"开始生成最终版飞机形状点云，目标点数: " + std::to_wstring(TOTAL_POINTS));
    
    const float FUSELAGE_LENGTH = 30.0f;
    const float FUSELAGE_RADIUS = 1.0f;
    const float WING_SPAN = 35.0f;
    const float WING_CHORD = 8.0f;
    const float WING_THICKNESS = 0.5f;
    const float TAIL_HEIGHT = 8.0f;
    const float TAIL_WIDTH = 6.0f;
    const float ENGINE_RADIUS = 0.6f;
    const float COCKPIT_RADIUS = 0.8f;
    
    int pointsGenerated = 0;
    
    const int FUSELAGE_POINTS = TOTAL_POINTS / 5;
    for (int i = 0; i < FUSELAGE_POINTS; i++) {
        Point3D p;
        
        float t = static_cast<float>(rand()) / RAND_MAX;
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
        
        float noseFactor = 1.0f - t * 0.5f;
        float middleFactor = 3.0f + sin(t * 3.14159f) * 0.3f;
        float tailFactor = 2.0f - (1.0f - t) * 0.4f;
        float maxRadius = FUSELAGE_RADIUS * noseFactor * middleFactor * tailFactor;
        
        float radius = static_cast<float>(rand()) / RAND_MAX * maxRadius;
        
        p.x = (t - 0.5f) * FUSELAGE_LENGTH;
        p.y = cos(theta) * radius;
        p.z = sin(theta) * radius;
        
        float depthFactor = radius / maxRadius;
        p.r = 0.9f - depthFactor * 0.2f;
        p.g = 0.1f + depthFactor * 0.1f;
        p.b = 0.1f + depthFactor * 0.1f;
        
        m_pointCloud.push_back(p);
        pointsGenerated++;
    }
    
    wprintf(L"机身生成完成: %d 个点\n", FUSELAGE_POINTS);
    logToFile(L"机身生成完成: " + std::to_wstring(FUSELAGE_POINTS) + L" 个点");
    
    const int WING_POINTS = TOTAL_POINTS / 3;
    for (int i = 0; i < WING_POINTS; i++) {
        Point3D p;
        
        bool isLeftWing = (rand() % 2) == 0;
        float wingSign = isLeftWing ? 1.0f : -1.0f;
        
        float u = static_cast<float>(rand()) / RAND_MAX;
        float v = static_cast<float>(rand()) / RAND_MAX;
        float w = static_cast<float>(rand()) / RAND_MAX;
        
        float maxThickness = WING_THICKNESS * (2.0f - v * 0.6f);
        
        p.x = (v - 0.2f) * WING_CHORD;
        p.y = (w - 0.5f) * maxThickness * 3.0f;
        p.z = wingSign * (u * WING_SPAN / 2.0f + 3.0f);
        
        float depthFactor = fabs(w - 0.5f) * 2.0f;
        p.r = 0.9f - depthFactor * 0.2f;
        p.g = 0.3f + depthFactor * 0.1f;
        p.b = 0.3f + depthFactor * 0.1f;
        
        m_pointCloud.push_back(p);
        pointsGenerated++;
    }
    
    wprintf(L"主翼生成完成: %d 个点\n", WING_POINTS);
    logToFile(L"主翼生成完成: " + std::to_wstring(WING_POINTS) + L" 个点");
    
    const int HORIZONTAL_TAIL_POINTS = TOTAL_POINTS / 6;
    for (int i = 0; i < HORIZONTAL_TAIL_POINTS; i++) {
        Point3D p;
        
        float u = static_cast<float>(rand()) / RAND_MAX;
        float v = static_cast<float>(rand()) / RAND_MAX;
        float w = static_cast<float>(rand()) / RAND_MAX;
        
        float tailThickness = 0.2f;
        float tailWidth = TAIL_WIDTH * 1.8f;
        
        p.x = FUSELAGE_LENGTH / 2.0f - 2.0f + u * 5.0f;
        p.y = 2.0f + (w - 0.5f) * tailThickness;
        p.z = (v - 0.5f) * tailWidth;
        
        float depthFactor = fabs(w - 0.5f) * 2.0f;
        p.r = 0.7f - depthFactor * 0.1f;
        p.g = 0.7f - depthFactor * 0.1f;
        p.b = 0.7f - depthFactor * 0.1f;
        
        m_pointCloud.push_back(p);
        pointsGenerated++;
    }
    
    wprintf(L"水平尾翼生成完成: %d 个点\n", HORIZONTAL_TAIL_POINTS);
    logToFile(L"水平尾翼生成完成: " + std::to_wstring(HORIZONTAL_TAIL_POINTS) + L" 个点");
    
    const int VERTICAL_TAIL_POINTS = TOTAL_POINTS / 6;
    for (int i = 0; i < VERTICAL_TAIL_POINTS; i++) {
        Point3D p;
        
        float u = static_cast<float>(rand()) / RAND_MAX;
        float v = static_cast<float>(rand()) / RAND_MAX;
        float w = static_cast<float>(rand()) / RAND_MAX;
        
        float tailThickness = 0.2f;
        float tailHeight = TAIL_HEIGHT * 1.8f;
        
        p.x = FUSELAGE_LENGTH / 2.0f - 2.0f + u * 4.0f;
        p.y = 2.5f + v * tailHeight;
        p.z = (w - 0.5f) * tailThickness;
        
        float depthFactor = fabs(w - 0.5f) * 2.0f;
        p.r = 0.7f - depthFactor * 0.1f;
        p.g = 0.7f - depthFactor * 0.1f;
        p.b = 0.7f - depthFactor * 0.1f;
        
        m_pointCloud.push_back(p);
        pointsGenerated++;
    }
    
    wprintf(L"垂直尾翼生成完成: %d 个点\n", VERTICAL_TAIL_POINTS);
    logToFile(L"垂直尾翼生成完成: " + std::to_wstring(VERTICAL_TAIL_POINTS) + L" 个点");
    
    const int ENGINE_POINTS = TOTAL_POINTS / 15;
    for (int i = 0; i < ENGINE_POINTS; i++) {
        Point3D p;
        
        bool isLeftEngine = (rand() % 2) == 0;
        float engineSign = isLeftEngine ? 1.0f : -1.0f;
        
        float t = static_cast<float>(rand()) / RAND_MAX;
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
        
        p.x = 5.0f + t * 3.0f;
        p.y = -3.0f + cos(theta) * ENGINE_RADIUS;
        p.z = engineSign * (WING_SPAN / 2.5f) + sin(theta) * ENGINE_RADIUS;
        
        p.r = 0.3f;
        p.g = 0.3f;
        p.b = 0.3f;
        
        m_pointCloud.push_back(p);
        pointsGenerated++;
    }
    
    wprintf(L"发动机生成完成: %d 个点\n", ENGINE_POINTS);
    logToFile(L"发动机生成完成: " + std::to_wstring(ENGINE_POINTS) + L" 个点");
    
    const int COCKPIT_POINTS = TOTAL_POINTS / 20;
    for (int i = 0; i < COCKPIT_POINTS; i++) {
        Point3D p;
        
        float t = static_cast<float>(rand()) / RAND_MAX;
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
        
        p.x = -FUSELAGE_LENGTH / 2.0f + 1.0f + t * 4.0f;
        p.y = 2.5f + cos(theta) * COCKPIT_RADIUS;
        p.z = sin(theta) * COCKPIT_RADIUS;
        
        p.r = 0.2f;
        p.g = 0.2f;
        p.b = 0.8f;
        
        m_pointCloud.push_back(p);
        pointsGenerated++;
    }
    
    wprintf(L"驾驶舱生成完成: %d 个点\n", COCKPIT_POINTS);
    logToFile(L"驾驶舱生成完成: " + std::to_wstring(COCKPIT_POINTS) + L" 个点");
    
    wprintf(L"清晰版飞机点云生成完成! 总点数: %zu\n", m_pointCloud.size());
    wprintf(L"飞机尺寸: 长度=%.1f, 翼展=%.1f, 高度=%.1f\n", 
           FUSELAGE_LENGTH, WING_SPAN, TAIL_HEIGHT);
    wprintf(L"包含部件: 机身、主翼、水平尾翼、垂直尾翼、发动机、驾驶舱\n");
    wprintf(L"颜色区分: 机身=白色, 机翼=浅灰, 尾翼=中灰, 发动机=深灰, 驾驶舱=蓝色\n");
    
    logToFile(L"清晰版飞机点云生成完成! 总点数: " + std::to_wstring(m_pointCloud.size()));
    logToFile(L"飞机尺寸: 长度=" + std::to_wstring(FUSELAGE_LENGTH) + 
              L", 翼展=" + std::to_wstring(WING_SPAN) + 
              L", 高度=" + std::to_wstring(TAIL_HEIGHT));
    logToFile(L"包含部件: 机身、主翼、水平尾翼、垂直尾翼、发动机、驾驶舱");
    logToFile(L"颜色区分: 机身=白色, 机翼=浅灰, 尾翼=中灰, 发动机=深灰, 驾驶舱=蓝色");
}

void PointCloud::optimizePointCloud(float camPos[3], float camTarget[3], float camUp[3], float distance) {
    m_visiblePoints.clear();
    
    float viewDir[3] = {
        camTarget[0] - camPos[0],
        camTarget[1] - camPos[1],
        camTarget[2] - camPos[2]
    };
    
    float length = sqrtf(viewDir[0]*viewDir[0] + viewDir[1]*viewDir[1] + viewDir[2]*viewDir[2]);
    if (length > 0.0f) {
        viewDir[0] /= length;
        viewDir[1] /= length;
        viewDir[2] /= length;
    }
    
    size_t pointsBefore = m_pointCloud.size();
    size_t pointsAfter = 0;
    
    const float NEAR_PLANE = 0.1f;
    const float FAR_PLANE = 100.0f;
    const float FOV_ANGLE = 60.0f;
    const float FOV_RADIANS = FOV_ANGLE * 3.14159f / 180.0f;
    const float TAN_FOV = tanf(FOV_RADIANS * 0.5f);
    
    for (const auto& point : m_pointCloud) {
        float toPoint[3] = {
            point.x - camPos[0],
            point.y - camPos[1],
            point.z - camPos[2]
        };
        
        float pointDistance = sqrtf(toPoint[0]*toPoint[0] + toPoint[1]*toPoint[1] + toPoint[2]*toPoint[2]);
        
        if (pointDistance < NEAR_PLANE || pointDistance > FAR_PLANE) {
            continue;
        }
        
        float dotProduct = toPoint[0]*viewDir[0] + toPoint[1]*viewDir[1] + toPoint[2]*viewDir[2];
        
        if (dotProduct > 0.0f) {
            float projectedLength = dotProduct / pointDistance;
            
            float right[3], up[3];
            
            right[0] = viewDir[1]*camUp[2] - viewDir[2]*camUp[1];
            right[1] = viewDir[2]*camUp[0] - viewDir[0]*camUp[2];
            right[2] = viewDir[0]*camUp[1] - viewDir[1]*camUp[0];
            
            float rightLength = sqrtf(right[0]*right[0] + right[1]*right[1] + right[2]*right[2]);
            if (rightLength > 0.0f) {
                right[0] /= rightLength;
                right[1] /= rightLength;
                right[2] /= rightLength;
            }
            
            up[0] = right[1]*viewDir[2] - right[2]*viewDir[1];
            up[1] = right[2]*viewDir[0] - right[0]*viewDir[2];
            up[2] = right[0]*viewDir[1] - right[1]*viewDir[0];
            
            float horizontalOffset = toPoint[0]*right[0] + toPoint[1]*right[1] + toPoint[2]*right[2];
            float verticalOffset = toPoint[0]*up[0] + toPoint[1]*up[1] + toPoint[2]*up[2];
            
            if (fabs(horizontalOffset) < pointDistance * TAN_FOV * 1.5f && 
                fabs(verticalOffset) < pointDistance * TAN_FOV * 1.5f) {
                m_visiblePoints.push_back(point);
                pointsAfter++;
            }
        }
    }
    
    if (pointsAfter < pointsBefore * 0.1f) {
        wprintf(L"警告：裁剪过多，显示所有点\n");
        m_visiblePoints = m_pointCloud;
        pointsAfter = pointsBefore;
    }
    
    wprintf(L"视锥体裁剪: 总点数 %zu -> 可见点数 %zu (裁剪率: %.1f%%)\n", 
            pointsBefore, pointsAfter, (1.0f - static_cast<float>(pointsAfter)/pointsBefore) * 100.0f);
}

const std::vector<Point3D>& PointCloud::getPointCloud() const {
    return m_pointCloud;
}

const std::vector<Point3D>& PointCloud::getVisiblePoints() const {
    return m_visiblePoints;
}

size_t PointCloud::getTotalPoints() const {
    return m_pointCloud.size();
}

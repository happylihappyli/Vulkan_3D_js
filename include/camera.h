#pragma once

#include <cmath>

class Camera {
private:
    float m_position[3];
    float m_target[3];
    float m_up[3];
    float m_yaw;
    float m_pitch;
    float m_distance;
    
    void updatePosition();
    
public:
    Camera();
    
    void setDistance(float distance);
    void setTarget(float x, float y, float z);
    void setUp(float x, float y, float z);
    
    void rotate(float deltaX, float deltaY);
    void zoom(float delta);
    
    void getPosition(float* pos);
    void getTarget(float* target);
    void getUp(float* up);
    
    float getDistance() const { return m_distance; }
};

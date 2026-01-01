#include "camera.h"
#include <cstdio>
#include "utils.h"

Camera::Camera() {
    m_target[0] = 0.0f;
    m_target[1] = 0.0f; 
    m_target[2] = 0.0f;
    
    m_up[0] = 0.0f;
    m_up[1] = 1.0f;
    m_up[2] = 0.0f;
    
    m_yaw = 0.0f;
    m_pitch = 0.3f;
    m_distance = 38.0f;
    
    updatePosition();
}

void Camera::updatePosition() {
    m_position[0] = m_target[0] + m_distance * cos(m_pitch) * sin(m_yaw);
    m_position[1] = m_target[1] + m_distance * sin(m_pitch);
    m_position[2] = m_target[2] + m_distance * cos(m_pitch) * cos(m_yaw);
}

void Camera::rotate(float dx, float dy) {
    float sensitivity = 0.005f * (1.0f + m_distance * 0.05f);
    
    m_yaw += dx * sensitivity;
    m_pitch += dy * sensitivity;
    
    if (m_pitch < -1.57f) m_pitch = -1.57f;
    if (m_pitch > 1.57f) m_pitch = 1.57f;
    
    if (m_yaw > 6.283f) m_yaw -= 6.283f;
    if (m_yaw < -6.283f) m_yaw += 6.283f;
    
    wprintf(L"相机旋转: 水平角=%f 俯仰角=%f\n", m_yaw, m_pitch);
    updatePosition();
}

void Camera::zoom(float delta) {
    wprintf(L"Zoom: delta=%f\n", delta);
    if (delta > 0) {
        m_distance *= 0.9f;
    } else if (delta < 0) {
        m_distance *= 1.1f;
    }
    
    if (m_distance < 1.0f) m_distance = 1.0f;
    if (m_distance > 1000.0f) m_distance = 1000.0f;
    wprintf(L"缩放: 距离=%f\n", m_distance);
    updatePosition();
}

void Camera::getPosition(float* pos) {
    pos[0] = m_position[0];
    pos[1] = m_position[1];
    pos[2] = m_position[2];
}

void Camera::getTarget(float* target) {
    target[0] = m_target[0];
    target[1] = m_target[1];
    target[2] = m_target[2];
}

void Camera::getUp(float* up) {
    up[0] = m_up[0];
    up[1] = m_up[1];
    up[2] = m_up[2];
}

void Camera::setDistance(float distance) {
    m_distance = distance;
    if (m_distance < 1.0f) m_distance = 1.0f;
    if (m_distance > 1000.0f) m_distance = 1000.0f;
    updatePosition();
}

void Camera::setTarget(float x, float y, float z) {
    m_target[0] = x;
    m_target[1] = y;
    m_target[2] = z;
    updatePosition();
}

void Camera::setUp(float x, float y, float z) {
    m_up[0] = x;
    m_up[1] = y;
    m_up[2] = z;
}

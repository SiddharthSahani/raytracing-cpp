
#include "src/camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#ifndef RT_NO_RAYLIB
#include <raylib.h>
#endif


namespace rt {

Camera::Camera(float fov, const glm::ivec2& size, const glm::vec3& position, const glm::vec3& direction)
: m_size(size), m_position(position), m_direction(direction) {
    float near_clip = 0.1f;
    float far_clip = 100.0f;
    m_inv_projection = glm::inverse(glm::perspectiveFov(
        glm::radians(fov), (float) size.x, (float) size.y, near_clip, far_clip
    ));

    glm::vec3 up_direction = {0, 1, 0};
    m_inv_view = glm::inverse(glm::lookAt(
        position, position + direction, up_direction
    ));

    m_rays.resize(size.x * size.y);
    for (uint32_t x = 0; x < size.x; x++) {
        for (uint32_t y = 0; y < size.y; y++) {
            glm::vec2 coord = {x / (float) size.x, y / (float) size.y};
            coord = coord * 2.0f - 1.0f;
            
            glm::vec4 target = m_inv_projection * glm::vec4(coord, 1.0f, 1.0f);
            glm::vec3 ray_direction = glm::vec3(m_inv_view * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0.0f));
            m_rays[x + y*size.x] = {m_position, ray_direction};
        }
    }
}


Ray Camera::get_ray(uint32_t x, uint32_t y) const {
    return m_rays[x + y*m_size.x];
}


#ifndef RT_NO_RAYLIB

bool Camera::update(float timestep) {
    const float sensitivity = 0.002f;
    const float speed = 5.0f;
    const float rotation_speed = 2.0f;

    static bool rmb_toggle_flag = false; // rmb: right mouse button

    auto _delta = GetMouseDelta();
    glm::vec2 delta = {_delta.x*sensitivity, _delta.y*sensitivity};

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        rmb_toggle_flag = true;
        DisableCursor();
    }

    if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        if (rmb_toggle_flag) {
            rmb_toggle_flag = false;
            EnableCursor();
        }
        return false;
    }

    bool moved = false;

    glm::vec3 up_direction = {0, 1, 0};
    glm::vec3 right_direction = glm::cross(m_direction, up_direction);

    if (IsKeyDown(KEY_W)) {
        m_position += m_direction * speed * timestep;
        moved = true;
    }
    if (IsKeyDown(KEY_S)) {
        m_position -= m_direction * speed * timestep;
        moved = true;
    }
    if (IsKeyDown(KEY_A)) {
        m_position -= right_direction * speed * timestep;
        moved = true;
    }
    if (IsKeyDown(KEY_D)) {
        m_position += right_direction * speed * timestep;
        moved = true;
    }
    if (IsKeyDown(KEY_Q)) {
        m_position += up_direction * speed * timestep;
        moved = true;
    }
    if (IsKeyDown(KEY_E)) {
        m_position -= up_direction * speed * timestep;
        moved = true;
    }

    // mouse movement
    if (delta.x != 0.0f || delta.y != 0.0f) {
        float yaw_delta = delta.x * rotation_speed;
        float pitch_delta = delta.y * rotation_speed;

        glm::quat q = glm::normalize(glm::cross(
            glm::angleAxis(-pitch_delta, right_direction),
            glm::angleAxis(-yaw_delta, up_direction)
        ));

        m_direction = glm::rotate(q, m_direction);

        moved = true;
    }

    if (moved) {
        // recalculating inverse view
        m_inv_view = glm::inverse(glm::lookAt(
            m_position, m_position + m_direction, up_direction
        ));
        // caching rays
        for (uint32_t x = 0; x < m_size.x; x++) {
            for (uint32_t y = 0; y < m_size.y; y++) {
                glm::vec2 coord = {x / (float) m_size.x, y / (float) m_size.y};
                coord = coord * 2.0f - 1.0f;
                
                glm::vec4 target = m_inv_projection * glm::vec4(coord, 1.0f, 1.0f);
                glm::vec3 ray_direction = glm::vec3(m_inv_view * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0.0f));
                m_rays[x + y*m_size.x] = {m_position, ray_direction};
            }
        }
    }

    return moved;
}

#endif

}

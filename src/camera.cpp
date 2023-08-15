
#include "src/camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>


namespace rt {

Camera::Camera(float fov, const glm::ivec2& size, const glm::vec3& position, const glm::vec3& direction)
: m_position(position) {
    float near_clip = 0.1f;
    float far_clip = 100.0f;
    m_inv_projection = glm::inverse(glm::perspectiveFov(
        glm::radians(fov), (float) size.x, (float) size.y, near_clip, far_clip
    ));

    glm::vec3 up_direction = {0, 1, 0};
    m_inv_view = glm::inverse(glm::lookAt(
        position, position + direction, up_direction
    ));
}


Ray Camera::get_ray(float u, float v) const {
    // u and v are from 0 to 1
    glm::vec2 coord = {u, v};
    coord = coord * 2.0f - 1.0f;
    
    glm::vec4 target = m_inv_projection * glm::vec4(coord, 1.0f, 1.0f);
    glm::vec3 ray_direction = glm::vec3(m_inv_view * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0.0f));
    return {m_position, ray_direction};
}

}

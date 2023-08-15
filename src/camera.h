
#pragma once

#include "src/ray.h"
#include <glm/glm.hpp>


namespace rt {

class Camera {
    
    public:
        Camera(float fov, const glm::ivec2& size, const glm::vec3& position, const glm::vec3& direction);
        Ray get_ray(float u, float v) const; // returns the ray for the uv coordinates
    
    private:
        glm::vec3 m_position; // current 'eye' position
        // needed for calculations
        glm::mat4 m_inv_projection;
        glm::mat4 m_inv_view;

};

}

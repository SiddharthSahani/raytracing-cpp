
#pragma once

#include "src/ray.h"
#include <glm/glm.hpp>
#include <vector>


namespace rt {

class Camera {
    
    public:
        Camera(float fov, const glm::ivec2& size, const glm::vec3& position, const glm::vec3& direction);
        Ray get_ray(uint32_t x, uint32_t y) const; // returns the ray for the xy pixel
#ifndef RT_NO_RAYLIB
        bool update(float timestep); // handles movement and returns true if moved
#endif

    private:
        glm::ivec2 m_size;
        glm::vec3 m_position; // current 'eye' position
        glm::vec3 m_direction; // camera direction
        // needed for calculations
        glm::mat4 m_inv_projection;
        glm::mat4 m_inv_view;
        // caching the rays
        std::vector<Ray> m_rays;

};

}

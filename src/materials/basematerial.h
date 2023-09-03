
#pragma once

#include "src/hit_payload.h"


namespace rt {

class Material {
    public:
        // returns true if the material doesnt absorb the ray
        virtual bool scatter(glm::vec3& attenuation, const glm::vec3& ray_direction, const glm::vec3& normal, glm::vec3& new_ray_direction) const = 0;
        // returns the emitted color (implemented for emissive material)
        virtual glm::vec3 emitted() const { return {0.0f, 0.0f, 0.0f}; }
};

}

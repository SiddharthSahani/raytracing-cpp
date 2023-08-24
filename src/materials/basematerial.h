
#pragma once

#include "src/hit_payload.h"


namespace rt {

class Material {
    public:
        // returns if the ray should bounce more or not
        virtual bool scatter(glm::vec3& attenuation, const glm::vec3& ray_direction, const glm::vec3& normal, glm::vec3& new_ray_direction) const = 0;
};

}

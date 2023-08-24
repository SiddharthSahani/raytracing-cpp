
#pragma once

#include "src/ray.h"
#include "src/hit_payload.h"


namespace rt {

class Material {
    public:
        virtual bool scatter(
            const Ray& r_in, const HitPayload& payload, glm::vec3& attenuation, glm::vec3& new_ray_direction) const = 0;
};

}

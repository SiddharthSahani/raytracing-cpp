
#pragma once

#include "src/ray.h"
#include "src/hit_payload.h"
#include <glm/glm.hpp>


namespace rt {

class Object {
    public:
        // returns true if the object has been hit by ray
        // only if the hit distance is lower than the previous hit distance
        // modifies the payload (except for the object_index)
        virtual bool hit(const Ray& ray, HitPayload& payload) const = 0;
};

}

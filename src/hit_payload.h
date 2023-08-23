
#pragma once

#include <glm/glm.hpp>
#include <memory>


namespace rt {

// forward declaration
struct Material;

struct HitPayload {
    float hit_distance;
    glm::vec3 world_position;
    glm::vec3 world_normal;
    std::shared_ptr<Material> material;
};

}

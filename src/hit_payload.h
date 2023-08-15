
#pragma once

#include <glm/glm.hpp>


namespace rt {

struct HitPayload {
    float hit_distance;
    glm::vec3 world_position;
    glm::vec3 world_normal;
    int object_index;
};

}

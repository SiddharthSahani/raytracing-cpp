
#pragma once

#include <glm/glm.hpp>


namespace rt {

struct Sphere {
    glm::vec3 position;
    float radius;
    int material_index;
};

}

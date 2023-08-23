
#pragma once

#include "src/geometries/baseobject.h"
#include <vector>
#include <memory>


namespace rt {

struct Scene {
    std::vector<std::shared_ptr<Object>> objects;
    glm::vec3 sky_color;
};

}


#pragma once

#include "src/material.h"
#include "src/baseobject.h"
#include <vector>
#include <memory>


namespace rt {

struct Scene {
    std::vector<Material> materials;
    std::vector<std::shared_ptr<Object>> objects;
};

}

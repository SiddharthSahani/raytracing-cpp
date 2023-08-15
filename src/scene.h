
#pragma once

#include "src/material.h"
#include "src/sphere.h"
#include <vector>


namespace rt {

struct Scene {
    std::vector<Material> materials;
    std::vector<Sphere> spheres;
};

}

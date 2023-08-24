
#pragma once

#include "src/materials/basematerial.h"
#include "src/utils.h"


namespace rt {

class Metal : public Material {

    public:
        Metal(const glm::vec3& albedo);
        bool scatter(glm::vec3& attenuation, const glm::vec3& ray_direction, const glm::vec3& normal, glm::vec3& new_ray_direction) const override;
    
    private:
        glm::vec3 m_albedo;

};


Metal::Metal(const glm::vec3& albedo)
: m_albedo(albedo) {
}


bool Metal::scatter(glm::vec3& attenuation, const glm::vec3& ray_direction, const glm::vec3& normal, glm::vec3& new_ray_direction) const {
    new_ray_direction = glm::reflect(ray_direction, normal);
    attenuation = m_albedo;
    return true;
}

}

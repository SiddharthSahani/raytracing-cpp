
#pragma once

#include "src/materials/basematerial.h"
#include "src/utils.h"


namespace rt {

class Metal : public Material {

    public:
        Metal(const glm::vec3& albedo, float fuzz);
        bool scatter(glm::vec3& attenuation, const glm::vec3& ray_direction, const glm::vec3& normal, glm::vec3& new_ray_direction) const override;
    
    private:
        glm::vec3 m_albedo;
        float m_fuzz;

};


Metal::Metal(const glm::vec3& albedo, float fuzz)
: m_albedo(albedo), m_fuzz(fuzz) {
}


bool Metal::scatter(glm::vec3& attenuation, const glm::vec3& ray_direction, const glm::vec3& normal, glm::vec3& new_ray_direction) const {
    glm::vec3 random_vec3 = m_fuzz * utils::random_vec3_in_unit_sphere();
    new_ray_direction = glm::reflect(ray_direction, glm::normalize(normal + random_vec3));
    attenuation = m_albedo;
    return true;
}

}

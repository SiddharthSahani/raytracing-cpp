
#pragma once

#include "src/materials/basematerial.h"
#include "src/utils.h"


namespace rt {

class Lambertian : public Material {

    public:
        Lambertian(const glm::vec3& albedo);
        bool scatter(glm::vec3& attenuation, const glm::vec3& ray_direction, const glm::vec3& normal, glm::vec3& new_ray_direction) const override;

    private:
        glm::vec3 m_albedo;

};


Lambertian::Lambertian(const glm::vec3& albedo)
: m_albedo(albedo) {
}


bool Lambertian::scatter(glm::vec3& attenuation, const glm::vec3& ray_direction, const glm::vec3& normal, glm::vec3& new_ray_direction) const {
    // ray_direction is unused here cause we choose a random vec3
    new_ray_direction = glm::normalize(normal + utils::random_vec3_in_unit_sphere());
    attenuation = m_albedo;
    return true;
}

}

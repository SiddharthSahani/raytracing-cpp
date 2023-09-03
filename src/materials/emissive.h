

#pragma once

#include "src/materials/basematerial.h"
#include "src/utils.h"


namespace rt {

class Emissive : public Material {

    public:
        Emissive(const glm::vec3& color, float power);
        bool scatter(glm::vec3& attenuation, const glm::vec3& ray_direction, const glm::vec3& normal, glm::vec3& new_ray_direction) const override;
        glm::vec3 emitted() const override { return m_color * m_power; }
    
    private:
        glm::vec3 m_color;
        float m_power;

};


Emissive::Emissive(const glm::vec3& color, float power)
: m_color(color), m_power(power) {
}


bool Emissive::scatter(glm::vec3& attenuation, const glm::vec3& ray_direction, const glm::vec3& normal, glm::vec3& new_ray_direction) const {
    new_ray_direction = glm::normalize(normal + utils::random_vec3_in_unit_sphere());
    attenuation = m_color;
    return true;
}

}



#pragma once

#include "src/materials/basematerial.h"
#include "src/utils.h"


namespace rt {

class Lambertian : public Material {

    public:
        Lambertian(const glm::vec3& albedo);
        bool scatter(const Ray& r_in, const HitPayload& payload, glm::vec3& attenuation, glm::vec3& new_ray_direction) const override;

    private:
        glm::vec3 m_albedo;

};


Lambertian::Lambertian(const glm::vec3& albedo)
: m_albedo(albedo) {
}


bool Lambertian::scatter(const Ray& r_in, const HitPayload& payload, glm::vec3& attenuation, glm::vec3& new_ray_direction) const {
    new_ray_direction = glm::normalize(payload.world_normal + utils::random_vec3_in_unit_sphere());
    attenuation = m_albedo;
    return true;
}

}

#pragma once

#include "src/baseobject.h"


namespace rt {

class Sphere : public Object {

    public:
        Sphere(const glm::vec3& position, float radius, int material_index);
        bool hit(const Ray& ray, HitPayload& payload) const override;
        int get_material_index() const override { return m_material_index; }
    
    private:
        glm::vec3 m_position;
        float m_radius;
        int m_material_index;

};


Sphere::Sphere(const glm::vec3& position, float radius, int material_index)
: m_position(position), m_radius(radius), m_material_index(material_index) {
}


bool Sphere::hit(const Ray& ray, HitPayload& payload) const {
    glm::vec3 origin = ray.origin - m_position;

    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(origin, ray.direction);
    float c = glm::dot(origin, origin) - m_radius * m_radius;

    float d = b * b - 4.0f * a * c;
    if (d < 0.0f) {
        return false;
    }

    float t1 = (-b - glm::sqrt(d)) / (2.0f * a);
    if (t1 > 0.0f && t1 < payload.hit_distance) {
        payload.hit_distance = t1;
        payload.world_position = ray.origin + ray.direction * t1;
        payload.world_normal = glm::normalize(payload.world_position - m_position);
        return true;
    }

    return false;
}

}

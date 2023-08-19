
#pragma once

#include "src/baseobject.h"
#include <glm/gtx/intersect.hpp>


namespace rt {

class Plane : public Object {

    public:
        Plane(const glm::vec3& normal, const glm::vec3& point, int material_index);
        bool hit(const Ray& ray, HitPayload& payload) const override;
        int get_material_index() const override { return m_material_index; }
    
    private:
        glm::vec3 m_normal;
        glm::vec3 m_point; // a point in the plane
        int m_material_index;

};


Plane::Plane(const glm::vec3& normal, const glm::vec3& point, int material_index)
: m_normal(normal), m_point(point), m_material_index(material_index) {
}


bool Plane::hit(const Ray& ray, HitPayload& payload) const {
    float hit_distance;

    if (glm::intersectRayPlane(ray.origin, ray.direction, m_point, m_normal, hit_distance)) {
        if (hit_distance > 0.0f && hit_distance < payload.hit_distance) {
            payload.hit_distance = hit_distance;
            payload.world_position = ray.origin + ray.direction * hit_distance;
            // changing the normal's direction if it goes into the plane
            payload.world_normal = glm::dot(ray.direction, m_normal) > 0.0f ? -m_normal : m_normal;
            return true;
        }
    }

    return false;
}

}


#pragma once

#include "src/geometries/baseobject.h"
#include <glm/gtx/intersect.hpp>


namespace rt {

class Triangle : public Object {
    public:
        Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, int material_idx);
        bool hit(const Ray& ray, HitPayload& payload) const override;
        int get_material_index() const override { return m_material_index; }    

    private:
        glm::vec3 m_v0, m_v1, m_v2; // vertices of the triangle
        int m_material_index;

};


Triangle::Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, int material_index)
: m_v0(v0), m_v1(v1), m_v2(v2), m_material_index(material_index) {
}


bool Triangle::hit(const Ray& ray, HitPayload& payload) const {
    float hit_distance;
    glm::vec2 bary_position; // not used

    if (glm::intersectRayTriangle(ray.origin, ray.direction, m_v0, m_v1, m_v2, bary_position, hit_distance)) {
        if (hit_distance > 0.0f && hit_distance < payload.hit_distance) {
            payload.hit_distance = hit_distance;
            payload.world_position = ray.origin + ray.direction * hit_distance;
            glm::vec3 normal = glm::normalize(glm::cross(m_v0 - m_v1, m_v0 - m_v2));
            // changing the normal's direction if it goes into the triangle's plane
            payload.world_normal = glm::dot(ray.direction, normal) > 0.0f ? -normal : normal;
            return true;
        }
    }

    return false;
}

}

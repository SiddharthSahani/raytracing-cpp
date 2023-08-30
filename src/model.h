
#pragma once

#include "src/geometries/triangle.h"
#include <glm/ext/matrix_transform.hpp>
#include <vector>


namespace rt {

class Model : public Object {

    public:
        Model(const std::vector<Triangle>& triangles);

        // first vec3 is scale
        // second vec3 is rotation
        // third vec3 is translation
        // void apply_transformation(const glm::mat3& transformation);

        void apply_transform(const glm::vec3& translate, const glm::vec3& rotate, const glm::vec3& scale);

        bool hit(const Ray& ray, HitPayload& payload) const override;


    private:
        // each model is just a bunch of triangles
        std::vector<Triangle> m_triangles;

};


Model::Model(const std::vector<Triangle>& triangles)
: m_triangles(triangles) {
}


void Model::apply_transform(const glm::vec3& translate, const glm::vec3& rotate, const glm::vec3& scale) {
    glm::mat4 m(1.0f);

    // scale
    m = glm::scale(m, scale);

    // rotate
    m = glm::rotate(m, glm::radians(rotate.x), {1, 0, 0}); // x axis
    m = glm::rotate(m, glm::radians(rotate.y), {0, 1, 0}); // y axis
    m = glm::rotate(m, glm::radians(rotate.z), {0, 0, 1}); // z axis

    // translate
    m = glm::translate(m, translate);

    for (uint32_t i = 0; i < m_triangles.size(); i++) {
        Triangle& triangle = m_triangles[i];
        
        triangle.m_v0 = m * glm::vec4(triangle.m_v0, 1.0f);
        triangle.m_v1 = m * glm::vec4(triangle.m_v1, 1.0f);
        triangle.m_v2 = m * glm::vec4(triangle.m_v2, 1.0f);
    }

}


bool Model::hit(const Ray& ray, HitPayload& payload) const {
    bool hits_anything = false;

    for (const Triangle& triangle : m_triangles) {
        hits_anything |= triangle.hit(ray, payload);
    }

    return hits_anything;
}

}

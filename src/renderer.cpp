
#include "src/renderer.h"
#include "src/utils.h"
#include <cstring> // memset


namespace rt {

Renderer::Renderer(const glm::ivec2& size, const Scene& scene, const Camera& camera)
: m_size(size), m_scene(scene), m_camera(camera) {
    m_pixels.resize(size.x * size.y);
    m_accumulation_data.resize(size.x * size.y);

    Image temp = GenImageColor(size.x, size.y, RED);
    m_texture = LoadTextureFromImage(temp);
    UnloadImage(temp);
}


Renderer::~Renderer() {
    UnloadTexture(m_texture);
}


void Renderer::update() {
    if (m_frame_index == 1) {
        memset(&m_accumulation_data[0], 0, m_size.x*m_size.y * sizeof(glm::vec3));
    }

    uint32_t width = m_size.x;
    uint32_t height = m_size.y;

    #pragma omp parallel for
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            glm::vec3 color = per_pixel(x, y);
            m_accumulation_data[x + y*width] += color;

            glm::vec3 accumulated_color = m_accumulation_data[x + y*width];
            accumulated_color /= (float) m_frame_index;
            accumulated_color = glm::clamp(accumulated_color, glm::vec3(0.0f), glm::vec3(1.0f));

            m_pixels[x + (height-y-1)*width] = utils::convert_to_rgba(accumulated_color);
        }
    }

    m_frame_index++;
}


void Renderer::reload() {
    UpdateTexture(m_texture, &m_pixels[0]);
}


void Renderer::render(float scale) {
    DrawTextureEx(m_texture, {0, 0}, 0.0f, scale, WHITE);
}


void Renderer::reset_frame_index() {
    m_frame_index = 1;
}


glm::vec3 Renderer::per_pixel(uint32_t x, uint32_t y) {
    Ray ray = m_camera.get_ray(x, y);

    glm::vec3 light = {0, 0, 0};
    glm::vec3 contribution = {1, 1, 1};

    int bounces = 5;
    for (int i = 0; i < bounces; i++) {
        HitPayload payload = trace_ray(ray);

        if (payload.hit_distance == FLT_MAX) {
            glm::vec3 sky_color = {0.6, 0.7, 0.9};
            light += sky_color * contribution;
            break;
        }

        const auto& sphere = m_scene.spheres[payload.object_index];
        const Material& material = m_scene.materials[sphere->get_material_index()];

        contribution *= material.albedo;

        ray.origin = payload.world_position + payload.world_normal * 0.0001f; // small bias so the ray doesnt start exactly at the surface
        ray.direction = glm::normalize(payload.world_normal + utils::random_vec3()); // new random direction
    }

    return light;
}


HitPayload Renderer::trace_ray(const Ray& ray) {
    HitPayload payload;
    payload.hit_distance = FLT_MAX;

    for (uint32_t i = 0; i < m_scene.spheres.size(); i++) {
        const auto& sphere = m_scene.spheres[i];
        if (sphere->hit(ray, payload)) {
            payload.object_index = i;
        }
    }

    return payload;
}

}

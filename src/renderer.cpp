
#include "src/renderer.h"
#include "src/material.h"
#include "src/utils.h"
#ifdef RT_NO_RAYLIB
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include <external/stb_image_write.h>
#include <cstring> // memset


namespace rt {

Renderer::Renderer(const glm::ivec2& size, const Scene& scene, const Camera& camera)
: m_size(size), m_scene(scene), m_camera(camera) {
    m_pixels.resize(size.x * size.y);
    m_accumulation_data.resize(size.x * size.y);
    memset(&m_accumulation_data[0], 0, m_size.x*m_size.y * sizeof(glm::vec3));

#ifndef RT_NO_RAYLIB
    Image temp = GenImageColor(size.x, size.y, RED);
    m_texture = LoadTextureFromImage(temp);
    UnloadImage(temp);
#endif

}


#ifndef RT_NO_RAYLIB

Renderer::~Renderer() {
    UnloadTexture(m_texture);
}

#endif


#ifdef RT_NO_RAYLIB
void Renderer::update(uint32_t frame_count) {
#else
void Renderer::update() {
    if (m_frame_index == 1) {
        memset(&m_accumulation_data[0], 0, m_size.x*m_size.y * sizeof(glm::vec3));
    }
#endif

    uint32_t width = m_size.x;
    uint32_t height = m_size.y;

#ifdef RT_NO_RAYLIB
    for (uint32_t i = 0; i < frame_count; i++) {
#endif

    #pragma omp parallel for
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            glm::vec3 color = per_pixel(x, y);
            m_accumulation_data[x + y*width] += color;

#ifndef RT_NO_RAYLIB
            glm::vec3 accumulated_color = m_accumulation_data[x + y*width];
            accumulated_color /= (float) m_frame_index;
            accumulated_color = glm::clamp(accumulated_color, glm::vec3(0.0f), glm::vec3(1.0f));

            m_pixels[x + (height-y-1)*width] = utils::convert_to_rgba(accumulated_color);
#endif
        }
    }

    m_frame_index++;

#ifdef RT_NO_RAYLIB
    }
#endif
}

#ifndef RT_NO_RAYLIB

void Renderer::reload() {
    UpdateTexture(m_texture, &m_pixels[0]);
}


void Renderer::render(float scale) {
    DrawTextureEx(m_texture, {0, 0}, 0.0f, scale, WHITE);
}


void Renderer::reset_frame_index() {
    m_frame_index = 1;
}


uint32_t Renderer::get_frame_index() const {
    return m_frame_index;
}

#endif


const uint32_t* Renderer::get_pixels() const {
    return m_pixels.data();
}


bool Renderer::save_to_file(const char* filepath) {

#ifdef RT_NO_RAYLIB
    uint32_t width = m_size.x;
    uint32_t height = m_size.y;

    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            glm::vec3 accumulated_color = m_accumulation_data[x + y*width];
            accumulated_color /= (float) m_frame_index;
            accumulated_color = glm::clamp(accumulated_color, glm::vec3(0.0f), glm::vec3(1.0f));

            m_pixels[x + (height-y-1)*width] = utils::convert_to_rgba(accumulated_color);   
        }
    }
#endif

    return stbi_write_png(filepath, m_size.x, m_size.y, 4, &m_pixels[0], m_size.x*4);
}


glm::vec3 Renderer::per_pixel(uint32_t x, uint32_t y) {
    Ray ray = m_camera.get_ray(x, y);

    glm::vec3 light = {0, 0, 0};
    glm::vec3 contribution = {1, 1, 1};

    utils::rng_seed = x + y*m_size.x;
    utils::rng_seed += m_frame_index * 32421;

    int bounces = 5;
    for (int i = 0; i < bounces; i++) {
        utils::rng_seed += i * i * i;

        HitPayload payload = trace_ray(ray);

        if (payload.hit_distance == FLT_MAX) {
            light += m_scene.sky_color * contribution;
            break;
        }

        const auto& material = payload.material;

        contribution *= material->albedo;

        ray.origin = payload.world_position + payload.world_normal * 0.0001f; // small bias so the ray doesnt start exactly at the surface
        ray.direction = glm::normalize(payload.world_normal + utils::random_vec3_in_unit_sphere()); // new random direction
    }

    return light;
}


HitPayload Renderer::trace_ray(const Ray& ray) {
    HitPayload payload;
    payload.hit_distance = FLT_MAX;

    for (uint32_t i = 0; i < m_scene.objects.size(); i++) {
        const auto& object = m_scene.objects[i];
        object->hit(ray, payload);
    }

    return payload;
}

}

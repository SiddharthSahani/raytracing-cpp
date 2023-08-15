
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

    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            glm::vec3 color = per_pixel(x, y);
            m_accumulation_data[x + y*width] += color;

            glm::vec3 accumulated_color = m_accumulation_data[x + y*width];
            accumulated_color /= (float) m_frame_index;
            accumulated_color = glm::clamp(accumulated_color, glm::vec3(0.0f), glm::vec3(1.0f));

            uint32_t pixel_position = (width-x-1) + (height-y-1) * width;
            m_pixels[pixel_position] = utils::convert_to_rgba(accumulated_color);
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


glm::vec3 Renderer::per_pixel(uint32_t x, uint32_t y) {
    Ray ray = m_camera.get_ray(x / (float) m_size.x, y / (float) m_size.y);

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

        const Sphere& sphere = m_scene.spheres[payload.object_index];
        const Material& material = m_scene.materials[sphere.material_index];

        contribution *= material.albedo;

        ray.origin = payload.world_position + payload.world_normal * 0.0001f; // small bias so the ray doesnt start exactly at the surface
        ray.direction = glm::normalize(payload.world_normal + utils::random_vec3()); // new random direction
    }

    return light;
}


HitPayload Renderer::trace_ray(const Ray& ray) {
    int closest_sphere = -1;
    float hit_distance = FLT_MAX;
    for (int i = 0; i < m_scene.spheres.size(); i++) {
        const Sphere& sphere = m_scene.spheres[i];

        // shifting the origin
        glm::vec3 origin = ray.origin - sphere.position;

        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(origin, ray.direction);
        float c = glm::dot(origin, origin) - sphere.radius * sphere.radius;

        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0.0f) {
            // no solution to the quadtratic equation
            continue;
        }

        float t = (-b - glm::sqrt(discriminant)) / (2.0f * a);
        if (t > 0.0f && t < hit_distance) {
            // this sphere is closer
            hit_distance = t;
            closest_sphere = i;
        }
    }

    HitPayload payload;
    payload.hit_distance = hit_distance;

    if (payload.hit_distance != FLT_MAX) {
        // the object did hit something
        const Sphere& sphere = m_scene.spheres[closest_sphere];
        glm::vec3 origin = ray.origin - sphere.position;

        payload.world_position = origin + ray.direction * payload.hit_distance;
        payload.world_normal = glm::normalize(payload.world_position);
        // unshifting the origin
        payload.world_position += sphere.position;
        payload.object_index = closest_sphere;
    }

    return payload;
}

}

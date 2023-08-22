
#pragma once

#include "src/hit_payload.h"
#include "src/scene.h"
#include "src/camera.h"
#ifndef RT_NO_RAYLIB
#include <raylib.h>
#endif


namespace rt {

class Renderer {

    public:
        Renderer(const glm::ivec2& size, const Scene& scene, const Camera& camera);
#ifdef RT_NO_RAYLIB
        void update(uint32_t frame_count); // its like calling update() multiple times
#else
        ~Renderer();
        void update(); // update the pixels
        void reload(); // update the texture
        void render(float scale); // draw the underlying texture to the screen
        void reset_frame_index(); // call this to start again
        uint32_t get_frame_index() const;
#endif
        const uint32_t* get_pixels() const;
        bool save_to_file(const char* filepath); // saves the output as an image returns true if successfully saved

    private:
        glm::vec3 per_pixel(uint32_t x, uint32_t y); // returns the color at that position
        HitPayload trace_ray(const Ray& ray); // simulates the ray

    private:
        glm::ivec2 m_size;
        const Scene& m_scene; // current scene
        const Camera& m_camera; // current camera

        std::vector<uint32_t> m_pixels;
        std::vector<glm::vec3> m_accumulation_data;
        uint32_t m_frame_index = 1;
#ifndef RT_NO_RAYLIB
        Texture2D m_texture; // underlying raylib texture that gets drawn on screen
#endif

};

}


#pragma once

#include <glm/glm.hpp>


namespace utils {

inline uint32_t rng_seed;


uint32_t convert_to_rgba(const glm::vec3& color) {
    // color is from 0 to 1
    uint8_t r = color.r * 255;
    uint8_t g = color.g * 255;
    uint8_t b = color.b * 255;
    uint8_t a = 255;
    
    // combining the different components by 'or'ing them together
    return (a << 24) | (b << 16) | (g << 8) | r;
}


uint32_t pcg_hash(uint32_t input) {
    uint32_t state = input * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}


float random_float() {
    rng_seed = pcg_hash(rng_seed);
    return (float) rng_seed / UINT32_MAX;
}


glm::vec3 random_vec3_in_unit_sphere() {
    float x = random_float();
    float y = random_float();
    float z = random_float();
    glm::vec3 vec = {x, y, z};
    return glm::normalize(vec * 2.0f - 1.0f);
}

}

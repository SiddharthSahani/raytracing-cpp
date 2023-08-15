
#pragma once

#include <glm/glm.hpp>
#include <random>


namespace utils {

uint32_t convert_to_rgba(const glm::vec3& color) {
    // color is from 0 to 1
    uint8_t r = color.r * 255;
    uint8_t g = color.g * 255;
    uint8_t b = color.b * 255;
    uint8_t a = 255;
    
    // combining the different components by 'or'ing them together
    return (a << 24) | (b << 16) | (g << 8) | r;
}


float random_float() {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    return dist(mt);
}


glm::vec3 random_vec3() {
    float x = random_float();
    float y = random_float();
    float z = random_float();
    return {x, y, z};
}

}

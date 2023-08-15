
#include "src/renderer.h"
#include <raylib.h>


rt::Scene create_scene() {
    rt::Scene scene;

    // creating materials
    {
        rt::Material& material = scene.materials.emplace_back();
        material.albedo = {1.0, 0.0, 1.0};
    }
    {
        rt::Material& material = scene.materials.emplace_back();
        material.albedo = {0.2, 0.3, 1.0};
    }
    {
        rt::Material& material = scene.materials.emplace_back();
        material.albedo = {0.8, 0.5, 0.2};
    }

    // creating spheres
    {
        rt::Sphere& sphere = scene.spheres.emplace_back();
        sphere.position = {0.0, 0.0, 0.0};
        sphere.radius = 1.0;
        sphere.material_index = 0;
    }
    {
        rt::Sphere& sphere = scene.spheres.emplace_back();
        sphere.position = {0.0, -101.0, 0.0};
        sphere.radius = 100.0;
        sphere.material_index = 1;
    }
    {
        rt::Sphere& sphere = scene.spheres.emplace_back();
        sphere.position = {2.0, 0.0, 0.0};
        sphere.radius = 1.0;
        sphere.material_index = 2;
    }

    return scene;
}


int main() {
    const int screen_width = 1080;
    const int screen_height = 720;
    const float scale = 3.0f;
    const int image_width = screen_width / scale;
    const int image_height = screen_height / scale;

    InitWindow(screen_width, screen_height, "Raytracing with c++");
    SetTargetFPS(30);

    rt::Scene scene = create_scene();

    glm::vec3 camera_position = {0, 0, 6};
    glm::vec3 camera_direction = {0, 0, -1};
    rt::Camera camera(45.0f, {image_width, image_height}, camera_position, camera_direction);

    rt::Renderer renderer({image_width, image_height}, scene, camera);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        renderer.update();
        renderer.reload();
        renderer.render(scale);

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
}

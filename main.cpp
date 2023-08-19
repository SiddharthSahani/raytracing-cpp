
#include "src/renderer.h"
#include "src/sphere.h"
#include "src/triangle.h"
#include <raylib.h>


rt::Scene create_test_scene_1() {
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
    {
        rt::Material& material = scene.materials.emplace_back();
        material.albedo = {0.1, 1.0, 0.1};
    }
    // creating objects
    {
        glm::vec3 position = {0.0, 0.0, 0.0};
        float radius = 1.0;
        int material_index = 0;
        scene.objects.emplace_back(new rt::Sphere(position, radius, material_index));
    }
    {
        glm::vec3 position = {0.0, -101.0, 0.0};
        float radius = 100.0;
        int material_index = 1;
        scene.objects.emplace_back(new rt::Sphere(position, radius, material_index));
    }
    {
        glm::vec3 position = {2.0, 0.0, 0.0};
        float radius = 1.0;
        int material_index = 2;
        scene.objects.emplace_back(new rt::Sphere(position, radius, material_index));
    }
    {
        glm::vec3 v0 = {-1.0, -1, -1};
        glm::vec3 v1 = {-1.0,  1, -1};
        glm::vec3 v2 = {-1.5,  0,  2};
        int material_index = 3;
        scene.objects.emplace_back(new rt::Triangle(v0, v1, v2, material_index));
    }
    
    scene.sky_color = {230, 230, 250};
    scene.sky_color /= 255.0f;
    return scene;
}


rt::Scene create_test_scene_2() {
    rt::Scene scene;
    // creating materials
    {
        rt::Material& material = scene.materials.emplace_back();
        material.albedo = {0.2, 0.6, 1.0};
    }
    {
        rt::Material& material = scene.materials.emplace_back();
        material.albedo = {0.2, 0.6, 0.0};
    }
    {
        rt::Material& material = scene.materials.emplace_back();
        material.albedo = {1, 0, 0};
    }
    // creating objects
    {
        glm::vec3 position = {0.0, 0.0, 0.0};
        float radius = 1.0;
        int material_index = 0;
        scene.objects.emplace_back(new rt::Sphere(position, radius, material_index));
    }
    {
        glm::vec3 position = {0.0, -101.0, 0.0};
        float radius = 100.0;
        int material_index = 1;
        scene.objects.emplace_back(new rt::Sphere(position, radius, material_index));
    }
    {
        const float size = 3;
		glm::vec3 v0 = {-size, -1,  size};
		glm::vec3 v1 = {-size, -1, -size};
		glm::vec3 v2 = { size, -0.5, -size};
		glm::vec3 v3 = { size, -0.5,  size};
        int material_index = 2;
        scene.objects.emplace_back(new rt::Triangle(v0, v1, v2, material_index));
        scene.objects.emplace_back(new rt::Triangle(v2, v3, v0, material_index));
    }
    
    scene.sky_color = {104, 184, 235};
    scene.sky_color /= 255.0f;
    return scene;
}


int check_if_scene_changed() {
    if (IsKeyDown(KEY_C)) {
        if (IsKeyPressed(KEY_ONE)) { return 0; }
        if (IsKeyPressed(KEY_TWO)) { return 1; }
    }
    return -1;
}


int main() {
    const int window_width = 1080;
    const int window_height = 720;
    const float scale = 2.0f;
    const int image_width = window_width / scale;
    const int image_height = window_height / scale;

    SetTraceLogLevel(LOG_NONE);
    InitWindow(window_width, window_height, "Raytracing with c++");
    SetTargetFPS(30);

    int current_scene_index = 0;
    rt::Scene test_scenes[] = {
        create_test_scene_1(),
        create_test_scene_2(),
    };
    int num_of_test_scenes = sizeof(test_scenes) / sizeof(rt::Scene);

    rt::Scene test_scene = test_scenes[current_scene_index];

    glm::vec3 camera_position = {0, 0, 6};
    glm::vec3 camera_direction = {0, 0, -1};
    rt::Camera camera(45.0f, {image_width, image_height}, camera_position, camera_direction);

    rt::Renderer renderer({image_width, image_height}, test_scene, camera);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        renderer.update();
        renderer.reload();
        renderer.render(scale);

        if (camera.update(GetFrameTime())) {
            renderer.reset_frame_index();
        }

        int new_scene_index = check_if_scene_changed();
        if (new_scene_index != -1 && new_scene_index < num_of_test_scenes && current_scene_index != new_scene_index) {
            current_scene_index = new_scene_index;
            test_scene = test_scenes[current_scene_index];
            renderer.reset_frame_index();
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_D)) {
            // debug info
            DrawText("Debug stuff", 10, window_height-150, 20, RED);
            DrawText(TextFormat("  Window size : %d x %d", window_width, window_height), 10, window_height-120, 17, RED);
            DrawText(TextFormat("  Renderer size : %d x %d", image_width, image_height), 10, window_height-100, 17, RED);
            DrawText(TextFormat("  Scaling factor : %f", scale), 10, window_height-80, 17, RED);
            DrawText(TextFormat("  Number of objects : %d", test_scene.objects.size()), 10, window_height-60, 17, RED);
            DrawText(TextFormat("  Number of materials : %d", test_scene.materials.size()), 10, window_height-40, 17, RED);
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
}

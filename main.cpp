
#include "src/renderer.h"
#include "src/geometries/all_geometries.h"
#include "src/materials/all_materials.h"
#include "src/model.h"
#ifndef RT_NO_RAYLIB
#include <raylib.h>
#endif


rt::Scene create_test_scene_1() {
    rt::Scene scene;
    
    // creating materials
    auto mat_1 = std::make_shared<rt::Lambertian>(glm::vec3(1.0, 0.0, 1.0));
    auto mat_2 = std::make_shared<rt::Lambertian>(glm::vec3(0.2, 0.3, 1.0));
    auto mat_3 = std::make_shared<rt::Lambertian>(glm::vec3(0.8, 0.5, 0.2));
    auto mat_4 = std::make_shared<rt::Metal>(glm::vec3(0.8, 0.8, 0.8), 0.1f);
    
    // creating objects
    {
        glm::vec3 position = {0.0, 0.0, 0.0};
        float radius = 1.0;
        scene.objects.emplace_back(new rt::Sphere(position, radius, mat_1));
    }
    {
        glm::vec3 position = {0.0, -101.0, 0.0};
        float radius = 100.0;
        scene.objects.emplace_back(new rt::Sphere(position, radius, mat_2));
    }
    {
        glm::vec3 position = {2.0, 0.0, 0.0};
        float radius = 1.0;
        scene.objects.emplace_back(new rt::Sphere(position, radius, mat_3));
    }
    {
        glm::vec3 v0 = {-1.0, -1, -1};
        glm::vec3 v1 = {-1.0,  1, -1};
        glm::vec3 v2 = {-1.5,  0,  2};
        scene.objects.emplace_back(new rt::Triangle(v0, v1, v2, mat_4));
    }
    
    scene.sky_color = {230, 230, 250};
    scene.sky_color /= 255.0f;
    return scene;
}


rt::Scene create_test_scene_2() {
    rt::Scene scene;

    // creating materials
    auto mat_1 = std::make_shared<rt::Lambertian>(glm::vec3(0.2, 0.6, 1.0));
    auto mat_2 = std::make_shared<rt::Lambertian>(glm::vec3(0.2, 0.6, 0.0));
    auto mat_3 = std::make_shared<rt::Metal>(glm::vec3(0.7, 0.7, 0.7), 0.05f);
    
    // creating objects
    {
        glm::vec3 position = {0.0, 0.0, 0.0};
        float radius = 1.0;
        scene.objects.emplace_back(new rt::Sphere(position, radius, mat_1));
    }
    {
        glm::vec3 position = {0.0, -101.0, 0.0};
        float radius = 100.0;
        scene.objects.emplace_back(new rt::Sphere(position, radius, mat_2));
    }
    {
		glm::vec3 v0 = {-1,  0,   1};
		glm::vec3 v1 = {-1,  0,  -1};
		glm::vec3 v2 = { 1, 0.5, -1};
		glm::vec3 v3 = { 1, 0.5,  1};

        rt::Model* model = (rt::Model*) scene.objects.emplace_back(new rt::Model(
            {
                rt::Triangle(v0, v1, v2, mat_3),
                rt::Triangle(v2, v3, v0, mat_3),
            }
        )).get();

        model->apply_transform({0, -1, 0}, {0, 0, 0}, {2, 1, 2});
    }
    
    scene.sky_color = {104, 184, 235};
    scene.sky_color /= 255.0f;
    return scene;
}


#ifndef RT_NO_RAYLIB
int check_if_scene_changed() {
    if (IsKeyDown(KEY_C)) {
        if (IsKeyPressed(KEY_ONE))   { return 0; }
        if (IsKeyPressed(KEY_TWO))   { return 1; }
    }
    return -1;
}
#endif


int main() {
    const int window_width = 1280;
    const int window_height = 720;

    const float scale = 2.0f;
    const int image_width = window_width / scale;
    const int image_height = window_height / scale;

#ifndef RT_NO_RAYLIB
    SetTraceLogLevel(LOG_NONE);
    InitWindow(window_width, window_height, "Raytracing with c++");
    SetTargetFPS(30);

#endif
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

#ifdef RT_NO_RAYLIB
    renderer.update(300);
    renderer.save_to_file("output.png");
#endif

#ifndef RT_NO_RAYLIB
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
            DrawText("Debug stuff", 10, window_height-170, 20, RED);
            DrawText(TextFormat("  Frame index: %d", renderer.get_frame_index()), 10, window_height-140, 17, RED);
            DrawText(TextFormat("  Window size : %d x %d", window_width, window_height), 10, window_height-120, 17, RED);
            DrawText(TextFormat("  Renderer size : %d x %d", image_width, image_height), 10, window_height-100, 17, RED);
            DrawText(TextFormat("  Scaling factor : %f", scale), 10, window_height-80, 17, RED);
            DrawText(TextFormat("  Number of objects : %d", test_scene.objects.size()), 10, window_height-60, 17, RED);
        }

        DrawText(TextFormat("Frame time: %f", GetFrameTime()), 10, 10, 17, GREEN);
        DrawText(TextFormat("FPS: %f", 1/GetFrameTime()), 10, 30, 17, GREEN);
        EndDrawing();
    }

    CloseWindow();
#endif

}

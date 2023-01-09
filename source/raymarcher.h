
#include "raylib.h"
#include <rose/sokol-shdc.h>
#include "shader.h"

Camera camera;

Shader shader;
raytracer_params_t raytracer_params = {};
int raytracing_ps_params_loc  = -1;
int raytracing_ps_params_vec4count = -1;
float runTime = 0.0f;

void raymarcher_init() {
    auto desc = raytracer_shader_desc(SG_BACKEND_GLCORE33);
    shader = LoadShaderFromMemory(0, desc->fs.source);
    for (int blocki = 0;; ++blocki) {
        auto &block = desc->fs.uniform_blocks[blocki];
        if (block.size == 0) break;

        for (int uniformi = 0;; ++uniformi) {
            auto &uniform = block.uniforms[uniformi];
            if (uniform.type == SG_UNIFORMTYPE_NONE) break;
            //assert(rose::hash(uniform.name) == rose::hash("raytracer_params"));
            raytracing_ps_params_loc = GetShaderLocation(shader, uniform.name);
            raytracing_ps_params_vec4count = uniform.array_count;
        }
    }

}

void raymarcher_update() {
        UpdateCamera(&camera);

        float deltaTime = GetFrameTime();
        runTime += deltaTime;

        raytracer_params_t params;
        params.resolution.x = (float)GetScreenWidth();
        params.resolution.y = (float)GetScreenHeight();
        params.viewEye = camera.position;
        params.viewCenter = camera.target;
        params.runTime = runTime;
        
        SetShaderValueV(shader, raytracing_ps_params_loc, &params, SHADER_UNIFORM_VEC4, raytracing_ps_params_vec4count);
}

void raymarcher_draw() {
    
    
        // Draw
        //----------------------------------------------------------------------------------
        //BeginDrawing();

            //ClearBackground(BLUE);

            // We only draw a white full-screen rectangle,
            // frame is generated in shader using raymarching
            BeginShaderMode(shader);
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
            EndShaderMode();

            //DrawText("(c) Raymarching shader by Iñigo Quilez. MIT License.", GetScreenWidth() - 280, GetScreenHeight() - 20, 10, BLACK);

        //EndDrawing();
        //----------------------------------------------------------------------------------
}

void raymarcher_destroy() {


    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(shader);           // Unload shader
    //--------------------------------------------------------------------------------------

}

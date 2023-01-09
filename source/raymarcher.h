
#include "raylib.h"
#include <rose/sokol-shdc.h>
#include "shader.h"

Shader shader;

int viewEyeLoc;
int viewCenterLoc;
int runTimeLoc;
int resolutionLoc;
float runTime = 0.0f;
Camera camera;

raytracer_params_t raytracer_params = {};
int raytracing_ps_params_loc  = -1;
int raytracing_ps_params_vec4count = -1;

void raymarcher_init() {
       // Initialization
    //--------------------------------------------------------------------------------------

    /*
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - raymarching shapes");

    Camera camera = { 0 };
    camera.position = (Vector3){ 2.5f, 2.5f, 3.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.7f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 65.0f;                                // Camera field-of-view Y

    SetCameraMode(camera, CAMERA_FREE);                 // Set camera mode
    */

    // Load raymarching shader
    // NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader

    /*
    shader = LoadShader(0, TextFormat("resources/shaders/glsl%i/raymarching.fs", GLSL_VERSION));
    // Get shader locations for required uniforms
    viewEyeLoc = GetShaderLocation(shader, "viewEye");
    viewCenterLoc = GetShaderLocation(shader, "viewCenter");
    runTimeLoc = GetShaderLocation(shader, "runTime");
    resolutionLoc = GetShaderLocation(shader, "resolution");

    float resolution[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
    SetShaderValue(shader, resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
    */


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
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);

        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        float cameraTarget[3] = { camera.target.x, camera.target.y, camera.target.z };

        float deltaTime = GetFrameTime();
        runTime += deltaTime;

        raytracer_params_t params;
        params.resolution[0] = (float)GetScreenWidth();
        params.resolution[1] = (float)GetScreenHeight();
        params.runTime = runTime;

        params.viewEye[0] = cameraPos[0];
        params.viewEye[1] = cameraPos[1];
        params.viewEye[2] = cameraPos[2];
        
        params.viewCenter[0] = cameraTarget[0];
        params.viewCenter[1] = cameraTarget[1];
        params.viewCenter[2] = cameraTarget[2];
        
        SetShaderValueV(shader, raytracing_ps_params_loc, &params, SHADER_UNIFORM_VEC4, raytracing_ps_params_vec4count);


        // Set shader required uniform values
        /*
        SetShaderValue(shader, viewEyeLoc, cameraPos, SHADER_UNIFORM_VEC3);
        SetShaderValue(shader, viewCenterLoc, cameraTarget, SHADER_UNIFORM_VEC3);
        SetShaderValue(shader, runTimeLoc, &runTime, SHADER_UNIFORM_FLOAT);

        // Check if screen is resized
        if (IsWindowResized())
        {
            float resolution[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
            SetShaderValue(shader, resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
        }
        */
        //----------------------------------------------------------------------------------


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

#include <raylib.h>
#include <rlgl.h>
#include <rose.h>
#include <rose/event.h>
#include <rose/hash.h>
#include <roseio.h>
#include <imgui.h>
#include <components/components.h>
#include <components/padevents.h>
#include <components/roseoptions.h>
#include <components/workspace.h>

#define IMPL_SERIALIZER
#include <components/components_ser.h>
#include <serializer/imguiserializer.h>
#include <algorithm>

#include "world.h"
#include "world.serializer.h"

#define CUTE_C2_IMPLEMENTATION
#include "cute/cute_c2.h"

World world;
Camera3D camera = { 0 };
float ballSpeed = .3f;
float ballRadius = .5f;
Texture2D cubeTexture;

WorldRecording worldrecording;

static Vector3 operator*(Vector3 lhs, float rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
}

static Vector3 operator+(Vector3 lhs, Vector3 rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

static Vector3 operator-(Vector3 lhs, Vector3 rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

static c2v operator+(c2v lhs, c2v rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

static c2v operator-(c2v lhs, c2v rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

static Vector3 & operator+=(Vector3 & lhs, Vector3 rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

static c2v toc2v(Vector3 v) {
    c2v v2;
    v2.x = v.x;
    v2.y = v.y;
    return v2;
}

bool equalish(Vector3 lhs, Vector3 rhs) {
    float dx = lhs.x - rhs.x;
    float dy = lhs.y - rhs.y;
    float dz = lhs.z - rhs.z;
    if(dx < 0) dx = -dx;
    if(dy < 0) dy = -dy;
    if(dz < 0) dz = -dz;

    return dx < 0.001f && dy < 0.001f && dz < 0.001f;
}

void processPadEvent(const PadEvent & pad) ;

ROSE_EXPORT void postload() {
    // Define the camera to look into our 3d world
    camera.position = { 0.0f, 10.0f, 35.5f }; // Camera position
    camera.target = { 0.0f, 10.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type
    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    // Create a small texture in memory
    Image image = GenImageGradientRadial(64, 64, .22f, WHITE, {200,200,200,255});
    
    // Load the texture from image data
    cubeTexture = LoadTextureFromImage(image);

    SetTextureFilter(cubeTexture, TEXTURE_FILTER_POINT);

    // Unload the image data to free memory
    UnloadImage(image);

    rose::io::json::read(world, rose::io::Folder::Working, "game_state.json");
}

ROSE_EXPORT void predestroy() {
    rose::io::json::write(world, rose::io::Folder::Working, "game_state.json");
}

void collisionCheck() {
    c2Circle pBall;
    pBall.p = toc2v(world.ballPosition);
    pBall.r = ballRadius;

    for(auto & stone : world.stones) {
        c2v p = toc2v(stone.position);

        c2AABB pStone;
        pStone.max = p + toc2v( stone.size * .5 );
        pStone.min = p - toc2v( stone.size * .5 );

        if(c2CircletoAABB(pBall, pStone)) {
            stone.state = StoneState::Dead;

            float dx = fabsf(p.x - pBall.p.x);
            float dy = fabsf(p.y - pBall.p.y);

            if(dy > dx)
                world.ballVelocity.y *= -1;
            else
                world.ballVelocity.x *= -1;
            break;
        }
    }

    world.stones.erase(std::remove_if(world.stones.begin(), world.stones.end(), [](auto & stone) {
        return stone.state == StoneState::Dead;
    }), world.stones.end());
}

void reset_ball() {
    world.ballPosition = world.cubePosition + Vector3 {0.0f, 1.0f, 0.0f};
    float vx = rose::nextf(world.random) * 2 - 1;
    world.ballVelocity = Vector3 {vx, 1.0, 0};
}

void update() {
    if(world.state == WorldState::Paused) return;

    collisionCheck();

    world.cubePosition.x += world.currentStick.x;
    world.ballPosition += world.ballVelocity * ballSpeed;

    if(world.cubePosition.x < -12) world.cubePosition.x = -12;
    if(world.cubePosition.x > 12)  world.cubePosition.x =  12;

    if(world.ballPosition.x < -13) world.ballVelocity.x *= -1;
    if(world.ballPosition.x >  13) world.ballVelocity.x *= -1;
    if(world.ballPosition.y >  23) world.ballVelocity.y *= -1;
    
    if(world.ballPosition.y < 0) {
        world.points -= 1;

        reset_ball();
    }
    else if(world.ballPosition.y < 1 && world.ballVelocity.y < 0) {
        if(world.ballPosition.x > world.cubePosition.x - 2 && world.ballPosition.x < world.cubePosition.x + 2) {
            world.ballVelocity.y *= -1;
            Vector3 vec_dif = world.ballPosition - world.cubePosition;
            world.ballVelocity.x += vec_dif.x * .25f;
        }
    }
}

void DrawCubeWiresOutline(Vector3 position, float width, float height, float length, Color color) {
    DrawCubeTexture(cubeTexture, position, width, height, length, color);
}

ROSE_EXPORT void draw() {
    auto add_new_stone = +[]() {
        float x = -10;
        float y = 17;
        float xyi = 2;
        int i = 0;

        int rows_max = 5;
        int row = 0;

        Color colors[] = {RED,BLUE,YELLOW, ORANGE,GREEN};

        Stone stone { {x, y, 0} };
        for(;;) {
            if(i < world.stones.size()) {
                if(equalish(world.stones[i].position, stone.position)) {
                    i++;
                    stone.position.x += xyi;
                    if(stone.position.x > 10) {
                        stone.position.x = -10;
                        stone.position.y -= xyi;
                        row++;
                        if(row == rows_max) break;
                        stone.color = colors[row];
                    }
                    continue;
                }
            }
            break;
        }

        if(row != rows_max) {
            world.stones.push_back(stone);
        }
        std::sort(world.stones.begin(), world.stones.end(), [](auto & lhs, auto & rhs) {
            if(lhs.position.y == rhs.position.y) {
                return lhs.position.x < rhs.position.x;
            }
            return lhs.position.y > rhs.position.y;
        });
    };

    auto do_replay = [&]() {
        auto end = std::find_if_not(
            worldrecording.padFrames.begin(), 
            worldrecording.padFrames.end(), 
            [&](const PadEventFrameTuple & tuple) { return tuple.frame <= worldrecording.replayFrame; }
        );

        static int skipFirst = 0;
        int i = 0;
        for(auto it = worldrecording.padFrames.begin(); it != end; ++it, ++i) {
            if(i < skipFirst) continue;
            PadEventFrameTuple & tuple = *it;
            processPadEvent(tuple.padEvent);
        }

        skipFirst = i;

        if(worldrecording.replayFrame == worldrecording.totalFrames) {
            worldrecording.state = RecordingState::ReplayingStop;
        }
    };

    switch (worldrecording.state)
    {
    case RecordingState::Inactive:
        break;

    case RecordingState::RecordingStart:
        worldrecording.startworld = world;
        worldrecording.replayFrame = 0;
        worldrecording.padFrames.clear();
        worldrecording.state = RecordingState::Recording;
        [[fallthrough]];
    case RecordingState::Recording:
        break;
    
    case RecordingState::RecordingStop:
        worldrecording.totalFrames = worldrecording.replayFrame;
        rose::io::json::write(worldrecording, rose::io::Folder::Working, "recording.json");
        worldrecording.state = RecordingState::Inactive;
        break;
    
    case RecordingState::ReplayingStart:
        {
            bool found = rose::io::json::read(worldrecording, rose::io::Folder::Working, "recording.json");
            if(found) {
                world = worldrecording.startworld;
                worldrecording.replayFrame = 0;
                worldrecording.state = RecordingState::Replaying;
            }
            else {
                worldrecording.state = RecordingState::Inactive;
            }
        }
        [[fallthrough]];
    case RecordingState::Replaying:
        do_replay();
        break;
    case RecordingState::ReplayingSeek:
        {
            int goalFrame = worldrecording.replayFrame;
            worldrecording.replayFrame = 0;
            world = worldrecording.startworld;
            while(worldrecording.replayFrame < goalFrame) {
                do_replay();
                update();
                worldrecording.replayFrame++;
            }
            worldrecording.state = RecordingState::Replaying;        
        }
        break;
    case RecordingState::ReplayingStop:
        worldrecording.state = RecordingState::Inactive;
        break;

    default:
        break;
    }

    update();
    if(worldrecording.state == RecordingState::Recording || worldrecording.state == RecordingState::Replaying) {
        worldrecording.replayFrame++;
    }

    static float new_build = 7;
    ImGui::LabelText("Build Time", "%s %s", __TIME__, new_build > 0 ? "NEW BUILD!" : "");
    new_build -= .1f;

    if(ImGui::Button("New Stone")) {
        add_new_stone();
    }

    if(ImGui::Button("Clear Stone")) {
        world.stones.clear();
    }

    if(ImGui::Button("Quicksafe")) {
        rose::io::json::write(world, rose::io::Folder::Working, "quicksafe.json");
    }

    if(ImGui::Button("Quickload")) {
        rose::io::json::read(world, rose::io::Folder::Working, "Quicksafe.json");
    }

    if(ImGui::Button("New Game")) {
        world.points = 0;
        world.random = rose::hash_from_clock();
        world.state = WorldState::Paused;
        reset_ball();
        world.stones.clear();
        for(int i = 0; i != 5*11; ++i) {
            add_new_stone();
        }
    }
    ImGui::DragFloat("Cam Distance", &camera.position.z, .1f, 1, 100);


    ImGui::Separator();
    if(ImGui::TreeNode("Recording")) {
        ImGui::BeginDisabled(worldrecording.state != RecordingState::Inactive);
        if(ImGui::Button("Start Recording")) {
            worldrecording.state = RecordingState::RecordingStart;
        }
        ImGui::EndDisabled();

        ImGui::BeginDisabled(worldrecording.state != RecordingState::Recording);
        if(ImGui::Button("Stop Recording")) {
            worldrecording.state = RecordingState::RecordingStop;
        }
        ImGui::EndDisabled();


        ImGui::BeginDisabled(worldrecording.state != RecordingState::Inactive);
        if(ImGui::Button("Start Replaying")) {
            worldrecording.state = RecordingState::ReplayingStart;
        }
        ImGui::EndDisabled();

        ImGui::BeginDisabled(worldrecording.state != RecordingState::Replaying);
        if(ImGui::Button("Stop Replaying")) {
            worldrecording.state = RecordingState::ReplayingStop;
        }
        ImGui::EndDisabled();
        
        if(worldrecording.state == RecordingState::Replaying) {
            float progress = (float)worldrecording.replayFrame / (float)worldrecording.totalFrames;
            
            char buffer[64];
            std::sprintf(buffer, "%d/%d", worldrecording.replayFrame, worldrecording.totalFrames);
            ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buffer);

            int frame = worldrecording.replayFrame;
            if(ImGui::SliderInt("Progress", &frame, 0, worldrecording.totalFrames, buffer)) {
                worldrecording.state = RecordingState::ReplayingSeek;
                worldrecording.replayFrame = frame;
            }
        }

        ImGui::TreePop();
    }

    ImGui::Separator();

    ImguiSerializer serializer;
    rose::ecs::serialize(world, serializer);

    BeginDrawing();
    {
        camera.position = { 0.0f, 10.0f, 35.5f }; // Camera position
        camera.target = { 0.0f, 10.0f, 0.0f };      // Camera looking at point
        camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
        ImGui::DragFloat("Cam fov ", &camera.fovy);
        camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type
        SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

        BeginMode3D(camera);
        {
            //Paddle
            world.cubePosition.z = 2;
            DrawCubeWiresOutline(world.cubePosition, 3.0f, 1.0f, 1.0f, YELLOW);

            //Ball
            world.ballPosition.z = 2;
            DrawSphere(world.ballPosition, ballRadius, PURPLE);

            //Stones
            for(auto & stone : world.stones) {
                DrawCubeWiresOutline(stone.position, stone.size.x, stone.size.y, stone.size.z, stone.color);
            }
            
            //Borders
            DrawCubeWiresOutline({-14.0f,   .0f, .0f,}, 1.0f, 100.0f, 1.0f, GRAY);
            DrawCubeWiresOutline({ 14.0f,   .0f, .0f,}, 1.0f, 100.0f, 1.0f, GRAY);
            DrawCubeWiresOutline({   .0f, 24.0f, .0f,}, 29.0f, 1.0f, 1.0f, GRAY);

            DrawGrid(10, 1.0f);
        }
        EndMode3D();

        DrawRectangle( 10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines( 10, 10, 320, 133, BLUE);

        DrawText("Free camera default controls:", 20, 20, 10, BLACK);
        DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
        DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
        DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
        DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, DARKGRAY);
        DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);
    }
    EndDrawing();
}

void processPadEvent(const PadEvent & pad) {
    PadEventButton changed_button = pad.buttons ^ world.previous_pad_event.buttons;

    if((pad.buttons & PadEventButton::OptionRight) && (pad.buttons & changed_button)) {
        switch (world.state) {
            case WorldState::Paused: world.state = WorldState::Running; break;
            case WorldState::Running: world.state = WorldState::Paused; break;
            default: break;
        }
    }

    world.currentStick.x = pad.stick_lx;
    world.currentStick.y = pad.stick_ly;
    world.previous_pad_event = pad;
}

ROSE_EXPORT void event(const rose::Event & ev) {
    if(worldrecording.state == RecordingState::Inactive || worldrecording.state == RecordingState::Recording) {
        if(auto pad = ev.get<PadEvent>()) {
            if(worldrecording.state == RecordingState::Recording) {
                PadEventFrameTuple tuple;
                tuple.frame = worldrecording.replayFrame;
                tuple.padEvent = *pad;
                worldrecording.padFrames.push_back(tuple);
            }
            processPadEvent(*pad);
        }
    }
}

ROSE_EXPORT void ui() {
}

#include <rose/engine.hpp>
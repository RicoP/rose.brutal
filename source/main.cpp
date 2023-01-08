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

#include "raymarcher.h"

World world;
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

static Vector3 & operator+=(Vector3 & lhs, Vector3 rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

void processPadEvent(const PadEvent & pad) ;

ROSE_EXPORT void postload() {
    camera.position = Vector3{ 2.5f, 2.5f, 3.0f };    // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.7f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 65.0f;                                // Camera field-of-view Y
    SetCameraMode(camera, CAMERA_ORBITAL); // Set a free camera mode

    rose::io::json::read(world, rose::io::Folder::Working, "game_state.json");

    raymarcher_init();
}

ROSE_EXPORT void predestroy() {
    rose::io::json::write(world, rose::io::Folder::Working, "game_state.json");

    raymarcher_destroy();
}

void update() {    
    //if(world.state == WorldState::Paused) return;
    raymarcher_update();
}

ROSE_EXPORT void draw() {
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

    if(ImGui::Button("Quicksafe")) {
        rose::io::json::write(world, rose::io::Folder::Working, "quicksafe.json");
    }

    if(ImGui::Button("Quickload")) {
        rose::io::json::read(world, rose::io::Folder::Working, "Quicksafe.json");
    }

    if(ImGui::Button("New Game")) {
        world = World();
        world.state = WorldState::Paused;
    }

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
    if(world.quarter_steps) {
        static int frame = 0;
        if((frame % 4) == 0) raymarcher_draw();
        frame++;
        frame %= 4;
    }
    else {
        raymarcher_draw();
    }
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

#include <rose/engine.hpp>
#pragma once

#include <raylib.h>
#include <vector>
#include <rose/hash.h>
#include <rose.h>

// Import Raylib structs

//@Imposter Color is declared in raylib.h
/*
struct Color {
     char r;
     char g;
     char b;
     char a;
};
*/

//@Imposter vector3 is declared in raylib.h
/*
struct Vector3 {
	float x,y,z;
};
*/

namespace rose {
  namespace ecs {
    void        serialize(Color &o, ISerializer &s) {
        unsigned long long l = 0;
        l ^= o.a;
        l <<= 8;
        l ^= o.b;
        l <<= 8;
        l ^= o.g;
        l <<= 8;
        l ^= o.r;
        
        ::rose::ecs::serialize(l, s);
    }
    void      deserialize(Color &o, IDeserializer &s) {
        unsigned long long l = 0;
        ::rose::ecs::deserialize(l, s);

        o.r = (unsigned char)(l & 0xFF);
        l >>= 8;
        o.g = (unsigned char)(l & 0xFF);
        l >>= 8;
        o.b = (unsigned char)(l & 0xFF);
        l >>= 8;
        o.a = (unsigned char)(l & 0xFF);
    }
  }
}

namespace rose {
  namespace ecs {
    void        serialize(Vector3 &o, ISerializer &s) {
        float v[3] = {o.x, o.y, o.z};
        ::rose::ecs::serialize(v, s);
    }
    void      deserialize(Vector3 &o, IDeserializer &s) {
        float v[3] = {0,0,0};
        ::rose::ecs::deserialize(v, s);
        o.x = v[0];
        o.y = v[1];
        o.z = v[2];
    }
  }
}

// World state stuff

enum class StoneState : int {
  Alive = 0,
  Dead
};

enum class WorldState : int {
    NewGame,
    Running,
    Paused,
};

struct Stone {
    Vector3 position = { 0,0,0 };
    Vector3 size = { 1,1,1 };
    Color color = RED;
    StoneState state = StoneState::Alive;
};

struct World {
  Vector3 cubePosition = { 0,0,0 };
  Vector3 ballPosition = { 0,1,0 };
  Vector3 ballVelocity = { 1,1,0 };
  Vector3 currentStick = { 0,0,0 };

  rose::hash_value random = 0;
  int points = 0;

  PadEvent previous_pad_event;
  WorldState state = WorldState::NewGame;
  std::vector<Stone> stones;
};

struct PadEventFrameTuple {
  PadEvent padEvent;
  int frame = -1;
};

enum class RecordingState : int {
    Inactive = 0,
    RecordingStart,
    Recording,
    RecordingStop,

    ReplayingStart,
    Replaying,
    ReplayingSeek,
    ReplayingStop,
};

struct WorldRecording {
  World startworld;
  int replayFrame = 0;
  int totalFrames = 0;
  std::vector<PadEventFrameTuple> padFrames;
  RecordingState state = RecordingState::Inactive;
};

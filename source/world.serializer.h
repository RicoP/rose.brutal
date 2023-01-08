#pragma once

#include <rose/hash.h>
#include <rose/typetraits.h>
#include <rose/serializer.h>

///////////////////////////////////////////////////////////////////
//  AUTOGEN                                                      //
//  command:
//    rose.parser -I world.h -O world.serializer.h
///////////////////////////////////////////////////////////////////

enum class                   StoneState : int ;
const char * to_string(const StoneState &);
namespace rose {
  namespace ecs {
    void      deserialize(StoneState &o, IDeserializer &s);
    void        serialize(StoneState &o, ISerializer &s);
  }
  template<>
  struct type_id<StoneState> {
    inline static hash_value VALUE = 11910806599994158533ULL;
  };
  hash_value         hash(const StoneState &o);
  void construct_defaults(      StoneState &o); //implement me
}


enum class                   WorldState : int ;
const char * to_string(const WorldState &);
namespace rose {
  namespace ecs {
    void      deserialize(WorldState &o, IDeserializer &s);
    void        serialize(WorldState &o, ISerializer &s);
  }
  template<>
  struct type_id<WorldState> {
    inline static hash_value VALUE = 3483512573815606629ULL;
  };
  hash_value         hash(const WorldState &o);
  void construct_defaults(      WorldState &o); //implement me
}


enum class                   RecordingState : int ;
const char * to_string(const RecordingState &);
namespace rose {
  namespace ecs {
    void      deserialize(RecordingState &o, IDeserializer &s);
    void        serialize(RecordingState &o, ISerializer &s);
  }
  template<>
  struct type_id<RecordingState> {
    inline static hash_value VALUE = 12895677666688842227ULL;
  };
  hash_value         hash(const RecordingState &o);
  void construct_defaults(      RecordingState &o); //implement me
}


namespace rose {
  namespace ecs {
  }
  hash_value         hash(const Color &o);
  template<>
  struct type_id<Color> {
    inline static hash_value VALUE = 11375897551430744876ULL;
  };
  void construct_defaults(      Color &o); // implement me
}
bool operator==(const Color &lhs, const Color &rhs);
bool operator!=(const Color &lhs, const Color &rhs);


namespace rose {
  namespace ecs {
  }
  hash_value         hash(const Vector3 &o);
  template<>
  struct type_id<Vector3> {
    inline static hash_value VALUE = 865855757241434760ULL;
  };
  void construct_defaults(      Vector3 &o); // implement me
}
bool operator==(const Vector3 &lhs, const Vector3 &rhs);
bool operator!=(const Vector3 &lhs, const Vector3 &rhs);


struct                World;
namespace rose {
  namespace ecs {
    void        serialize(World &o, ISerializer &s);
    void      deserialize(World &o, IDeserializer &s);
  }
  hash_value         hash(const World &o);
  template<>
  struct type_id<World> {
    inline static hash_value VALUE = 2068493676604329404ULL;
  };
  void construct_defaults(      World &o); // implement me
}
bool operator==(const World &lhs, const World &rhs);
bool operator!=(const World &lhs, const World &rhs);


struct                PadEventFrameTuple;
namespace rose {
  namespace ecs {
    void        serialize(PadEventFrameTuple &o, ISerializer &s);
    void      deserialize(PadEventFrameTuple &o, IDeserializer &s);
  }
  hash_value         hash(const PadEventFrameTuple &o);
  template<>
  struct type_id<PadEventFrameTuple> {
    inline static hash_value VALUE = 7765748126551423679ULL;
  };
  void construct_defaults(      PadEventFrameTuple &o); // implement me
}
bool operator==(const PadEventFrameTuple &lhs, const PadEventFrameTuple &rhs);
bool operator!=(const PadEventFrameTuple &lhs, const PadEventFrameTuple &rhs);


struct                WorldRecording;
namespace rose {
  namespace ecs {
    void        serialize(WorldRecording &o, ISerializer &s);
    void      deserialize(WorldRecording &o, IDeserializer &s);
  }
  hash_value         hash(const WorldRecording &o);
  template<>
  struct type_id<WorldRecording> {
    inline static hash_value VALUE = 15755819351817888067ULL;
  };
  void construct_defaults(      WorldRecording &o); // implement me
}
bool operator==(const WorldRecording &lhs, const WorldRecording &rhs);
bool operator!=(const WorldRecording &lhs, const WorldRecording &rhs);


#ifdef IMPL_SERIALIZER

    #ifndef IMPL_SERIALIZER_UTIL
    #define IMPL_SERIALIZER_UTIL
    #include <cstring>

    namespace {
    //internal helper methods
    template<class T>
    bool rose_parser_equals(const T& lhs, const T& rhs) {
      return lhs == rhs;
    }

    template<class T, size_t N>
    bool rose_parser_equals(const T(&lhs)[N], const T(&rhs)[N]) {
      for (size_t i = 0; i != N; ++i) {
        if (lhs[i] != rhs[i]) return false;
      }
      return true;
    }

    template<size_t N>
    bool rose_parser_equals(const char(&lhs)[N], const char(&rhs)[N]) {
      for (size_t i = 0; i != N; ++i) {
        if (lhs[i] != rhs[i]) return false;
        if (lhs[i] == 0) return true;
      }
      return true;
    }

    template<class T>
    bool rose_parser_equals(const std::vector<T> &lhs, const std::vector<T> &rhs) {
      if (lhs.size() != rhs.size()) return false;
      for (size_t i = 0; i != lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) return false;
      }
      return true;
    }

    template<class TL, class TR>
    void assign(TL& lhs, TR&& rhs) {
      lhs = rhs;
    }

    template<class T>
    void construct_default(std::vector<T> & v) {
      c.clear();
    }
    }
    #endif
  
const char * to_string(const StoneState & e) {
    switch(e) {
        case StoneState::Alive: return "Alive";
        case StoneState::Dead: return "Dead";
        default: return "<UNKNOWN>";
    }
}
void rose::ecs::serialize(StoneState& o, ISerializer& s) {
  switch (o) {
    case StoneState::Alive: {
      char str[] = "Alive";
      serialize(str, s);
      break;
    }
    case StoneState::Dead: {
      char str[] = "Dead";
      serialize(str, s);
      break;
    }
    default: /* unknown */ break;
  }
}
void rose::ecs::deserialize(StoneState& o, IDeserializer& s) {
  char str[64];
  deserialize(str, s);
  rose::hash_value h = rose::hash(str);
  switch (h) {
  case rose::hash("Alive"): o = StoneState::Alive; break;
  case rose::hash("Dead"): o = StoneState::Dead; break;
  default: /*unknown value*/ break;
  }
}
rose::hash_value       rose::hash(const StoneState& o) {
  return static_cast<rose::hash_value>(o);
}

const char * to_string(const WorldState & e) {
    switch(e) {
        case WorldState::Running: return "Running";
        case WorldState::Paused: return "Paused";
        default: return "<UNKNOWN>";
    }
}
void rose::ecs::serialize(WorldState& o, ISerializer& s) {
  switch (o) {
    case WorldState::Running: {
      char str[] = "Running";
      serialize(str, s);
      break;
    }
    case WorldState::Paused: {
      char str[] = "Paused";
      serialize(str, s);
      break;
    }
    default: /* unknown */ break;
  }
}
void rose::ecs::deserialize(WorldState& o, IDeserializer& s) {
  char str[64];
  deserialize(str, s);
  rose::hash_value h = rose::hash(str);
  switch (h) {
  case rose::hash("Running"): o = WorldState::Running; break;
  case rose::hash("Paused"): o = WorldState::Paused; break;
  default: /*unknown value*/ break;
  }
}
rose::hash_value       rose::hash(const WorldState& o) {
  return static_cast<rose::hash_value>(o);
}

const char * to_string(const RecordingState & e) {
    switch(e) {
        case RecordingState::Inactive: return "Inactive";
        case RecordingState::RecordingStart: return "RecordingStart";
        case RecordingState::Recording: return "Recording";
        case RecordingState::RecordingStop: return "RecordingStop";
        case RecordingState::ReplayingStart: return "ReplayingStart";
        case RecordingState::Replaying: return "Replaying";
        case RecordingState::ReplayingSeek: return "ReplayingSeek";
        case RecordingState::ReplayingStop: return "ReplayingStop";
        default: return "<UNKNOWN>";
    }
}
void rose::ecs::serialize(RecordingState& o, ISerializer& s) {
  switch (o) {
    case RecordingState::Inactive: {
      char str[] = "Inactive";
      serialize(str, s);
      break;
    }
    case RecordingState::RecordingStart: {
      char str[] = "RecordingStart";
      serialize(str, s);
      break;
    }
    case RecordingState::Recording: {
      char str[] = "Recording";
      serialize(str, s);
      break;
    }
    case RecordingState::RecordingStop: {
      char str[] = "RecordingStop";
      serialize(str, s);
      break;
    }
    case RecordingState::ReplayingStart: {
      char str[] = "ReplayingStart";
      serialize(str, s);
      break;
    }
    case RecordingState::Replaying: {
      char str[] = "Replaying";
      serialize(str, s);
      break;
    }
    case RecordingState::ReplayingSeek: {
      char str[] = "ReplayingSeek";
      serialize(str, s);
      break;
    }
    case RecordingState::ReplayingStop: {
      char str[] = "ReplayingStop";
      serialize(str, s);
      break;
    }
    default: /* unknown */ break;
  }
}
void rose::ecs::deserialize(RecordingState& o, IDeserializer& s) {
  char str[64];
  deserialize(str, s);
  rose::hash_value h = rose::hash(str);
  switch (h) {
  case rose::hash("Inactive"): o = RecordingState::Inactive; break;
  case rose::hash("RecordingStart"): o = RecordingState::RecordingStart; break;
  case rose::hash("Recording"): o = RecordingState::Recording; break;
  case rose::hash("RecordingStop"): o = RecordingState::RecordingStop; break;
  case rose::hash("ReplayingStart"): o = RecordingState::ReplayingStart; break;
  case rose::hash("Replaying"): o = RecordingState::Replaying; break;
  case rose::hash("ReplayingSeek"): o = RecordingState::ReplayingSeek; break;
  case rose::hash("ReplayingStop"): o = RecordingState::ReplayingStop; break;
  default: /*unknown value*/ break;
  }
}
rose::hash_value       rose::hash(const RecordingState& o) {
  return static_cast<rose::hash_value>(o);
}

///////////////////////////////////////////////////////////////////
//  struct Color
///////////////////////////////////////////////////////////////////
bool operator==(const Color &lhs, const Color &rhs) {
  return
    rose_parser_equals(lhs.r, rhs.r) &&
    rose_parser_equals(lhs.g, rhs.g) &&
    rose_parser_equals(lhs.b, rhs.b) &&
    rose_parser_equals(lhs.a, rhs.a) ;
}

bool operator!=(const Color &lhs, const Color &rhs) {
  return
    !rose_parser_equals(lhs.r, rhs.r) ||
    !rose_parser_equals(lhs.g, rhs.g) ||
    !rose_parser_equals(lhs.b, rhs.b) ||
    !rose_parser_equals(lhs.a, rhs.a) ;
}

rose::hash_value rose::hash(const Color &o) {
  rose::hash_value h = rose::hash(o.r);
  h = rose::xor64(h);
  h ^= rose::hash(o.g);
  h = rose::xor64(h);
  h ^= rose::hash(o.b);
  h = rose::xor64(h);
  h ^= rose::hash(o.a);
  return h;
}
///////////////////////////////////////////////////////////////////
//  struct Vector3
///////////////////////////////////////////////////////////////////
bool operator==(const Vector3 &lhs, const Vector3 &rhs) {
  return
    rose_parser_equals(lhs.x, rhs.x) &&
    rose_parser_equals(lhs.y, rhs.y) &&
    rose_parser_equals(lhs.z, rhs.z) ;
}

bool operator!=(const Vector3 &lhs, const Vector3 &rhs) {
  return
    !rose_parser_equals(lhs.x, rhs.x) ||
    !rose_parser_equals(lhs.y, rhs.y) ||
    !rose_parser_equals(lhs.z, rhs.z) ;
}

rose::hash_value rose::hash(const Vector3 &o) {
  rose::hash_value h = rose::hash(o.x);
  h = rose::xor64(h);
  h ^= rose::hash(o.y);
  h = rose::xor64(h);
  h ^= rose::hash(o.z);
  return h;
}
///////////////////////////////////////////////////////////////////
//  struct World
///////////////////////////////////////////////////////////////////
bool operator==(const World &lhs, const World &rhs) {
  return
    rose_parser_equals(lhs.quarter_steps, rhs.quarter_steps) &&
    rose_parser_equals(lhs.state, rhs.state) &&
    rose_parser_equals(lhs.previous_pad_event, rhs.previous_pad_event) &&
    rose_parser_equals(lhs.currentStick, rhs.currentStick) ;
}

bool operator!=(const World &lhs, const World &rhs) {
  return
    !rose_parser_equals(lhs.quarter_steps, rhs.quarter_steps) ||
    !rose_parser_equals(lhs.state, rhs.state) ||
    !rose_parser_equals(lhs.previous_pad_event, rhs.previous_pad_event) ||
    !rose_parser_equals(lhs.currentStick, rhs.currentStick) ;
}

void rose::ecs::serialize(World &o, ISerializer &s) {
  if(s.node_begin("World", rose::hash("World"), &o)) {
    s.key("quarter_steps");
    serialize(o.quarter_steps, s);
    s.key("state");
    serialize(o.state, s);
    s.key("previous_pad_event");
    serialize(o.previous_pad_event, s);
    s.key("currentStick");
    serialize(o.currentStick, s);
    s.node_end();
  }
  s.end();
}

void rose::ecs::deserialize(World &o, IDeserializer &s) {
  //implement me
  //construct_defaults(o);

  while (s.next_key()) {
    switch (s.hash_key()) {
      case rose::hash("quarter_steps"):
        deserialize(o.quarter_steps, s);
        break;
      case rose::hash("state"):
        deserialize(o.state, s);
        break;
      case rose::hash("previous_pad_event"):
        deserialize(o.previous_pad_event, s);
        break;
      case rose::hash("currentStick"):
        deserialize(o.currentStick, s);
        break;
      default: s.skip_key(); break;
    }
  }
}

rose::hash_value rose::hash(const World &o) {
  rose::hash_value h = rose::hash(o.quarter_steps);
  h = rose::xor64(h);
  h ^= rose::hash(o.state);
  h = rose::xor64(h);
  h ^= rose::hash(o.previous_pad_event);
  h = rose::xor64(h);
  h ^= rose::hash(o.currentStick);
  return h;
}
///////////////////////////////////////////////////////////////////
//  struct PadEventFrameTuple
///////////////////////////////////////////////////////////////////
bool operator==(const PadEventFrameTuple &lhs, const PadEventFrameTuple &rhs) {
  return
    rose_parser_equals(lhs.padEvent, rhs.padEvent) &&
    rose_parser_equals(lhs.frame, rhs.frame) ;
}

bool operator!=(const PadEventFrameTuple &lhs, const PadEventFrameTuple &rhs) {
  return
    !rose_parser_equals(lhs.padEvent, rhs.padEvent) ||
    !rose_parser_equals(lhs.frame, rhs.frame) ;
}

void rose::ecs::serialize(PadEventFrameTuple &o, ISerializer &s) {
  if(s.node_begin("PadEventFrameTuple", rose::hash("PadEventFrameTuple"), &o)) {
    s.key("padEvent");
    serialize(o.padEvent, s);
    s.key("frame");
    serialize(o.frame, s);
    s.node_end();
  }
  s.end();
}

void rose::ecs::deserialize(PadEventFrameTuple &o, IDeserializer &s) {
  //implement me
  //construct_defaults(o);

  while (s.next_key()) {
    switch (s.hash_key()) {
      case rose::hash("padEvent"):
        deserialize(o.padEvent, s);
        break;
      case rose::hash("frame"):
        deserialize(o.frame, s);
        break;
      default: s.skip_key(); break;
    }
  }
}

rose::hash_value rose::hash(const PadEventFrameTuple &o) {
  rose::hash_value h = rose::hash(o.padEvent);
  h = rose::xor64(h);
  h ^= rose::hash(o.frame);
  return h;
}
///////////////////////////////////////////////////////////////////
//  struct WorldRecording
///////////////////////////////////////////////////////////////////
bool operator==(const WorldRecording &lhs, const WorldRecording &rhs) {
  return
    rose_parser_equals(lhs.startworld, rhs.startworld) &&
    rose_parser_equals(lhs.replayFrame, rhs.replayFrame) &&
    rose_parser_equals(lhs.totalFrames, rhs.totalFrames) &&
    rose_parser_equals(lhs.padFrames, rhs.padFrames) &&
    rose_parser_equals(lhs.state, rhs.state) ;
}

bool operator!=(const WorldRecording &lhs, const WorldRecording &rhs) {
  return
    !rose_parser_equals(lhs.startworld, rhs.startworld) ||
    !rose_parser_equals(lhs.replayFrame, rhs.replayFrame) ||
    !rose_parser_equals(lhs.totalFrames, rhs.totalFrames) ||
    !rose_parser_equals(lhs.padFrames, rhs.padFrames) ||
    !rose_parser_equals(lhs.state, rhs.state) ;
}

void rose::ecs::serialize(WorldRecording &o, ISerializer &s) {
  if(s.node_begin("WorldRecording", rose::hash("WorldRecording"), &o)) {
    s.key("startworld");
    serialize(o.startworld, s);
    s.key("replayFrame");
    serialize(o.replayFrame, s);
    s.key("totalFrames");
    serialize(o.totalFrames, s);
    s.key("padFrames");
    serialize(o.padFrames, s);
    s.key("state");
    serialize(o.state, s);
    s.node_end();
  }
  s.end();
}

void rose::ecs::deserialize(WorldRecording &o, IDeserializer &s) {
  //implement me
  //construct_defaults(o);

  while (s.next_key()) {
    switch (s.hash_key()) {
      case rose::hash("startworld"):
        deserialize(o.startworld, s);
        break;
      case rose::hash("replayFrame"):
        deserialize(o.replayFrame, s);
        break;
      case rose::hash("totalFrames"):
        deserialize(o.totalFrames, s);
        break;
      case rose::hash("padFrames"):
        deserialize(o.padFrames, s);
        break;
      case rose::hash("state"):
        deserialize(o.state, s);
        break;
      default: s.skip_key(); break;
    }
  }
}

rose::hash_value rose::hash(const WorldRecording &o) {
  rose::hash_value h = rose::hash(o.startworld);
  h = rose::xor64(h);
  h ^= rose::hash(o.replayFrame);
  h = rose::xor64(h);
  h ^= rose::hash(o.totalFrames);
  h = rose::xor64(h);
  h ^= rose::hash(o.padFrames);
  h = rose::xor64(h);
  h ^= rose::hash(o.state);
  return h;
}

#endif

#pragma once

// EWColors — Color palette for EwolFX plugins
// Must be included AFTER IGraphics.h is available (via EWOL.h)

#include "IGraphics.h"

namespace EWColors {

// ── Base palette (unchanging across all plugins) ──────────────────────────

struct BG {
  static inline const iplug::igraphics::IColor Main = [] {
    return iplug::igraphics::IColor(255, 17, 17, 17);
  }(); // #111111
  static inline const iplug::igraphics::IColor Panel = [] {
    return iplug::igraphics::IColor(255, 22, 22, 22);
  }(); // #161616
  static inline const iplug::igraphics::IColor Border = [] {
    return iplug::igraphics::IColor(255, 42, 42, 42);
  }(); // #2a2a2a
  static inline const iplug::igraphics::IColor Divider = [] {
    return iplug::igraphics::IColor(255, 50, 50, 50);
  }(); // #323232
};

struct Knob {
  static inline const iplug::igraphics::IColor Body = [] {
    return iplug::igraphics::IColor(255, 30, 30, 30);
  }(); // #1e1e1e
  static inline const iplug::igraphics::IColor Track = [] {
    return iplug::igraphics::IColor(255, 42, 42, 42);
  }(); // #2a2a2a
  static inline const iplug::igraphics::IColor Highlight = [] {
    return iplug::igraphics::IColor(255, 55, 55, 55);
  }(); // #373737
};

struct Text {
  static inline const iplug::igraphics::IColor Label = [] {
    return iplug::igraphics::IColor(255, 136, 136, 136);
  }(); // #888888
  static inline const iplug::igraphics::IColor Value = [] {
    return iplug::igraphics::IColor(255, 224, 224, 224);
  }(); // #e0e0e0
  static inline const iplug::igraphics::IColor Brand = [] {
    return iplug::igraphics::IColor(255, 170, 170, 170);
  }(); // #aaaaaa
  static inline const iplug::igraphics::IColor Footer = [] {
    return iplug::igraphics::IColor(255, 85, 85, 85);
  }(); // #555555
  static inline const iplug::igraphics::IColor Copyright = [] {
    return iplug::igraphics::IColor(255, 68, 68, 68);
  }(); // #444444
};

struct Led {
  static inline const iplug::igraphics::IColor Off = [] {
    return iplug::igraphics::IColor(255, 42, 42, 42);
  }(); // #2a2a2a
};

struct Button {
  static inline const iplug::igraphics::IColor Fill = [] {
    return iplug::igraphics::IColor(255, 30, 30, 30);
  }(); // #1e1e1e
  static inline const iplug::igraphics::IColor BorderOff = [] {
    return iplug::igraphics::IColor(255, 50, 50, 50);
  }(); // #323232
  static inline const iplug::igraphics::IColor FillActive = [] {
    return iplug::igraphics::IColor(255, 40, 40, 40);
  }(); // #282828
};

// ── Per-plugin accent colors (selected at compile time) ───────────────────

struct Accent {
  static inline const iplug::igraphics::IColor Amber = [] {
    return iplug::igraphics::IColor(255, 232, 168, 50);
  }(); // Delay / Echo
  static inline const iplug::igraphics::IColor Blue = [] {
    return iplug::igraphics::IColor(255, 74, 144, 217);
  }(); // Reverb
  static inline const iplug::igraphics::IColor Red = [] {
    return iplug::igraphics::IColor(255, 217, 74, 74);
  }(); // Distortion / Drive
  static inline const iplug::igraphics::IColor Purple = [] {
    return iplug::igraphics::IColor(255, 155, 89, 182);
  }(); // Modulation
  static inline const iplug::igraphics::IColor Teal = [] {
    return iplug::igraphics::IColor(255, 26, 188, 156);
  }(); // Compression
  static inline const iplug::igraphics::IColor Orange = [] {
    return iplug::igraphics::IColor(255, 230, 126, 34);
  }(); // Filter / EQ
  static inline const iplug::igraphics::IColor Green = [] {
    return iplug::igraphics::IColor(255, 46, 204, 113);
  }(); // Generic / Utility
};

} // namespace EWColors
#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "IPlug_include_in_plug_src.h"

// ============================================================================
// EwolFX Color Palette — Centralized for easy theming
// All plugins share the base palette; each plugin selects ONE accent color.
// ============================================================================

namespace EWColors {

// ── Base palette (unchanging across all plugins) ──────────────────────────

struct BG {
  static constexpr IColor Main = IColor(17, 17, 17, 255);    // #111111
  static constexpr IColor Panel = IColor(22, 22, 22, 255);   // #161616
  static constexpr IColor Border = IColor(42, 42, 42, 255);  // #2a2a2a
  static constexpr IColor Divider = IColor(50, 50, 50, 255); // #323232
};

struct Knob {
  static constexpr IColor Body = IColor(30, 30, 30, 255);      // #1e1e1e
  static constexpr IColor Track = IColor(42, 42, 42, 255);     // #2a2a2a
  static constexpr IColor Highlight = IColor(55, 55, 55, 255); // #373737
};

struct Text {
  static constexpr IColor Label = IColor(136, 136, 136, 255);  // #888888
  static constexpr IColor Value = IColor(224, 224, 224, 255);  // #e0e0e0
  static constexpr IColor Brand = IColor(170, 170, 170, 255);  // #aaaaaa
  static constexpr IColor Footer = IColor(85, 85, 85, 255);    // #555555
  static constexpr IColor Copyright = IColor(68, 68, 68, 255); // #444444
};

struct Led {
  static constexpr IColor Off = IColor(42, 42, 42, 255); // #2a2a2a
};

struct Button {
  static constexpr IColor Fill = IColor(30, 30, 30, 255);       // #1e1e1e
  static constexpr IColor BorderOff = IColor(50, 50, 50, 255);  // #323232
  static constexpr IColor FillActive = IColor(40, 40, 40, 255); // #282828
};

// ── Per-plugin accent colors (selected at compile time) ───────────────────

struct Accent {
  static constexpr IColor Amber = IColor(232, 168, 50, 255); // Delay / Echo
  static constexpr IColor Blue = IColor(74, 144, 217, 255);  // Reverb
  static constexpr IColor Red = IColor(217, 74, 74, 255); // Distortion / Drive
  static constexpr IColor Purple = IColor(155, 89, 182, 255); // Modulation
  static constexpr IColor Teal = IColor(26, 188, 156, 255);   // Compression
  static constexpr IColor Orange = IColor(230, 126, 34, 255); // Filter / EQ
  static constexpr IColor Green =
      IColor(46, 204, 113, 255); // Generic / Utility
};

} // namespace EWColors
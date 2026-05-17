#pragma once

// EWOL (EwolFX Object Wrapper Library) — Base plugin class + accent color enum
//
// Include order in your plugin header:
//   1. EWColors.h (for IColor definitions)
//   2. EWOL.h (for Accent enum, EWPlugin base class — includes IGraphics +
//      IPlug_include_in_plug_hdr)
//
// Include order in your plugin .cpp:
//   1. IPlug_include_in_plug_src.h (for full IGraphics definitions)
//   2. EWControls.cpp (if using pre-built controls)

#include "IGraphics.h"
#include "IPlug_include_in_plug_hdr.h"

// Bring iplug types into global scope so derived plugins can use unqualified
// names like Plugin, sample, InstanceInfo, Config etc.  Also bring iplug::
// igraphics types into scope for IGraphics, IRECT, IText etc.  This matches
// the pattern used by MyPermissivePlugin which compiles and builds correctly.
using namespace iplug;
using namespace iplug::igraphics;

// ============================================================================
// EwolFX Base Plugin Class + Accent Color Enum
// New plugins inherit from EWPlugin. Define params, accent color, and DSP.
// ============================================================================

namespace EWOL {

// Accent color selector — override GetAccent() in your plugin
enum class Accent {
  Amber,  // Delay / Echo
  Blue,   // Reverb
  Red,    // Distortion / Drive
  Purple, // Modulation
  Teal,   // Compression
  Orange, // Filter / EQ
  Green   // Generic / Utility
};

// Version string — override in your plugin if needed
inline const char *GetVersionString() { return "1.0.0"; }

} // namespace EWOL
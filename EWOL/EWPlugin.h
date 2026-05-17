#pragma once

// EWPlugin — Base class for all EwolFX plugins using iPlug2
// Provides the interface contract and common utilities.
// Derived classes handle their own UI setup in SetupLayout() and
// ProcessBlock().

#include "EWOL.h"

namespace EWPluginInternal {

inline iplug::igraphics::IColor GetAccentColor(EWOL::Accent accent) {
  switch (accent) {
  case EWOL::Accent::Amber:
    return iplug::igraphics::IColor(232, 168, 50, 255);
  case EWOL::Accent::Blue:
    return iplug::igraphics::IColor(74, 144, 217, 255);
  case EWOL::Accent::Red:
    return iplug::igraphics::IColor(217, 74, 74, 255);
  case EWOL::Accent::Purple:
    return iplug::igraphics::IColor(155, 89, 182, 255);
  case EWOL::Accent::Teal:
    return iplug::igraphics::IColor(26, 188, 156, 255);
  case EWOL::Accent::Orange:
    return iplug::igraphics::IColor(230, 126, 34, 255);
  case EWOL::Accent::Green:
    return iplug::igraphics::IColor(46, 204, 113, 255);
  default:
    return iplug::igraphics::IColor(46, 204, 113, 255);
  }
}

} // namespace EWPluginInternal

// ============================================================================
// EWPlugin — Base class for all EwolFX plugins
// ============================================================================

class EWPlugin : public iplug::Plugin {
public:
  // Override these in your derived plugin class
  virtual const char *GetPluginName() const = 0;
  virtual EWOL::Accent GetAccent() const = 0;

  // Number of knobs (auto-creates UI for all params up to this count)
  virtual int GetNumKnobs() const = 0;

  // Called after graphics is initialized — attach extra controls here
  virtual void SetupLayout(IGraphics *pGraphics) {}

protected:
  EWPlugin(const iplug::InstanceInfo &info, iplug::Config config)
      : iplug::Plugin(info, std::move(config)) {}

  EWOL::Accent mAccent = EWOL::Accent::Green;
};
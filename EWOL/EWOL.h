#pragma once

#include "EWColors.h"
#include "EWControls.h"
#include "EWLayout.h"
#include "IPlug_include_in_plug_hdr.h"

// ============================================================================
// EwolFX Base Plugin Class
// New plugins inherit from this. Define params, accent color, and DSP.
// ============================================================================

namespace EWOL {

// Accent color selector — override GetAccentColor() in your plugin
enum class Accent {
  Amber,  // Delay / Echo
  Blue,   // Reverb
  Red,    // Distortion / Drive
  Purple, // Modulation
  Teal,   // Compression
  Orange, // Filter / EQ
  Green   // Generic / Utility
};

// Get IColor from enum
inline IColor GetAccentColor(Accent accent) {
  switch (accent) {
  case Accent::Amber:
    return EWColors::Accent::Amber;
  case Accent::Blue:
    return EWColors::Accent::Blue;
  case Accent::Red:
    return EWColors::Accent::Red;
  case Accent::Purple:
    return EWColors::Accent::Purple;
  case Accent::Teal:
    return EWColors::Accent::Teal;
  case Accent::Orange:
    return EWColors::Accent::Orange;
  case Accent::Green:
    return EWColors::Accent::Green;
  }
  return EWColors::Accent::Amber;
}

// Version string — override in your plugin if needed
inline const char *GetVersionString() { return "1.0.0"; }

} // namespace EWOL

using namespace EWOL;

// ============================================================================
// EWPlugin — Base class for all EwolFX plugins
// ============================================================================

class EWPlugin : public iplug::Plugin {
public:
  // Override these in your derived plugin class
  virtual const char *GetPluginName() const = 0;
  virtual Accent GetAccent() const = 0;

  // Number of knobs (auto-creates UI for all params up to this count)
  virtual int GetNumKnobs() const = 0;

  // Called after graphics is initialized — attach extra controls here
  virtual void SetupLayout(IGraphics *pGraphics) {}

  // Get accent color (used by controls that need it at draw time)
  IColor GetAccentColor() const { return mAccent; }

#if IPLUG_DSP
  void ProcessBlock(sample **inputs, sample **outputs, int nFrames) override {
    // Default: pass-through. Override in derived class.
    const int numChannels = GetNumInputChannels();
    const int numOutChannels = GetNumOutputChannels();
    const int minCh =
        (numChannels < numOutChannels) ? numChannels : numOutChannels;

    for (int c = 0; c < minCh; c++)
      memcpy(outputs[c], inputs[c], nFrames * sizeof(sample));

    // Zero extra output channels
    for (int c = minCh; c < numOutChannels; c++)
      memset(outputs[c], 0, nFrames * sizeof(sample));
  }
#endif

protected:
  EWPlugin(const InstanceInfo &info, Config config)
      : Plugin(info, std::move(config)) {
    // Accent is set in OnModule() after graphics init
  }

private:
  IColor mAccent;

protected:
  void OnModule() override {
    // Set accent color now that we know which plugin this is
    mAccent = GetAccentColor(GetAccent());

    // Attach background panel
    mGraphics->AttachBGControl(new EWPanel(mGraphics->GetBounds()));

    // Calculate available area (between header and footer)
    const auto bounds = mGraphics->GetBounds();
    const auto available =
        bounds.GetHShifted(32).GetVShifted(40).GetVShifted(-28);

    const int numKnobs = GetNumKnobs();
    if (numKnobs > 0) {
      // Compute grid positions for all knobs
      IVector<IRECT> knobBounds;
      EWLayout::ComputeGrid(available, numKnobs, 0, 48, 24, knobBounds);

      // Attach knobs + labels
      for (int i = 0; i < numKnobs && i < GetNumParams(); i++) {
        IRECT knobRect = knobBounds[i];
        IRECT labelRect = EWLayout::GetLabelBounds(knobRect, 36);

        // Create knob (uses accent from mAccent set above)
        auto *knob = new EWKnob(knobRect, GetParam(i));
        // Store accent color reference for this knob
        knob->SetTag(i); // We'll use tag to pass accent color info

        mGraphics->AttachControl(knob, i);

        // Create label below knob
        WDL_String paramName;
        GetParam(i)->GetName(paramName);
        auto *label = new EWLabel(labelRect, paramName.Get(), GetParam(i));
        mGraphics->AttachControl(label);
      }
    }

    // Attach bypass LED (bottom-right)
    auto ledRect = EWLayout::GetLedBounds(EWLayout::GetFooterBounds(bounds));
    mGraphics->AttachControl(new EWLed(ledRect, true));

    // Attach plugin name in footer
    WDL_String nameStr;
    GetPluginName().Copy(nameStr);
    auto nameRect = bounds.GetBOTTOMHEIGHT(28).GetLEFTWIDTH(200);
    mGraphics->AttachControl(new EWNameLabel(nameRect, nameStr.Get()));

    // Call derived class setup (for extra controls like sliders, buttons)
    SetupLayout(mGraphics);

    // Update version text in header
    WDL_String verStr;
    GetVersionString().Copy(verStr);
  }

public:
  ~EWPlugin() override = default;
};
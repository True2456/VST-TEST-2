#pragma once

// EWControls — Custom control classes for EwolFX plugins
// This wraps standard IPlug2 controls with EWOL's color palette.
// It is designed to be easily used by smaller LLMs.

#include "IGraphics.h"
#include "IControls.h"
#include "EWColors.h"

namespace EWControls {

using namespace iplug::igraphics;

// Helper to map EWOL::Accent enum to IColor
inline IColor GetColorForAccent(EWOL::Accent accent) {
  switch (accent) {
    case EWOL::Accent::Amber: return EWColors::Accent::Amber;
    case EWOL::Accent::Blue: return EWColors::Accent::Blue;
    case EWOL::Accent::Red: return EWColors::Accent::Red;
    case EWOL::Accent::Purple: return EWColors::Accent::Purple;
    case EWOL::Accent::Teal: return EWColors::Accent::Teal;
    case EWOL::Accent::Orange: return EWColors::Accent::Orange;
    case EWOL::Accent::Green: return EWColors::Accent::Green;
    default: return EWColors::Accent::Amber;
  }
}

// Generate the standard IVStyle using the EWColors palette
inline IVStyle GetEwolStyle(EWOL::Accent accentEnum) {
  IColor accentColor = GetColorForAccent(accentEnum);
  
  return IVStyle()
    .WithShowLabel(true)
    .WithShowValue(true)
    .WithDrawFrame(true)
    .WithColor(EVColor::kBG, EWColors::BG::Panel)
    .WithColor(EVColor::kFG, EWColors::BG::Main)
    .WithColor(EVColor::kPR, accentColor)
    .WithColor(EVColor::kFR, EWColors::Knob::Track)
    .WithColor(EVColor::kHL, EWColors::Knob::Highlight)
    .WithColor(EVColor::kSH, EWColors::BG::Border)
    .WithColor(EVColor::kX1, EWColors::Text::Value)
    .WithColor(EVColor::kX2, EWColors::Text::Label)
    .WithColor(EVColor::kX3, EWColors::Text::Brand)
    .WithValueText(IText(12, EWColors::Text::Value, "Roboto-Regular", EAlign::Center, EVAlign::Bottom, 0.f, EWColors::Text::Label))
    .WithLabelText(IText(12, EWColors::Text::Label, "Roboto-Regular", EAlign::Center, EVAlign::Top, 0.f, EWColors::Text::Value));
}

// ── EWKnob — Wraps IVKnobControl with EWOL styling ─────────────────────────

class EWKnob : public IVKnobControl {
public:
  EWKnob(const IRECT& bounds, int paramIdx, const char* label = "", EWOL::Accent accent = EWOL::Accent::Amber)
      : IVKnobControl(bounds, paramIdx, label, GetEwolStyle(accent), true) {}
};

// ── EWLabel — Wraps ITextControl for static labels ─────────────────────────

class EWLabel : public ITextControl {
public:
  EWLabel(const IRECT& bounds, const char* str, IColor color = EWColors::Text::Label)
      : ITextControl(bounds, str, IText(14, color, "Roboto-Regular", EAlign::Center, EVAlign::Middle)) {}
};

// ── EWPanel — Draws the background ──────────────────────────────────────────

class EWPanel : public IPanelControl {
public:
  EWPanel(const IRECT& bounds)
      : IPanelControl(bounds, EWColors::BG::Main) {}
};

} // namespace EWControls
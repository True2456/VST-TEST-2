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

// ── EWKnob — Fully Custom High-End Knob ─────────────────────────

class EWKnob : public IKnobControlBase {
public:
  EWKnob(const IRECT& bounds, int paramIdx, const char* label = "", EWOL::Accent accent = EWOL::Accent::Amber)
      : IKnobControlBase(bounds, paramIdx), mLabel(label), mAccent(accent) {
      mColor = GetColorForAccent(accent);
  }

  void Draw(IGraphics& g) override {
    // 1. Calculate geometry (perfect circle regardless of IRECT)
    const float radius = (mRECT.W() < mRECT.H() ? mRECT.W() : mRECT.H()) / 2.0f - 8.0f;
    const float cx = mRECT.MW();
    const float cy = mRECT.MH();
    
    const float startAngle = -135.f;
    const float endAngle = 135.f;
    const float trackThickness = 4.0f;
    
    // 2. Draw outer track (background)
    g.DrawArc(EWColors::Knob::Track, cx, cy, radius, startAngle, endAngle, 0, trackThickness);
    
    // 3. Draw active track (value)
    float valueAngle = startAngle + (static_cast<float>(GetValue()) * (endAngle - startAngle));
    
    // Only draw active track if > startAngle
    if (valueAngle > startAngle) {
      g.DrawArc(mColor, cx, cy, radius, startAngle, valueAngle, 0, trackThickness);
    }
    
    // 4. Draw Knob Body
    const float bodyRadius = radius - 8.0f;
    g.FillCircle(EWColors::Knob::Body, cx, cy, bodyRadius);
    
    // 5. Draw Pointer Dot
    const float pointerRadius = bodyRadius - 5.0f;
    // Note: IGraphics arc angles are 0=top, clockwise.
    // Mathematical sin/cos for 0=top:
    // x = cx + r * sin(angle), y = cy - r * cos(angle)
    const float angleRad = valueAngle * (M_PI / 180.f);
    const float px = cx + pointerRadius * std::sin(angleRad);
    const float py = cy - pointerRadius * std::cos(angleRad);
    
    g.FillCircle(mColor, px, py, 3.5f); // Bright pointer dot
    
    // Draw subtle highlight on hover
    if (mMouseIsOver) {
        g.DrawCircle(EWColors::Knob::Highlight, cx, cy, bodyRadius, 0, 1.5f);
    }
  }

private:
  const char* mLabel;
  EWOL::Accent mAccent;
  IColor mColor;
};

// ── EWLabel — Wraps ITextControl for static labels ─────────────────────────

class EWLabel : public ITextControl {
public:
  EWLabel(const IRECT& bounds, const char* str, IColor color = EWColors::Text::Label)
      : ITextControl(bounds, str, IText(13, color, "Roboto-Regular", EAlign::Center, EVAlign::Middle)) {}
};

// ── EWTitle — Wraps ITextControl for large plugin headers ─────────────────────────

class EWTitle : public ITextControl {
public:
  EWTitle(const IRECT& bounds, const char* str, IColor color = EWColors::Text::Value)
      : ITextControl(bounds, str, IText(24, color, "Roboto-Regular", EAlign::Center, EVAlign::Middle)) {}
};

// ── EWPanel — Draws a premium background ──────────────────────────────────────────

class EWPanel : public IControl {
public:
  EWPanel(const IRECT& bounds) : IControl(bounds) {
    mIgnoreMouse = true;
  }
  
  void Draw(IGraphics& g) override {
    // Fill main dark background
    g.FillRect(EWColors::BG::Main, mRECT);
    
    // Draw a subtle "glassmorphic" module container
    IRECT moduleBox = mRECT.GetPadded(-20.f).GetVShifted(10.f);
    g.FillRoundRect(EWColors::BG::Panel, moduleBox, 10.f);
    g.DrawRoundRect(EWColors::BG::Border, moduleBox, 10.f, 0, 1.5f);
  }
};

} // namespace EWControls
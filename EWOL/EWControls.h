#pragma once

#include "EWColors.h"
#include "EWLayout.h"
#include "IPlug_include_in_plug_hdr.h"

// ============================================================================
// EwolFX Custom Controls — Minimalist UI components
// All controls use the accent color from their parent EWPlugin.
// ============================================================================

// ── EWKnob — Minimalist rotary knob with accent indicator ──────────────────

class EWKnob : public IControl {
public:
  EWKnob(const IRECT &bounds, const IParam *param)
      : IControl(bounds, param), mValue(0.f) {}

  void OnResize() override { mKnobRect = GetBounds().GetCentredInside(40); }

  void OnDoubleTap(int channel, const IRECT &p) override {
    // Double-tap resets to midpoint
    if (mParam) {
      double mid = (mParam->Min() + mParam->Max()) / 2.;
      mParam->Set(mid, channel);
    }
  }

  void Draw(IGraphics *pGraphics) override {
    float normalized = mParam ? mParam->Value() : 0.f;

    // Draw track ring
    IRECT trackRect = mKnobRect.GetInsetFromBorder(4);
    pGraphics->DrawCircle(EWColors::Knob::Track, trackRect, 2.f);

    // Draw accent arc (progress indicator)
    float angle = -M_PI_2 + normalized * M_PI * 2;
    IRECT arcRect = trackRect.GetInsetFromBorder(1);
    pGraphics->Fill(arcRect, EWColors::Accent::Amber, 2.f, angle,
                    angle + 0.01f);

    // Draw knob body
    pGraphics->FillEllipse(EWColors::Knob::Body, mKnobRect);

    // Draw highlight edge
    IRECT highlightRect = mKnobRect.GetHShifted(2).GetVShifted(-2);
    pGraphics->FillEllipse(EWColors::Knob::Highlight,
                           highlightRect.GetHShifted(-1).GetVShifted(1));

    // Draw indicator line
    float cx = mKnobRect.M().x;
    float cy = mKnobRect.M().y;
    float radius = mKnobRect.H() / 2.f - 6;
    float ix = cx + cos(angle) * radius;
    float iy = cy + sin(angle) * radius;

    ILine line({cx, cy}, {ix, iy}, 2.f);
    pGraphics->DrawLine(EWColors::Accent::Amber, line);

    // Draw small dot at end of indicator
    pGraphics->FillEllipse(EWColors::Accent::Amber, ix - 2, iy - 2, 4, 4);
  }

  void OnValue(const double *pVal) override { mValue = *pVal; }

private:
  IRECT mKnobRect;
  float mValue;
};

// ── EWLabel — Parameter label + live value display ────────────────────────

class EWLabel : public IControl {
public:
  EWLabel(const IRECT &bounds, const char *label, const IParam *param)
      : IControl(bounds), mLabel(label), mParam(param) {}

  void Draw(IGraphics *pGraphics) override {
    IRECT labelRect = GetBounds().GetTOPHEIGHT(14);
    IRECT valueRect = labelRect.GetVShifted(16).GetHEIGHT(14);

    IText text(EWLayout::Capitalize(mLabel.Get()).Get(), 10,
               IFont()); // TODO: load font once

    text.mColor = EWColors::Text::Label;
    pGraphics->DrawText(text, mLabel.Get(), labelRect);

    IText valueText("", 11, IFont());
    valueText.mColor = EWColors::Text::Value;

    WDL_String valStr;
    if (mParam) {
      double v = mParam->Value();
      const char *unit = mParam->GetLabel();
      if (v >= 1000.)
        valStr.Format("%.0f%s", v, unit);
      else if (v >= 100.)
        valStr.Format("%.1f%s", v, unit);
      else if (v >= 10.)
        valStr.Format("%.2f%s", v, unit);
      else
        valStr.Format("%.3f%s", v, unit);
    }

    text.mAlign = EAlign::Right;
    pGraphics->DrawText(valueText, valStr.Get(), valueRect);
  }

private:
  WDL_String mLabel;
  const IParam *mParam;
};

// ── EWLed — Circular LED indicator ────────────────────────────────────────

class EWLed : public IControl {
public:
  EWLed(const IRECT &bounds, bool active = false)
      : IControl(bounds), mActive(active) {}

  void SetState(bool active) {
    mActive = active;
    Invalidate();
  }

  bool GetState() const { return mActive; }

  void Draw(IGraphics *pGraphics) override {
    IColor color = mActive ? EWColors::Accent::Amber : EWColors::Led::Off;
    pGraphics->FillEllipse(color, GetBounds().GetInsetFromBorder(1));

    if (mActive) {
      // Glow effect: slightly larger, more transparent
      IRECT glow = GetBounds().GetInsetFromBorder(-2);
      pGraphics->FillEllipse(IColor(232, 168, 50, 60), glow);
    }
  }

private:
  bool mActive;
};

// ── EWButton — Flat toggle button with accent border ──────────────────────

class EWButton : public IControl {
public:
  EWButton(const IRECT &bounds, const char *text)
      : IControl(bounds), mText(text), mToggled(false) {}

  void Click(int channel) override {
    mToggled = !mToggled;
    Invalidate();
  }

  void Draw(IGraphics *pGraphics) override {
    IRECT bounds = GetBounds();

    // Button body
    pGraphics->FillEllipse(mToggled ? EWColors::Button::FillActive
                                    : EWColors::Button::Fill,
                           bounds.GetInsetFromBorder(2));

    // Border
    IRECT borderRect = bounds.GetInsetFromBorder(1);
    pGraphics->DrawEllipse(mToggled ? EWColors::Accent::Amber
                                    : EWColors::Button::BorderOff,
                           borderRect, 1.f);

    // Text label
    IText text("", 9, IFont());
    text.mColor = mToggled ? EWColors::Text::Value : EWColors::Text::Label;
    text.mAlign = EAlign::Center;
    pGraphics->DrawText(text, mText.Get(), bounds);
  }

private:
  WDL_String mText;
  bool mToggled;
};

// ── EWSlider — Vertical fader with accent fill ───────────────────────────

class EWSlider : public IControl {
public:
  EWSlider(const IRECT &bounds, const IParam *param)
      : IControl(bounds, param), mValue(0.f) {}

  void OnResize() override {
    mTrackRect = GetBounds().GetCentredHorizontallyWithin(8);
  }

  void Draw(IGraphics *pGraphics) override {
    float normalized = mParam ? mParam->Value() : 0.f;

    // Track background
    pGraphics->FillEllipse(EWColors::Knob::Track, mTrackRect);

    // Fill up to value
    float fillH = normalized * (mTrackRect.H() - 4);
    IRECT fillRect = mTrackRect.GetTOPHEIGHT(fillH + 2);
    pGraphics->FillEllipse(EWColors::Accent::Amber, fillRect);

    // Slider cap
    float capY = mTrackRect.T() + fillH;
    IRECT cap =
        mTrackRect.GetCentredHorizontallyWithin(16).GetVShifted(capY - 2, 4);
    pGraphics->FillEllipse(EWColors::Knob::Body, cap.GetInsetFromBorder(1));
    pGraphics->FillEllipse(EWColors::Knob::Highlight, cap);
  }

  void OnMouseDown(float x, float y, IMouseMod *pMod) override {
    mMouseDown = true;
  }

  void OnMouseUp(float x, float y, IMouseMod *pMod) override {
    mMouseDown = false;
  }

  void OnMouseMove(float x, float y) override {
    if (mMouseDown && mParam) {
      float normalized = 1.f - ((y - mTrackRect.T()) / (float)mTrackRect.H());
      normalized = fmax(0.f, fmin(1.f, normalized));
      mParam->Set(normalized);
    }
  }

  bool IsOver(float x, float y) override {
    return x >= mTrackRect.L() - 10 && x <= mTrackRect.R() + 10 &&
           y >= mTrackRect.T() - 5 && y <= mTrackRect.B() + 5;
  }

private:
  IRECT mTrackRect;
  float mValue;
  bool mMouseDown = false;
};

// ── EWPanel — Background panel with header, footer, dividers ─────────────

class EWPanel : public IBGControl {
public:
  EWPanel(const IRECT &bounds) : IBGControl(bounds) {}

  void Draw(IGraphics *pGraphics) override {
    const IRECT &b = GetBounds();

    // Main background
    pGraphics->FillEllipse(EWColors::BG::Main, b);

    // Header bar
    IRECT header = b.GetTOPHEIGHT(32);
    pGraphics->FillEllipse(EWColors::BG::Panel, header);

    // Header divider line
    ILine headerLine({b.L(), 32}, {b.R(), 32}, 1.f);
    pGraphics->DrawLine(EWColors::BG::Border, headerLine);

    // Footer bar
    IRECT footer = b.GetBOTTOMHEIGHT(28);
    pGraphics->FillEllipse(EWColors::BG::Panel, footer);

    // Footer divider line
    IRECT footerTop = b.GetHShifted(0).GetVShifted(b.H() - 28);
    ILine footerLine({b.L(), b.H() - 28}, {b.R(), b.H() - 28}, 1.f);
    pGraphics->DrawLine(EWColors::BG::Border, footerLine);

    // Side dividers (vertical lines between header and footer)
    ILine leftLine({32, 32}, {32, b.H() - 28}, 1.f);
    ILine rightLine({b.W() - 32, 32}, {b.W() - 32, b.H() - 28}, 1.f);
    pGraphics->DrawLine(EWColors::BG::Border, leftLine);
    pGraphics->DrawLine(EWColors::BG::Border, rightLine);

    // Brand text (top-left)
    IText brandText("", 12, IFont());
    brandText.mColor = EWColors::Text::Brand;
    brandText.mAlign = EAlign::Left;
    pGraphics->DrawText(brandText, "EWOLFX",
                        header.GetVShifted(2).GetLEFTWIDTH(80));

    // Version text (top-right) — overridden by EWPlugin
    IText versionText("", 9, IFont());
    versionText.mColor = EWColors::Text::Footer;
    versionText.mAlign = EAlign::Right;
    pGraphics->DrawText(versionText, "v1.0.0",
                        header.GetVShifted(2).GetRIGHTWIDTH(60));
  }
};

// ── EWNameLabel — Plugin name in footer ───────────────────────────────────

class EWNameLabel : public IControl {
public:
  EWNameLabel(const IRECT &bounds, const char *name)
      : IControl(bounds), mName(name) {}

  void Draw(IGraphics *pGraphics) override {
    IText text("", 10, IFont());
    text.mColor = EWColors::Text::Footer;
    pGraphics->DrawText(text, mName.Get(), GetBounds());
  }

private:
  WDL_String mName;
};
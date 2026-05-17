#include "EwolFX_Gain.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include "../EWOL/EWControls.h"

// ============================================================================
// EwolFX_Gain Implementation — Gain plugin using EWOL template framework
// ============================================================================

EwolFX_Gain::EwolFX_Gain(const InstanceInfo &info)
    : EWPlugin(info, MakeConfig(kNumParams, 1)) {

#if IPLUG_EDITOR
  mMakeGraphicsFunc = [this]() {
    return MakeGraphics(*this, 480, 360, 60, GetScaleForScreen(480, 360));
  };

  mLayoutFunc = [this](IGraphics *pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, true);
    pGraphics->LoadFont("Roboto-Regular", "Roboto-Regular.ttf");

    const IRECT bounds = pGraphics->GetBounds();
    pGraphics->AttachControl(new EWControls::EWPanel(bounds));

    IRECT knobRect = bounds.GetCentredInside(100).GetVShifted(-20);
    pGraphics->AttachControl(new EWControls::EWKnob(knobRect, kGain, "GAIN", GetAccent()));

    SetupLayout(pGraphics);
  };
#endif

  // Single gain parameter: 0% — 100%
  GetParam(kGain)->InitDouble("GAIN", 100.0, 0.0, 100.0, 0.1, "%");
}

void EwolFX_Gain::SetupLayout(IGraphics *pGraphics) {
  // No extra controls needed — the single knob is auto-generated.
  // This method is empty by design for simple plugins.
}

void EwolFX_Gain::ProcessBlock(sample **inputs, sample **outputs, int nFrames) {
  float gain = GetParam(kGain)->Value() / 100.0f; // 0.0 — 1.0

  const int numIn = NInChansConnected();
  const int numOut = NOutChansConnected();
  const int minCh = (numIn < numOut) ? numIn : numOut;

  for (int s = 0; s < nFrames; s++) {
    for (int c = 0; c < minCh; c++) {
      outputs[c][s] = inputs[c][s] * gain;
    }
    for (int c = minCh; c < numOut; c++) {
      outputs[c][s] = 0.0;
    }
  }
}
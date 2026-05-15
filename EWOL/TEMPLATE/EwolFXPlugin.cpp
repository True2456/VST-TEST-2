#include "EwolFXPlugin.h"

// ============================================================================
// EwolFX Plugin Implementation Template
// This file shows exactly what you need to write for each new plugin.
// ============================================================================

EwolFXPlugin::EwolFXPlugin(const InstanceInfo &info)
    : EWPlugin(info, MakeConfig(kNumParams)) {

#if IPLUG_EDITOR
  // ── Editor configuration ───────────────────────────────────────────────

  mMakeGraphicsFunc = []() {
    // Default size: 480x360, scale for screen
    return MakeGraphics(*this, 480, 360, ScreenScale());
  };

  mLayoutFunc = [&](IGraphics *pGraphics) {
    // Enable corner resizing (UI scales with window drag)
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, true);

    // Set font (optional — EWOL uses default if not set)
    // const auto font = pGraphics->GetFont("Roboto-Regular.ttf");
    // pGraphics->SetAllControls(IGraphics::UpdateAction::UpdateProperties);

    // Attach any extra controls beyond the auto-generated knobs
    SetupLayout(pGraphics);
  };
#endif

  // ── Parameter definitions ──────────────────────────────────────────────
  // Each parameter: name, default value, min, max, step, unit label

  GetParam(kParam1)
      ->InitDouble("PARAM_1", 50.0, 0.0, 100.0, 0.1, "%")
      ->SetRange(0.0, 1.0) // Normalized range for UI
      ->MakeGeneric();

  GetParam(kParam2)
      ->InitDouble("PARAM_2", 50.0, 0.0, 100.0, 0.1, "ms")
      ->SetRange(0.0, 1.0)
      ->MakeGeneric();

  GetParam(kParam3)
      ->InitDouble("PARAM_3", 50.0, 0.0, 100.0, 0.1, "%")
      ->SetRange(0.0, 1.0)
      ->MakeGeneric();

  GetParam(kParam4)
      ->InitDouble("PARAM_4", 50.0, 0.0, 100.0, 0.1, "dB")
      ->SetRange(-24.0, 6.0)
      ->MakeLogScale();

  GetParam(kParam5)
      ->InitDouble("PARAM_5", 50.0, 0.0, 100.0, 0.1, "%")
      ->SetRange(0.0, 1.0)
      ->MakeGeneric();
}

// ── Extra layout (override for non-knob controls) ─────────────────────────

void EwolFXPlugin::SetupLayout(IGraphics *pGraphics) {
  // Add extra controls here if needed.
  // Examples:
  //
  //   // Vertical slider for output level
  //   auto sliderRect = pGraphics->GetBounds()
  //                         .GetFromLRHC(30, 80)
  //                         .GetVShifted(40);
  //   pGraphics->AttachControl(new EWSlider(sliderRect, GetParam(kOutput)));
  //
  //   // Bypass button
  //   auto btnRect = pGraphics->GetBounds()
  //                             .GetBOTTOMHEIGHT(24)
  //                             .GetLEFTWIDTH(80);
  //   pGraphics->AttachControl(new EWButton(btnRect, "BYPASS"));

  // The auto-generated knobs + labels + LED are already attached by EWPlugin.
}

// ── DSP processing (REQUIRED — implement your effect here) ────────────────

void EwolFXPlugin::ProcessBlock(sample **inputs, sample **outputs,
                                int nFrames) {
  // ── Read parameter values ──────────────────────────────────────────────

  float p1 = GetParam(kParam1)->Value(); // 0.0 — 1.0
  float p2 = GetParam(kParam2)->Value();
  float p3 = GetParam(kParam3)->Value();
  float p4 = GetParam(kParam4)->Value();
  float p5 = GetParam(kParam5)->Value();

  // ── Your DSP algorithm here ────────────────────────────────────────────
  // Example: simple pass-through with gain (replace with your effect)

  const int numIn = GetNumInputChannels();
  const int numOut = GetNumOutputChannels();
  const int minCh = (numIn < numOut) ? numIn : numOut;

  for (int s = 0; s < nFrames; s++) {
    for (int c = 0; c < minCh; c++) {
      // Replace this with your actual DSP:
      outputs[c][s] = inputs[c][s] * p1;
    }
    // Zero extra output channels
    for (int c = minCh; c < numOut; c++) {
      outputs[c][s] = 0.0;
    }
  }
}
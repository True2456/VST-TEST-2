#include "MyPermissivePlugin.h"

// ============================================================================
// MyPermissivePlugin Implementation — Gain plugin using EWOL template
// ============================================================================

MyPermissivePlugin::MyPermissivePlugin(const InstanceInfo &info)
    : EWPlugin(info, MakeConfig(kNumParams)) {

#if IPLUG_EDITOR
  mMakeGraphicsFunc = []() {
    return MakeGraphics(*this, 480, 360, ScreenScale());
  };

  mLayoutFunc = [&](IGraphics *pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, true);

    // Optional: load custom font
    // const auto font = pGraphics->GetFont("Roboto-Regular.ttf");

    SetupLayout(pGraphics);
  };
#endif

  // Single gain parameter: 0% — 100%
  GetParam(kGain)
      ->InitDouble("GAIN", 100.0, 0.0, 100.0, 0.1, "%")
      ->SetRange(0.0, 1.0)
      ->MakeGeneric();
}

void MyPermissivePlugin::SetupLayout(IGraphics *pGraphics) {
  // No extra controls needed — the single knob is auto-generated.
  // This method is empty by design for simple plugins.
}

void MyPermissivePlugin::ProcessBlock(sample **inputs, sample **outputs,
                                      int nFrames) {
  float gain = GetParam(kGain)->Value(); // 0.0 — 1.0

  const int numIn = GetNumInputChannels();
  const int numOut = GetNumOutputChannels();
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
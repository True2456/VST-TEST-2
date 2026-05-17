#include "EwolFX_ZPlane.h"
#include "../EWOL/EWControls.h"
#include "IControls.h"
#include "IPlug_include_in_plug_src.h"

// ============================================================================
// EwolFX_ZPlane Implementation — Z-Plane Filter using EWOL template framework
// ============================================================================

EwolFX_ZPlane::EwolFX_ZPlane(const InstanceInfo &info)
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

    SetupLayout(pGraphics);
  };
#endif

  // Parameter initialization
  // kCutoff: 20Hz – 20kHz (exponential)
  GetParam(kCutoff)->InitDouble("CUTOFF", 1000.0, 20.0, 20000.0, 0.1, "Hz", 0, "", IParam::ShapeExp());
  // kResonance: 0.0 – 1.0
  GetParam(kResonance)->InitDouble("RES", 0.5, 0.0, 1.0, 0.001, "");
  // kMorph: 0.0 – 1.0
  GetParam(kMorph)->InitDouble("MORPH", 0.0, 0.0, 1.0, 0.001, "");
}

void EwolFX_ZPlane::SetupLayout(IGraphics *pGraphics) {
  const IRECT bounds = pGraphics->GetBounds();

  // Calculate knob dimensions and spacing
  const int numKnobs = 3;
  const float knobSize = 90.0f;
  IRECT knobBaseRect = bounds.GetCentredInside(knobSize * numKnobs + 40, knobSize);

  // Distribute knobs evenly across the lower portion of the panel
  const char *labels[] = {"CUTOFF", "RES", "MORPH"};

  for (int i = 0; i < numKnobs; i++) {
    IRECT kRect = knobBaseRect.GetGridCell(i, 1, numKnobs).GetCentredInside(knobSize, knobSize).GetVShifted(30);

    pGraphics->AttachControl(
        new EWControls::EWKnob(kRect, i, labels[i], GetAccent()));

    // Add a label below each knob
    IRECT labelRect = kRect.GetVShifted(knobSize + 5);
    labelRect.B = labelRect.T + 20.f;
    pGraphics->AttachControl(
        new EWControls::EWLabel(labelRect, labels[i], EWColors::Text::Label));
  }

  // Add a centered title label at the top
  IRECT titleRect = bounds.GetFromTop(60).GetVShifted(15.f);
  pGraphics->AttachControl(new EWControls::EWTitle(
      titleRect, "EWOL Z-PLANE FILTER", EWControls::GetColorForAccent(GetAccent())));
}

void EwolFX_ZPlane::ProcessBlock(sample **inputs, sample **outputs,
                                 int nFrames) {
  const int numIn = NInChansConnected();
  const int numOut = NOutChansConnected();
  const int minCh = (numIn < numOut) ? numIn : numOut;

  if (numIn == 0 || numOut == 0 || nFrames == 0) {
    return;
  }

  double sr = GetSampleRate();
  if (sr <= 0.0) return;

  // Initialize filters if needed (first call or sample rate change)
  if (mSampleRate != sr) {
    mSampleRate = sr;
    mFilterLeft.Reset(sr);
    mFilterRight.Reset(sr);
  }

  // Get current parameter values
  // kCutoff param is now directly in Hz thanks to ShapeExp
  double cutoffFreq = GetParam(kCutoff)->Value();

  double resonance = GetParam(kResonance)->Value();
  double morph = GetParam(kMorph)->Value();

  // Set filter parameters
  mFilterLeft.SetCutoff(cutoffFreq);
  mFilterLeft.SetResonance(resonance);
  mFilterLeft.SetMorph(morph);

  mFilterRight.SetCutoff(cutoffFreq);
  mFilterRight.SetResonance(resonance);
  mFilterRight.SetMorph(morph);

  sample *inL = inputs[0];
  sample *inR = (numIn > 1) ? inputs[1] : inputs[0];
  sample *outL = outputs[0];
  sample *outR = (numOut > 1) ? outputs[1] : outputs[0];

  for (int s = 0; s < nFrames; s++) {
    double L = mFilterLeft.Process((double)inL[s]);
    double R = (numIn > 1) ? mFilterRight.Process((double)inR[s]) : L;
    
    // NaN and Infinity protection
    if (std::isnan(L) || std::isinf(L)) {
      L = 0.0;
      mFilterLeft.Reset(mSampleRate);
    }
    if (std::isnan(R) || std::isinf(R)) {
      R = 0.0;
      mFilterRight.Reset(mSampleRate);
    }
    
    // Hard clamp to prevent Ableton redlining
    if (L > 2.0) L = 2.0;
    if (L < -2.0) L = -2.0;
    if (R > 2.0) R = 2.0;
    if (R < -2.0) R = -2.0;

    if (numOut > 1) {
      outL[s] = (sample)L;
      outR[s] = (sample)R;
    } else {
      outL[s] = (sample)((L + R) * 0.5);
    }
  }

  // Zero out unused output channels
  for (int c = minCh; c < numOut; c++) {
    for (int s = 0; s < nFrames; s++) {
      outputs[c][s] = 0.0;
    }
  }
}
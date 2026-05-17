#pragma once

// ============================================================================
// EwolFX_ZPlane — Z-Plane Filter plugin using EWOL template framework
// ============================================================================

#include "../EWOL/EWPlugin.h"

// ── Plugin metadata ────────────────────────────────────────────────────────

#define EWOLFX_PLUGIN_NAME "EwolFX_ZPlane"
#define EWOLFX_ACCENT EWOL::Accent::Orange // Filter/EQ accent color
#define EWOLFX_NUM_KNOBS 3
#define EWOLFX_VERSION "1.0.0"

// ── Parameter enum ─────────────────────────────────────────────────────────

enum EParams {
  kCutoff = 0, // Cutoff frequency: 20Hz – 20kHz
  kResonance,  // Resonance: 0.0 – 1.0
  kMorph,      // Morph (filter shape interpolation): 0.0 – 1.0
  kNumParams
};

// ── Biquad section for Z-Plane filtering ───────────────────────────────────

struct Biquad {
  // Coefficients (direct form I transposed)
  double b0, b1, b2;
  double a1, a2;

  // Delay line states
  double x1, x2; // previous input samples
  double y1, y2; // previous output samples

  inline void Reset() { x1 = x2 = y1 = y2 = 0.0; }

  inline double Process(double x, const Biquad &src) {
    double y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
    // Shift states
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
    return y;
  }

  inline double Process(const Biquad &src) {
    // Use our own states but copy coefficients from src for interpolation
    double x = 0.0; // placeholder, will be set by caller
    return src.b0 * x + src.b1 * src.x1 + src.b2 * src.x2 - src.a1 * src.y1 -
           src.a2 * src.y2;
  }
};

// ── Z-Plane Filter Engine ──────────────────────────────────────────────────
// Interpolates between different filter shapes by morphing poles/zeros.

class ZPlaneFilter {
public:
  inline void Reset(double sampleRate) {
    mSampleRate = sampleRate;
    mLowpass.Reset();
    mPhaser.Reset();
    mFormant.Reset();
    mBiquad.Reset();
  }

  // Set cutoff frequency in Hz (logarithmic mapping handled by caller)
  inline void SetCutoff(double cutoffHz) { mCutoff = cutoffHz; }

  // Set resonance (Q factor, 0.0 to 1.0)
  inline void SetResonance(double res) { mResonance = res; }

  // Set morph position (0.0 = lowpass, 0.5 = phaser, 1.0 = formant)
  inline void SetMorph(double morph) { mMorph = morph; }

  // Process a single sample through the filter
  inline double Process(double input) {
    // Compute interpolated coefficients based on morph position
    ComputeCoefficients();

    // Apply the interpolated biquad
    double output = mBiquad.b0 * input + mBiquad.b1 * mBiquad.x1 +
                    mBiquad.b2 * mBiquad.x2 - mBiquad.a1 * mBiquad.y1 -
                    mBiquad.a2 * mBiquad.y2;

    // Shift delay states
    mBiquad.x2 = mBiquad.x1;
    mBiquad.x1 = input;
    mBiquad.y2 = mBiquad.y1;
    mBiquad.y1 = output;

    return output;
  }

private:
  double mSampleRate = 44100.0;
  double mCutoff = 1000.0;
  double mResonance = 0.5;
  double mMorph = 0.0;

  Biquad mBiquad;
  Biquad mLowpass;
  Biquad mPhaser;
  Biquad mFormant;

  // Compute biquad coefficients from pole/zero positions
  inline void ComputeCoefficients() {
    double cutoff = mCutoff;
    double res = mResonance;

    // Clamp parameters
    if (cutoff < 10.0) cutoff = 10.0;
    if (cutoff > mSampleRate * 0.49) cutoff = mSampleRate * 0.49;
    if (res < 0.0) res = 0.0;
    if (res > 0.99) res = 0.99;

    // Normalized cutoff frequency
    double wc = 2.0 * M_PI * cutoff / mSampleRate;
    double cosWc = cos(wc);
    double sinWc = sin(wc);

    // Resonance to Q mapping: higher resonance = tighter poles near unit circle
    // res=0 -> Q=0.5, res=0.99 -> Q~50
    double Q = 0.5 / (1.0 - res);
    double alpha = sinWc / (2.0 * Q);
    double a0 = 1.0 + alpha;

    // ── Filter Shape 1: Lowpass (mMorph = 0.0) ──────────────────────────
    // Standard Audio EQ Cookbook Lowpass
    mLowpass.b0 = ((1.0 - cosWc) / 2.0) / a0;
    mLowpass.b1 = (1.0 - cosWc) / a0;
    mLowpass.b2 = ((1.0 - cosWc) / 2.0) / a0;
    mLowpass.a1 = (-2.0 * cosWc) / a0;
    mLowpass.a2 = (1.0 - alpha) / a0;

    // ── Filter Shape 2: Notch/Phaser (mMorph = 0.5) ───────────────────────────
    // Standard Audio EQ Cookbook Notch Filter
    mPhaser.b0 = 1.0 / a0;
    mPhaser.b1 = (-2.0 * cosWc) / a0;
    mPhaser.b2 = 1.0 / a0;
    mPhaser.a1 = (-2.0 * cosWc) / a0;
    mPhaser.a2 = (1.0 - alpha) / a0;

    // ── Filter Shape 3: Vocal Formant/Bandpass (mMorph = 1.0) ────────────────────
    // Standard Audio EQ Cookbook Bandpass (constant 0 dB peak gain)
    // Higher Q for a more "vocal" formant peak
    double Qformant = Q * 2.0; 
    double alphaF = sinWc / (2.0 * Qformant);
    double a0F = 1.0 + alphaF;

    mFormant.b0 = alphaF / a0F;
    mFormant.b1 = 0.0;
    mFormant.b2 = -alphaF / a0F;
    mFormant.a1 = (-2.0 * cosWc) / a0F;
    mFormant.a2 = (1.0 - alphaF) / a0F;

    // ── Interpolate coefficients based on morph position ─────────────────
    // Morph 0.0 → Lowpass, 0.5 → Notch, 1.0 → Bandpass
    double morph = mMorph;
    double b0, b1, b2, a1, a2;

    if (morph <= 0.5) {
      double t = morph * 2.0; // 0.0 to 1.0
      double it = 1.0 - t;
      b0 = it * mLowpass.b0 + t * mPhaser.b0;
      b1 = it * mLowpass.b1 + t * mPhaser.b1;
      b2 = it * mLowpass.b2 + t * mPhaser.b2;
      a1 = it * mLowpass.a1 + t * mPhaser.a1;
      a2 = it * mLowpass.a2 + t * mPhaser.a2;
    } else {
      double t = (morph - 0.5) * 2.0; // 0.0 to 1.0
      double it = 1.0 - t;
      b0 = it * mPhaser.b0 + t * mFormant.b0;
      b1 = it * mPhaser.b1 + t * mFormant.b1;
      b2 = it * mPhaser.b2 + t * mFormant.b2;
      a1 = it * mPhaser.a1 + t * mFormant.a1;
      a2 = it * mPhaser.a2 + t * mFormant.a2;
    }

    // Apply interpolated coefficients to active biquad
    mBiquad.b0 = b0;
    mBiquad.b1 = b1;
    mBiquad.b2 = b2;
    mBiquad.a1 = a1;
    mBiquad.a2 = a2;
  }
};

// ── Plugin class ───────────────────────────────────────────────────────────

class EwolFX_ZPlane : public EWPlugin {
public:
  const char *GetPluginName() const override { return EWOLFX_PLUGIN_NAME; }
  EWOL::Accent GetAccent() const override { return EWOLFX_ACCENT; }
  int GetNumKnobs() const override { return EWOLFX_NUM_KNOBS; }

  EwolFX_ZPlane(const InstanceInfo &info);

#if IPLUG_DSP
  void ProcessBlock(sample **inputs, sample **outputs, int nFrames) override;
#endif

private:
  void SetupLayout(IGraphics *pGraphics) override;

  // DSP state
  double mSampleRate = 0.0;
  ZPlaneFilter mFilterLeft;
  ZPlaneFilter mFilterRight;
};

// ── Plugin entry point (REQUIRED) ──────────────────────────────────────────

Plugin *OnCreate(const char *version) {
  return new EwolFX_ZPlane(InstanceInfo());
}

#if IPLUG_EDIT_API
#include "plugadapter.h"
iplug::edit_api::PluginAdapter *
OnCreateEditAdapter(const iplug::edit_api::HostInfo &host) {
  return new iplug::edit_api::PluginAdapter<EwolFX_ZPlane>(host);
}
#endif
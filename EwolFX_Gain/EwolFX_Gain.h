#pragma once

// ============================================================================
// EwolFX_Gain — Gain plugin using EWOL template framework
// ============================================================================

#include "../EWOL/EWPlugin.h"

// ── Plugin metadata ────────────────────────────────────────────────────────

#define EWOLFX_PLUGIN_NAME "EwolFX_Gain"
#define EWOLFX_ACCENT                                                          \
  EWOL::Accent::Red // Distortion/drive accent
#define EWOLFX_NUM_KNOBS 1            // Single gain knob
#define EWOLFX_VERSION "1.0.0"

// ── Parameter enum ─────────────────────────────────────────────────────────

enum EParams { kGain = 0, kNumParams };

// ── Plugin class ───────────────────────────────────────────────────────────

class EwolFX_Gain : public EWPlugin {
public:
  const char *GetPluginName() const override { return EWOLFX_PLUGIN_NAME; }
  EWOL::Accent GetAccent() const override {
    return EWOLFX_ACCENT;
  }
  int GetNumKnobs() const override { return EWOLFX_NUM_KNOBS; }

  EwolFX_Gain(const InstanceInfo &info);

#if IPLUG_DSP
  void ProcessBlock(sample **inputs, sample **outputs, int nFrames) override;
#endif

private:
  void SetupLayout(IGraphics *pGraphics) override;
};

// ── Plugin entry point (REQUIRED) ──────────────────────────────────────────

Plugin *OnCreate(const char *version) {
  return new EwolFX_Gain(InstanceInfo());
}

#if IPLUG_EDIT_API
#include "plugadapter.h"
iplug::edit_api::PluginAdapter *
OnCreateEditAdapter(const iplug::edit_api::HostInfo &host) {
  return new iplug::edit_api::PluginAdapter<EwolFX_Gain>(host);
}
#endif
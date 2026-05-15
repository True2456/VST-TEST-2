#pragma once

// ============================================================================
// EwolFX Plugin Template
// Copy this folder, rename files, and fill in the blanks.
// ============================================================================

#include "EWOL/EWOL.h"

// ── Plugin metadata (CHANGE THESE) ────────────────────────────────────────

#define EWOLFX_PLUGIN_NAME "EwolFX Plugin"
#define EWOLFX_ACCENT Accent::Amber // Amber|Blue|Red|Purple|Teal|Orange|Green
#define EWOLFX_NUM_KNOBS 5          // Number of knobs (auto-creates UI)
#define EWOLFX_VERSION "1.0.0"

// ── Parameter enum (ADD/REMOVE params as needed) ──────────────────────────

enum EParams {
  kParam1 = 0,
  kParam2,
  kParam3,
  kParam4,
  kParam5,
  kNumParams // Always last — auto-calculates param count
};

// ── Plugin class ──────────────────────────────────────────────────────────

class EwolFXPlugin : public EWPlugin {
public:
  // Use the macro to implement required virtual methods
  const char *GetPluginName() const override { return EWOLFX_PLUGIN_NAME; }
  Accent GetAccent() const override { return EWOLFX_ACCENT; }
  int GetNumKnobs() const override { return EWOLFX_NUM_KNOBS; }

  EwolFXPlugin(const InstanceInfo &info);

#if IPLUG_DSP
  void ProcessBlock(sample **inputs, sample **outputs, int nFrames) override;
#endif

private:
  // Override this to add extra controls (sliders, buttons, etc.)
  void SetupLayout(IGraphics *pGraphics) override;
};

// ── Plugin entry point (REQUIRED — do not change) ─────────────────────────

Plugin *OnCreate(const char * /*version*/) {
  return new EwolFXPlugin(InstanceInfo::Create());
}

#if IPLUG_EDIT_API
#include "plugadapter.h"
iplug::edit_api::PluginAdapter *
OnCreateEditAdapter(const iplug::edit_api::HostInfo &host) {
  return new iplug::edit_api::PluginAdapter<EwolFXPlugin>(host);
}
#endif
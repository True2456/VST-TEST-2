#pragma once

// ============================================================================
// MyPermissivePlugin — EwolFX template proof of concept
// A gain plugin demonstrating the EWOL framework.
// ============================================================================

#include "EWOL/EWOL.h"

// ── Plugin metadata ───────────────────────────────────────────────────────

#define EWOLFX_PLUGIN_NAME "MyPermissivePlugin"
#define EWOLFX_ACCENT Accent::Green // Generic/utility accent
#define EWOLFX_NUM_KNOBS 1          // Single gain knob
#define EWOLFX_VERSION "1.0.0"

// ── Parameter enum ────────────────────────────────────────────────────────

enum EParams { kGain = 0, kNumParams };

// ── Plugin class ──────────────────────────────────────────────────────────

class MyPermissivePlugin : public EWPlugin {
public:
  const char *GetPluginName() const override { return EWOLFX_PLUGIN_NAME; }
  Accent GetAccent() const override { return EWOLFX_ACCENT; }
  int GetNumKnobs() const override { return EWOLFX_NUM_KNOBS; }

  MyPermissivePlugin(const InstanceInfo &info);

#if IPLUG_DSP
  void ProcessBlock(sample **inputs, sample **outputs, int nFrames) override;
#endif

private:
  void SetupLayout(IGraphics *pGraphics) override;
};

// ── Plugin entry point (REQUIRED) ─────────────────────────────────────────

Plugin *OnCreate(const char * /*version*/) {
  return new MyPermissivePlugin(InstanceInfo::Create());
}

#if IPLUG_EDIT_API
#include "plugadapter.h"
iplug::edit_api::PluginAdapter *
OnCreateEditAdapter(const iplug::edit_api::HostInfo &host) {
  return new iplug::edit_api::PluginAdapter<MyPermissivePlugin>(host);
}
#endif
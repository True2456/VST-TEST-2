#pragma once

// ============================================================================
// EwolFX_ZPlane — Z-Plane Filter plugin configuration (iPlug2 convention)
// ============================================================================

#define PLUG_TYPE 0 // 0 = Effect, 1 = Instrument, 2 = MIDI Effect
#define PLUG_TYPE_STR "Effect"

// Version: 0xVVVVRMMM where VV = major, R = revision, MMM = minor
#define PLUG_VERSION_HEX 0x00010000

// Version string
#define PLUG_VERSION_STR "1.0.0"

// Unique four-character manufacturer ID (must be valid OSType)
#define PLUG_MFR_ID 'Ewol'

// Manufacturer and product names
#define PLUG_MFR "EwolFX"
#define PLUG_NAME "ZPlane"

// Class name (must match your plugin class name)
#define PLUG_CLASS_NAME EwolFX_ZPlane

// Bundle names (macOS/iOS)
#define BUNDLE_NAME "EwolFX_ZPlane"
#define BUNDLE_MFR "EwolFX"
#define BUNDLE_DOMAIN "com.ewolfx"

// Unique four-character UID for VST3 (change per plugin)
#define PLUG_UNIQUE_ID 'ZPln'

// Audio I/O settings
#define PLUG_CHANNEL_IO "2-2"

// Plugin categories
#define PLUG_CATEGORIES "Effect", "Filter"

// Compatibility
#define PLUG_DOES_MIDI 0
#define PLUG_DOES_MIDI_MSG 0
#define PLUG_DOES_MIDI_IN 0
#define PLUG_DOES_MIDI_OUT 0
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 0
#define PLUG_DOES_AUDIO_IN 1
#define PLUG_DOES_AUDIO_OUT 1
#define PLUG_LATENCY 0
#define VST3_SUBCATEGORY "Fx"

// Copyright (max 80 chars)
#define PLUG_COPYRIGHT_STR "Copyright EwolFX 2026"

// Website and email
#define PLUG_URL ""
#define PLUG_EMAIL ""

// Presets
#define PLUG_NUM_PRESETS 128

// UI Configuration
#define PLUG_HAS_UI 1
#define PLUG_WIDTH 480
#define PLUG_HEIGHT 360
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0
#define PLUG_HOST_RESIZE 0
#define SHARED_RESOURCES_SUBPATH "EwolFX_ZPlane"

// Number of parameters
#define PLUG_NUM_PARAMS 3

// API support
#define USE_API_VST3 1

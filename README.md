# EwolFX VST3 Template Framework

This repository serves as an AI-ready, robust template for generating state-of-the-art C++ VST3 plugins using the iPlug2 framework. It is specifically designed to be easily manipulated by smaller local LLMs (like Qwen 2.5/3.6, Llama 3 8B, etc.) to produce high-quality, stable agentic code.

## Why this template?

Traditional C++ VST3 development involves heavily coupled GUI logic, dense dependency trees, and pointer-heavy UI bindings. Small models often hallucinate complex GUI frameworks or fail to compile them because of missing headers or strict `const` correctness requirements in C++.

This framework provides an abstract wrapper (**EWOL**) over iPlug2 that solves these issues:

1. **Pre-configured Colors**: `EWColors.h` provides an ARGB palette using modern UI tokens.
2. **Simplified UI Components**: `EWControls.h` provides zero-boilerplate controls (`EWKnob`, `EWLabel`, `EWPanel`) that automatically wrap iPlug2's `IVKnobControl` and `ITextControl`. Models don't need to learn IPlug2's massive `IVStyle` or color arrays.
3. **Automated Layouts**: The core `EWPlugin` automatically handles UI setup, bounds calculation, font loading, and background panel attachment. A model simply needs to add controls in the `SetupLayout()` method.
4. **Stable DSP**: Clean `ProcessBlock()` examples for handling channels properly.

## Getting Started

1. Clone this repository.
2. Ensure you have CMake and the macOS/Windows SDK tools installed.
3. Use the `EwolFX_Gain` folder as your template for new plugins.
4. Modify `config.h` to change the Plugin Name, I/O, and Manufacturer.
5. Add logic to `SetupLayout()` and `ProcessBlock()`.

## Data Flow Architecture

See `DATA_FLOW.md` for a complete visual representation of how the framework operates.
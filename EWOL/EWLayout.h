#pragma once

// EWLayout — Grid layout helpers for EwolFX plugins
// Must be included AFTER IGraphics.h is available

#include "IGraphics.h"
#include <cmath>

namespace EWLayout {

// Compute number of columns based on available width
inline int ComputeCols(int availableWidth, int minKnobSize = 36) {
  int requiredWidth = (minKnobSize + 20) * 3; // 3 cols minimum width
  return (availableWidth >= requiredWidth) ? 3 : 2;
}

// Compute grid layout: fills array of IRECT for each control
inline void ComputeGrid(
    const iplug::igraphics::IRECT &available, int numControls, int numCols,
    int knobDiameter, int gap,
    iplug::igraphics::WDL_SmallVector<iplug::igraphics::IRECT, 32> &outBounds) {
  outBounds.Clear();

  int cols =
      (numCols > 0) ? numCols : ComputeCols(available.W(), knobDiameter * 0.75);
  int rows = (numControls + cols - 1) / cols; // ceiling division

  int cellW = (available.W() - (cols - 1) * gap) / cols;
  int cellH = (available.H() - (rows - 1) * gap) / rows;

  // Use the smaller dimension for square cells
  int cellSize = (cellW < cellH) ? cellW : cellH;

  for (int i = 0; i < numControls; i++) {
    int col = i % cols;
    int row = i / cols;

    iplug::igraphics::IRECT cell(
        available.L() + col * (cellSize + gap),
        available.T() + row * (cellSize + gap),
        available.L() + (col + 1) * cellSize + col * gap,
        available.T() + (row + 1) * cellSize + row * gap);

    // Center knob within cell
    iplug::igraphics::IRECT knob = cell.GetCentredInside(knobDiameter);
    outBounds.Add() = knob;
  }
}

// Compute knob position for a single index
inline iplug::igraphics::IRECT
GetKnobRect(const iplug::igraphics::IRECT &available, int index,
            int numControls, int numCols, int diameter, int gap) {
  iplug::igraphics::WDL_SmallVector<iplug::igraphics::IRECT, 32> bounds;
  ComputeGrid(available, numControls, numCols, diameter, gap, bounds);
  if (index < (int)bounds.GetSize()) {
    return bounds[index];
  }
  return iplug::igraphics::IRECT(); // out of range
}

// Compute header bounds (top bar for brand + version)
inline iplug::igraphics::IRECT
GetHeaderBounds(const iplug::igraphics::IRECT &parent, int height = 32) {
  return parent.GetHShifted(0).GetTOPHEIGHT(height);
}

// Compute footer bounds (bottom bar for plugin name + LED)
inline iplug::igraphics::IRECT
GetFooterBounds(const iplug::igraphics::IRECT &parent, int height = 28) {
  return parent.GetHShifted(0).GetBOTTOMHEIGHT(height);
}

// Compute bypass LED position (bottom-right of footer)
inline iplug::igraphics::IRECT
GetLedBounds(const iplug::igraphics::IRECT &footer, int size = 10) {
  return footer.GetFromTRHC(size + 16, size / 2);
}

// Compute label/value area below a knob
inline iplug::igraphics::IRECT
GetLabelBounds(const iplug::igraphics::IRECT &knob, int height = 36) {
  return knob.GetVShifted(knob.H() + 4).GetHEIGHT(height);
}

// Format a parameter value as string for display
inline void FormatValue(char *buf, int bufSize, double value,
                        const char *unit) {
  if (value >= 1000.0)
    snprintf(buf, bufSize, "%.0f%s", value, unit);
  else if (value >= 100.0)
    snprintf(buf, bufSize, "%.1f%s", value, unit);
  else if (value >= 10.0)
    snprintf(buf, bufSize, "%.2f%s", value, unit);
  else
    snprintf(buf, bufSize, "%.3f%s", value, unit);
}

// Capitalize first letter of a string (for parameter name display)
inline void Capitalize(char *out, int outSize, const char *input) {
  bool newWord = true;
  int i = 0;
  out[0] = '\0';
  while (input[i]) {
    if (input[i] == ' ') {
      newWord = true;
      out[i] = ' ';
    } else if (newWord) {
      out[i] = toupper((unsigned char)input[i]);
      newWord = false;
    } else {
      out[i] = tolower((unsigned char)input[i]);
    }
    i++;
  }
  out[i] = '\0';
}

} // namespace EWLayout
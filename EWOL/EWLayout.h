#pragma once

#include "EWColors.h"
#include "IPlug_include_in_plug_hdr.h"

// ============================================================================
// EwolFX Layout Calculator — Responsive grid for knobs/sliders
// Automatically computes control positions based on available space.
// ============================================================================

namespace EWLayout {

// Layout configuration — override these per-plugin if needed
struct Config {
  int numCols = 3;       // Default columns (auto-reflows to 2 at small sizes)
  int numRows = 2;       // Default rows (computed from numKnobs / numCols)
  int knobDiameter = 48; // Default knob size (scales with UI)
  int gridGap = 24;      // Gap between controls
  int paddingX = 32;     // Horizontal padding from edges
  int paddingY = 40;     // Vertical padding (header space)
  int footerHeight = 28; // Bottom area for plugin name + LED
  int ledSize = 10;      // Bypass LED diameter
};

// Compute number of columns based on available width
inline int ComputeCols(int availableWidth, int minKnobSize = 36) {
  int requiredWidth = (minKnobSize + 20) * 3; // 3 cols minimum width
  return (availableWidth >= requiredWidth) ? 3 : 2;
}

// Compute grid layout: returns array of IRECT for each control
// Controls are filled left-to-right, top-to-bottom
inline void ComputeGrid(const IRECT &available, int numControls, int numCols,
                        int knobDiameter, int gap, IVector<IRECT> &outBounds) {
  outBounds.Clear();
  outBounds.Reserve(numControls);

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

    IRECT cell(available.L() + col * (cellSize + gap),
               available.T() + row * (cellSize + gap),
               available.L() + (col + 1) * cellSize + col * gap,
               available.T() + (row + 1) * cellSize + row * gap);

    // Center knob within cell
    IRECT knob = cell.GetCentredInside(knobDiameter);
    outBounds.Emplace(knob);
  }
}

// Compute knob position for a single index
inline IRECT GetKnobRect(const IRECT &available, int index, int numControls,
                         int numCols, int diameter, int gap) {
  IVector<IRECT> bounds;
  ComputeGrid(available, numControls, numCols, diameter, gap, bounds);
  if (index < bounds.GetSize()) {
    return bounds[index];
  }
  return IRECT(); // out of range
}

// Compute header bounds (top bar for brand + version)
inline IRECT GetHeaderBounds(const IRECT &parent, int height = 32) {
  return parent.GetTOPHEIGHT(height);
}

// Compute footer bounds (bottom bar for plugin name + LED)
inline IRECT GetFooterBounds(const IRECT &parent, int height = 28) {
  return parent.GetBOTTOMHEIGHT(height);
}

// Compute bypass LED position (bottom-right of footer)
inline IRECT GetLedBounds(const IRECT &footer, int size = 10) {
  return footer.GetFromTRHC(size + 16, size / 2);
}

// Compute label/value area below a knob
inline IRECT GetLabelBounds(const IRECT &knob, int height = 36) {
  return knob.GetVShifted(knob.H() + 4).GetHEIGHT(height);
}

// Format a parameter value as string for display
inline WDL_String FormatValue(double value, const char *unit) {
  WDL_String str;
  if (value >= 1000.0) {
    str.Format("%.0f%s", value, unit);
  } else if (value >= 100.0) {
    str.Format("%.1f%s", value, unit);
  } else if (value >= 10.0) {
    str.Format("%.2f%s", value, unit);
  } else {
    str.Format("%.3f%s", value, unit);
  }
  return str;
}

// Capitalize first letter of a string (for parameter name display)
inline WDL_String Capitalize(const char *input) {
  WDL_String str;
  str.Set(input);
  if (str.Get() && str.Get()[0]) {
    // Convert uppercase param name to Title Case
    bool newWord = true;
    for (int i = 0; str.Get()[i]; i++) {
      if (str.Get()[i] == ' ') {
        newWord = true;
      } else if (newWord) {
        str.Get()[i] = toupper(str.Get()[i]);
        newWord = false;
      } else {
        str.Get()[i] = tolower(str.Get()[i]);
      }
    }
  }
  return str;
}

} // namespace EWLayout
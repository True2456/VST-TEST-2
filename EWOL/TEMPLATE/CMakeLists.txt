# ============================================================================
# EwolFX Plugin CMake Template
# Copy this file and the TEMPLATE/ folder contents to create a new plugin.
# Rename files: replace "EwolFXPlugin" with your plugin name.
# ============================================================================

cmake_minimum_required(VERSION 3.14)

# ── Plugin name (CHANGE THIS) ─────────────────────────────────────────────
project(EwolFXPlugin VERSION 1.0.0)

# ── iPlug2 path (adjust if EWOL is elsewhere) ─────────────────────────────
set(IPLUG2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../dependencies/iplug2" CACHE PATH "iPlug2 root")
include(${IPLUG2_DIR}/iPlug2.cmake)

# ── Build the plugin ──────────────────────────────────────────────────────
iplug_add_plugin(${PROJECT_NAME}
  HEADER_NAMES EWOLFX_PLUGIN_NAME
  SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/EwolFXPlugin.cpp
  INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
  FORMATS VST3
  GRAPHICS "${CMAKE_CURRENT_SOURCE_DIR}/../.."
)

# ── EWOL include path ─────────────────────────────────────────────────────
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

# ── Output directory ──────────────────────────────────────────────────────
set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out"
)

# ── Post-build: copy to VST3 directory (macOS) ───────────────────────────
if(APPLE)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
      "~/Library/Audio/Plug-Ins/VST3/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>"
      "~/Library/Audio/Plug-Ins/VST3/${PROJECT_NAME}.vst3/"
    COMMENT "Copying ${PROJECT_NAME} to ~/Library/Audio/Plug-Ins/VST3/"
  )

  # Re-sign after copy (required by macOS)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND codesign -f --sign -
      "~/Library/Audio/Plug-Ins/VST3/${PROJECT_NAME}.vst3"
    COMMENT "Re-signing ${PROJECT_NAME}.vst3"
  )
endif()
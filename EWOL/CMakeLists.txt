# ============================================================================
# EwolFX — CMake build configuration for the template framework
# Include this from your plugin's CMakeLists.txt:
#   add_subdirectory(EWOL)
# ============================================================================

# EWOL is header-only — no library target needed.
# Just set the include path:
#   target_include_directories(MyPlugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/EWOL)

message(STATUS "EwolFX template loaded (header-only framework)")
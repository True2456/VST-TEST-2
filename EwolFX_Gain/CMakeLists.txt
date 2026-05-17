cmake_minimum_required(VERSION 3.14)
project(EwolFX_Gain VERSION 1.0.0)

# Discover IPLUG2_DIR if not already set
if(NOT DEFINED IPLUG2_DIR)
  set(IPLUG2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/iplug2" CACHE PATH "iPlug2 root directory")
endif()

# Discover EWOL_DIR if not already set
if(NOT DEFINED EWOL_DIR)
  set(EWOL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../EWOL" CACHE PATH "EwolFX template root directory")
endif()

# Include iPlug2 CMake configuration
include(${IPLUG2_DIR}/iPlug2.cmake)

find_package(iPlug2 REQUIRED)

# EWOL include path
set(EWOL_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../EWOL")

iplug_add_plugin(${PROJECT_NAME}
  SOURCES
    EwolFX_Gain.cpp
    EwolFX_Gain.h
    resources/resource.h
  RESOURCES
    resources/fonts/Roboto-Regular.ttf
  FORMATS VST3
)

# EWOL include path — needed for EWOL/EWOL.h and custom controls
target_include_directories(${PROJECT_NAME}-vst3 PRIVATE ${EWOL_INCLUDE})

# macOS bundle settings
if(APPLE)
  set_target_properties(${PROJECT_NAME}-vst3 PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.ewolfx.Gain"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-")

  # Post-build: copy to VST3 directory and re-sign
  add_custom_command(TARGET ${PROJECT_NAME}-vst3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "~/Library/Audio/Plug-Ins/VST3/")
  add_custom_command(TARGET ${PROJECT_NAME}-vst3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}-vst3>"
      "~/Library/Audio/Plug-Ins/VST3/${PROJECT_NAME}.vst3/")
  add_custom_command(TARGET ${PROJECT_NAME}-vst3 POST_BUILD
    COMMAND codesign -f --sign - "--deep" "~/Library/Audio/Plug-Ins/VST3/${PROJECT_NAME}.vst3")
  add_custom_command(TARGET ${PROJECT_NAME}-vst3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Installed to ~/Library/Audio/Plug-Ins/VST3/${PROJECT_NAME}.vst3")
endif()

# Windows: copy to VST3 directory
if(WIN32)
  set(VST3_PATH "$ENV{PROGRAMFILES}/Steinberg/VST3")
  add_custom_command(TARGET ${PROJECT_NAME}-vst3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${VST3_PATH}")
  add_custom_command(TARGET ${PROJECT_NAME}-vst3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}-vst3>"
      "${VST3_PATH}/${PROJECT_NAME}-vst3/")
endif()
cmake_minimum_required(VERSION 3.14)
project(MyPermissivePlugin VERSION 1.0.0)

# Discover IPLUG2_DIR if not already set (for independent builds)
if(NOT DEFINED IPLUG2_DIR)
  set(IPLUG2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." CACHE PATH "iPlug2 root directory")
endif()

# Discover EWOL_DIR if not already set
if(NOT DEFINED EWOL_DIR)
  set(EWOL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../EWOL" CACHE PATH "EwolFX template root directory")
endif()

# Include iPlug2 CMake configuration
include(${IPLUG2_DIR}/iPlug2.cmake)

# IGraphics backend/renderer can be set via command line:
#   cmake -DIGRAPHICS_BACKEND=SKIA -DIGRAPHICS_RENDERER=GL3 ..
# Defaults: NANOVG/METAL on macOS, NANOVG/GL2 on Windows
# See iPlug2/Scripts/cmake/IGraphics.cmake for all options

find_package(iPlug2 REQUIRED)

# Build all formats except AUv3 (AUv3 is opt-in)
# To include AUv3: FORMATS ALL_AUV3
# To select specific formats: FORMATS APP VST3 AU
# Add EWOL include path for the EwolFX template framework
set(EWOL_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../EWOL")

iplug_add_plugin(${PROJECT_NAME}
  SOURCES
    MyPermissivePlugin.cpp
    MyPermissivePlugin.h
    resources/resource.h
  RESOURCES
    resources/fonts/Roboto-Regular.ttf
  FORMATS VST3
)

# EWOL include path — needed for EWOL/EWOL.h and custom controls
target_include_directories(${PROJECT_NAME} PRIVATE ${EWOL_INCLUDE})

# Post-build: copy to VST3 directory (macOS)
if(APPLE)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "~/Library/Audio/Plug-Ins/VST3/")
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>"
      "~/Library/Audio/Plug-Ins/VST3/${PROJECT_NAME}.vst3/")
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND codesign -f --sign - "~/Library/Audio/Plug-Ins/VST3/${PROJECT_NAME}.vst3")
  set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/resources/MyPermissivePlugin-macOS-Info.plist"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.ewolfx.MyPermissivePlugin")
endif()

import ctypes
import sys

lib = ctypes.cdll.LoadLibrary("/Users/true/Documents/VST3/EwolFX_Gain.vst3/Contents/MacOS/EwolFX_Gain")

GetPluginFactory = lib.GetPluginFactory
GetPluginFactory.restype = ctypes.c_void_p

factory = GetPluginFactory()
if factory:
    print(f"Factory obtained successfully: {factory}")
else:
    print("Factory returned NULL")


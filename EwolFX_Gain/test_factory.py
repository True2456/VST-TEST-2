import ctypes
import sys

# Define VST3 GUID struct
class TUID(ctypes.Structure):
    _fields_ = [("data", ctypes.c_uint8 * 16)]

# Define IPluginFactory struct
class IPluginFactory(ctypes.Structure):
    pass

class IPluginFactory_vtbl(ctypes.Structure):
    _fields_ = [
        ("queryInterface", ctypes.c_void_p),
        ("addRef", ctypes.c_void_p),
        ("release", ctypes.c_void_p),
        ("getFactoryInfo", ctypes.c_void_p),
        ("countClasses", ctypes.CFUNCTYPE(ctypes.c_int32, ctypes.POINTER(IPluginFactory))),
        ("getClassInfo", ctypes.c_void_p),
        ("createInstance", ctypes.c_void_p)
    ]

IPluginFactory._fields_ = [("lpVtbl", ctypes.POINTER(IPluginFactory_vtbl))]

try:
    lib = ctypes.cdll.LoadLibrary("/Users/true/Documents/VST3/EwolFX_Gain.vst3/Contents/MacOS/EwolFX_Gain")
    GetPluginFactory = lib.GetPluginFactory
    GetPluginFactory.restype = ctypes.POINTER(IPluginFactory)
    
    print("Calling GetPluginFactory()...")
    factory = GetPluginFactory()
    
    if factory:
        print(f"Factory obtained: {factory}")
        count = factory.contents.lpVtbl.contents.countClasses(factory)
        print(f"Factory reports {count} classes.")
    else:
        print("Factory returned NULL")
except Exception as e:
    print(f"Exception: {e}")


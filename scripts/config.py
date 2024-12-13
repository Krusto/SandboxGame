class CONFIG:
    VENDOR_DIR = 'Vendor'
    BUILD_DIR = './build'
    BUILD_SHARED = True
    IS_MSVC = True
    MODE = 'Debug'
    LIBS_STATIC = [
        ['glfw-3.4/src', 'glfw3'],
        ['yaml-cpp', 'yaml-cpp'],
        ['FastNoise/src', 'FastNoise']
    ]
    LIBS = [
        ['glfw-3.4/src', 'glfw3'],
        ['yaml-cpp', 'yaml-cppd'],
        ['FastNoise/src', 'FastNoiseD','buildtype']
    ]
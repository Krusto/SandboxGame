import sys
import os
from config import *
from pathlib import Path
def copylib(libsource,libtarget,liboutputname, config):
    os.makedirs(f"{libtarget}/{config.MODE}/bin",exist_ok=True)
    print(f'copy {libsource}/{config.MODE}/{liboutputname}.lib to {libtarget}/{config.MODE}/bin/{liboutputname}.lib')
    os.system(f'cp {libsource}/{config.MODE}/{liboutputname}.lib {libtarget}/{config.MODE}/bin/{liboutputname}.lib')
    if config.IS_MSVC and config.MODE == "Debug":
        print(f'copy {libsource}/{config.MODE}/{liboutputname}.pdb to {libtarget}/{config.MODE}/bin/{liboutputname}.pdb')
        os.system(f'cp {libsource}/{config.MODE}/{liboutputname}.pdb {libtarget}/{config.MODE}/bin/{liboutputname}.pdb')
    if config.IS_MSVC:
        print(f'copy {libsource}/{config.MODE}/{liboutputname}.exp to {libtarget}/{config.MODE}/bin/{liboutputname}.exp')
        os.system(f'cp {libsource}/{config.MODE}/{liboutputname}.exp {libtarget}/{config.MODE}/bin/{liboutputname}.exp')
    if config.BUILD_SHARED:
        print(f'copy {libsource}/{config.MODE}/{liboutputname}.dll to {libtarget}/{config.MODE}/bin/{liboutputname}.dll')
        os.system(f'cp {libsource}/{config.MODE}/{liboutputname}.dll {libtarget}/{config.MODE}/bin/{liboutputname}.dll')


if __name__ == '__main__':
    sys.argv.append("Debug")
    config = CONFIG

    if sys.argv[1] == "Debug":
        config.MODE = "Debug"
    elif sys.argv[1] == "Release":
        config.MODE = "Release"

    files_in_build_dir = os.listdir(f"{config.BUILD_DIR}/{config.MODE}")
    for file in files_in_build_dir:
        if file != "bin" and file != "lib":
            copylib(f"{config.BUILD_DIR}",f"{config.BUILD_DIR}",Path(file).stem,config)

    if config.BUILD_SHARED:
        for lib in config.LIBS:
            copylib(f"{config.BUILD_DIR}/Vendor/{lib[0]}",f"{config.BUILD_DIR}",lib[1],config)
    else:
        print("not implemented")
import os
import sys
import shutil
import subprocess
import re

pattern_header = r"^(.*?)\.(h|hpp)$"
pattern_cpp = r"^(.*?)\.(cpp|cc|c|cxx)$"

dest = '../external/imgui/'

repos = [
    'https://github.com/ocornut/imgui',
    'https://github.com/epezent/implot',
    'https://github.com/Nelarius/imnodes',
    'https://github.com/nothings/stb'
]

files_to_copy = [
    'imconfig.h',
    'imgui_freetype.h',
    'imgui_impl_glfw.h',
    'imgui_impl_opengl3_loader.h',
    'imgui_impl_opengl3.h',
    'imgui_internal.h',
    'imgui.h',
    'imnodes_internal.h',
    'imnodes.h',
    'implot_internal.h',
    'implot.h',
    'imstb_rectpack.h',
    'imstb_textedit.h',
    'imstb_truetype.h',
    'stb_image.h',
    'imgui.cpp',
    'imgui_draw.cpp',
    'imgui_freetype.cpp',
    'imgui_impl_glfw.cpp',
    'imgui_impl_opengl3.cpp',
    'imgui_tables.cpp',
    'imgui_widgets.cpp',
    'imnodes.cpp',
    'implot.cpp',
    'implot_items.cpp',
]

utils_files = [
    'binary_to_compressed_c.cpp',
]

def buildFontCode():
    result = subprocess.run(["cmake", "-DCMAKE_INSTALL_PREFIX=" + os.getcwd() + "\\fonts", "-B", os.getcwd() + "\\temp\\tools"])
    if result.returncode != 0:
        print("Error Generating Cmake cache for font Generation code")
        exit(1)

    result = subprocess.run(["cmake", "--build", os.getcwd() + "\\temp\\tools", "--config", "Release", "--target", "INSTALL"])
    if result.returncode != 0:
        print("Error building font Generation code from source")
        exit(1)
    
def fetchSource():
    if not os.path.exists('./temp'):
        try:
            os.makedirs('./temp/tools')
        except:
            print('failed to create directory: temp')
            exit(1)

    for repo in repos:
        result = subprocess.run(["git", "clone", repo, "--depth", "1", "./temp/git/" + repo.split('/')[-1]])
        if result.returncode != 0:
            print("Error fetching source: " + repo)
            exit(1)

def copySource():
    for root, dirs, files in os.walk('./temp/git'):
        for file in files:
            if file in files_to_copy:
                if re.compile(pattern_header).search(file):
                    shutil.copy2(os.path.join(root, file), os.path.join(dest + "include/", file))
                if re.compile(pattern_cpp).search(file):
                    shutil.copy2(os.path.join(root, file), os.path.join(dest + "source/", file))

            if file in utils_files:
                shutil.copy2(os.path.join(root, file), os.path.join("./temp/tools", file))

    buildFontCode()

if __name__ == "__main__":
    if sys.version_info[0] < 3:
        print("Use Python 3 to run script")
        exit(1)

    fetchSource()
    copySource()
#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["src/", "src/thirdparty/fmod/inc"])

# godot-cpp only defines TOOLS_ENABLED for target=editor. This extension's
# .gdextension file uses the template_debug binary inside the Godot editor, so
# keep editor-only integration code enabled for template_debug builds as well.
if env["target"] == "template_debug":
    env.Append(CPPDEFINES=["TOOLS_ENABLED"])

env.Tool("compilation_db")
env.CompilationDatabase()
sources = (Glob("src/core/*.cpp") + Glob("src/audio/*.cpp") + Glob("src/playback/*.cpp") + 
           Glob("src/mixer/*.cpp") + Glob("src/dsp/*.cpp") + Glob("src/nodes/*.cpp") + 
           Glob("src/editor/*.cpp")) + Glob("src/geometry/*.cpp") + Glob("src/spatial/*.cpp")

if env["target"] in ["editor", "template_debug"]:
    doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
    sources.append(doc_data)

if env["platform"] == "android":
    windows_libs_blacklist = [
        "mingw32", "gcc", "gcc_s", "moldname", "mingwex", "msvcrt", 
        "advapi32", "shell32", "user32", "kernel32", "stdc++"
    ]
    if "LIBS" in env:
        clean_libs = [lib for lib in env["LIBS"] if str(lib) not in windows_libs_blacklist]
        env.Replace(LIBS=clean_libs)

if env["platform"] == "android":
    env.Append(CXXFLAGS=['-std=c++17'])
    
    arch = env["arch"]
    
    fmod_arch_dir = ""
    if arch == "arm64":
        fmod_arch_dir = "arm64-v8a"
    elif arch == "arm32":
        fmod_arch_dir = "armeabi-v7a"
    elif arch == "x86_64":
        fmod_arch_dir = "x86_64"
    elif arch == "x86_32":
        fmod_arch_dir = "x86"
    else:
        print(f"Warning: Unknown architecture {arch}, defaulting to arm64-v8a")
        fmod_arch_dir = "arm64-v8a"

    base_path = os.getcwd()
    fmod_lib_path = os.path.join(base_path, "src/thirdparty/fmod/lib/android", fmod_arch_dir)
    
    print(f"--- Debug: FMOD Lib Path for {arch}: {fmod_lib_path}")
    
    env.Append(LIBPATH=[fmod_lib_path])
    
    if env["target"] == "template_release":
        env.Append(LIBS=["fmod"])
    else:
        env.Append(LIBS=["fmodL"]) 

    if env["target"] == "template_release":
        env.Append(CXXFLAGS=['-O3'])
    else:
        env.Append(CXXFLAGS=['-g', '-O0'])
    
    library = env.SharedLibrary(
        "addons/fmod_player/bin/libfmod_player{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

elif env["platform"] == "macos":
    library = env.SharedLibrary(
        "addons/fmod_player/bin/fmod_player.{}.{}.framework/fmod_player.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )

elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "addons/fmod_player/bin/fmod_player.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "addons/fmod_player/bin/fmod_player.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )

else:
    env.Append(CCFLAGS=["/utf-8"])
    env.Append(CXXFLAGS=["/utf-8"])
    env.Append(CCFLAGS=["/wd4828"])

    env.Append(LIBPATH=["src/thirdparty/fmod/lib/x64"])
    env.Append(LIBS=["fmod_vc"])
    library = env.SharedLibrary(
        "addons/fmod_player/bin/fmod_player{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)

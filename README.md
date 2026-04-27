<p align="center">
  <img alt="fmod_player_banner" src="https://github.com/user-attachments/assets/1d2655f5-d440-4bdb-978a-b095cb275e9c" />
</p>

<h1 align="center">Godot-FmodPlayer</h1>

<p align="center">
  <b>FMOD Core API audio playback, mixing, DSP, and 3D spatial tools for Godot 4.</b>
</p>

<p align="center">
  <a href="https://github.com/LuYingYiLong/Godot-FmodPlayer/releases">
    <img src="https://img.shields.io/github/v/release/LuYingYiLong/Godot-FmodPlayer?include_prereleases&style=flat-square" alt="Release" />
  </a>
  <a href="#platform-support">
    <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Android-blue?style=flat-square" alt="Platform" />
  </a>
  <a href="LICENSE.txt">
    <img src="https://img.shields.io/github/license/LuYingYiLong/Godot-FmodPlayer?style=flat-square" alt="License" />
  </a>
  <a href="https://godotengine.org">
    <img src="https://img.shields.io/badge/Godot-4.1%2B-478CBF?style=flat-square&logo=godot-engine" alt="Godot Version" />
  </a>
  <a href="https://godot-fmodplayerdocs.readthedocs.io/en/latest/">
    <img src="https://img.shields.io/badge/docs-online-2ea44f?style=flat-square" alt="Documentation" />
  </a>
</p>

<p align="center">
  A Godot 4 GDExtension plugin for developers who want low-level FMOD playback<br>
  without giving up Godot-style nodes, resources, buses, inspectors, and editor workflows.
</p>

---

## Why This Plugin?

Godot-FmodPlayer is built around the **FMOD Core API**, not FMOD Studio. It is aimed at projects that need direct control over sounds, channels, channel groups, DSP graphs, streamed audio, 3D spatial playback, and runtime mixing.

Use it when you want:

- FMOD's mature decoder, mixer, and DSP runtime inside Godot.
- Godot-friendly nodes such as `FmodAudioStreamPlayer`, `FmodAudioStreamPlayer2D`, and `FmodAudioStreamPlayer3D`.
- Editor tools for importing, previewing, inspecting, and testing FMOD audio resources.
- A bus layout that follows Godot's `AudioServer` while routing playback through FMOD.
- Advanced 3D audio features such as geometry occlusion and spatial reverb zones.

> Looking for FMOD Studio event/bank workflows? This project intentionally focuses on **FMOD Core API**. For Studio API integration, see [fmod-gdextension](https://github.com/utopia-rise/fmod-gdextension).

## Highlights

### Godot-Native Workflow

- Import audio as FMOD stream resources directly in the editor.
- Preview FMOD audio resources from the inspector.
- Use custom icons, 3D gizmos, and editor tools for FMOD nodes.
- Configure FMOD system options from `Project Settings`.
- Use a helper tool to prepare Android export templates for FMOD runtime files.

### Playback And Loading

- Play common FMOD-supported formats: MP3, WAV, OGG Vorbis, FLAC, AIFF, MIDI, MOD, XM, S3M, IT, and more.
- Load from `res://`, `user://`, absolute file paths, memory buffers, and Godot PCK resources.
- Choose streaming or sample loading modes depending on memory and latency needs.
- Use advanced stream settings for special formats, subsounds, tracks, and FMOD create flags.

### Mixing And Routing

- Synchronize FMOD buses with Godot's audio bus layout.
- Route players to named buses.
- Use channel groups and sound groups for grouped playback control.
- Adjust volume, pitch, pan, mute, solo, bypass, and DSP parameters at runtime.
- Monitor FMOD CPU and file usage in Godot's Performance panel.

### DSP Effects

Godot-FmodPlayer includes FMOD-backed effects designed to feel familiar to Godot users:

| Effect | Use Case |
|--------|----------|
| Amplify | Gain and volume adjustment |
| Filter | Low-pass, high-pass, band-pass filtering |
| EQ / EQ6 / EQ10 / EQ21 | Graphic and multi-band equalization |
| Chorus | Modulated stereo thickening |
| Delay | Echo, taps, and feedback delay |
| Distortion | Drive, clipping, and saturation |
| Phaser | Sweeping phase modulation |
| Pitch Shift | Real-time pitch manipulation |
| Reverb | Godot-like room reverb mapping |
| Compressor | Dynamic range compression |
| Hard Limiter | Peak control |
| Stereo Enhance | Stereo width shaping |
| Panner | Stereo positioning |
| Spectrum Analyzer | Real-time frequency visualization |
| Capture | Runtime sample capture |
| Record | Bus recording and waveform snapshots |

### 3D Audio

- `FmodAudioStreamPlayer3D` for spatial playback.
- Listener updates from the active Godot camera.
- Distance attenuation and rolloff controls.
- `FmodGeometryInstance3D` for FMOD geometry occlusion.
- MeshInstance3D editor tools for quickly creating FMOD geometry.
- `FmodReverbZone3D` for localized environmental reverb.
- Editor gizmos for spatial audio debugging.

## What's New?

- Real-time recording waveform snapshot API.
- Audio visualizer HUD examples for 3D scenes.
- More accurate Godot-like mappings for chorus, delay, reverb, record, and spectrum analyzer effects.
- Advanced audio stream resource for special FMOD loading options.
- Higher-level FMOD geometry and reverb zone wrappers.
- Mesh tools for creating FMOD geometry from Godot meshes.
- Android export template setup helper.
- Improved editor audio preview experience.

## Quick Start

### 1. Install The Plugin

Recommended: install Godot-FmodPlayer from the Godot Asset Library:

**[Godot Asset Library - Godot-FmodPlayer](https://godotengine.org/asset-library/asset/4905)**

You can also download a release from GitHub or build the GDExtension from source if you need the latest development version.

### 2. Install FMOD Core API

Download the FMOD Core API SDK from [fmod.com](https://www.fmod.com) and place the required headers and libraries under:

```text
src/thirdparty/fmod/
```

FMOD runtime libraries are not distributed by this repository. You must provide them according to the FMOD license.

### 3. Build The GDExtension

Windows debug build:

```bash
scons platform=windows target=template_debug arch=x86_64
```

Windows release build:

```bash
scons platform=windows target=template_release arch=x86_64
```

Android arm64 debug build:

```bash
scons platform=android target=template_debug arch=arm64
```

### 4. Enable The Plugin

Open your Godot project and enable:

```text
Project > Project Settings > Plugins > fmod_player
```

### 5. Play Audio

Add one of the player nodes:

- `FmodAudioStreamPlayer`
- `FmodAudioStreamPlayer2D`
- `FmodAudioStreamPlayer3D`

Assign an FMOD audio stream resource, choose a bus, and press play.

## Platform Support

| Platform | Status | Notes |
|----------|--------|-------|
| Windows x86_64 | Supported | Main development target |
| Android arm64 / arm32 / x86 / x86_64 | Supported | Requires FMOD Android runtime setup |
| macOS | Experimental | Build script support exists, FMOD linking may need local adjustment |
| iOS | Experimental | Static library workflow may need project-specific setup |

## Requirements

- **Godot:** 4.1 or later
- **Language:** C++17 or newer
- **Windows compiler:** Visual Studio 2022 / MSVC v145+
- **Android compiler:** Android NDK
- **Build system:** SCons
- **Audio runtime:** FMOD Core API SDK

## Documentation

Full documentation is available here:

**[godot-fmodplayerdocs.readthedocs.io](https://godot-fmodplayerdocs.readthedocs.io/en/latest/)**

The documentation includes setup guides, API references, usage examples, and platform-specific export notes.

## License

This repository is licensed under the MIT License. See [LICENSE.txt](LICENSE.txt).

FMOD is proprietary software by Firelight Technologies Pty Ltd. You must obtain the appropriate FMOD license for your project from [fmod.com](https://www.fmod.com).

## Acknowledgments

- [FMOD](https://www.fmod.com/) by Firelight Technologies Pty Ltd

<p align="center">
  <sub>FMOD is a trademark of Firelight Technologies Pty Ltd.<br>
  This plugin is not officially affiliated with or endorsed by Firelight Technologies.</sub>
</p>

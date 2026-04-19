<p align="center">
  <img alt="fmod_player_banner" src="https://github.com/user-attachments/assets/1d2655f5-d440-4bdb-978a-b095cb275e9c" />
</p>

<h1 align="center">Godot-FmodPlayer</h1>

<p align="center">
  <b>Godot 4 GDExtension for advanced audio playback via FMOD Core API</b>
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
</p>

<p align="center">
  A audio player plugin supporting multiple formats, flexible loading modes,<br>
  professional DSP effects, and real-time mixing capabilities.
</p>

## Features

### Multi-Format Audio Support
Play all major audio formats supported by FMOD:
- MP3, WAV, OGG Vorbis, FLAC
- MOD, XM, S3M, IT (tracker formats)
- MIDI, AIFF, and more

### Flexible Loading Modes
Load audio from virtually any source:
- **File System** - Direct file path loading (`res://`, `user://`, absolute paths)
- **Memory Buffer** - Load from raw byte arrays for procedural/generated audio
- **Godot PCK** - Seamless integration with Godot's resource pack system
- **Streaming** - Memory-efficient playback for large music files

### Professional DSP Effects
Built-in 16+ audio effects for real-time processing:
| Effect | Description |
|--------|-------------|
| **Amplify** | Volume gain control |
| **EQ** | 6/10/21-band equalizer |
| **Filter** | Low-pass, high-pass, band-pass |
| **Chorus** | Stereo chorus effect |
| **Delay** | Echo and delay lines |
| **Distortion** | Overdrive and fuzz |
| **Phaser** | Phase shifting effect |
| **Pitch Shift** | Real-time pitch manipulation |
| **Reverb** | Room simulation |
| **Compressor** | Dynamic range compression |
| **Hard Limiter** | Peak limiting |
| **Stereo Enhance** | Stereo width control |
| **Spectrum Analyzer** | Real-time frequency analysis |
| **Capture** | Audio capture functionality |
| **Record** | Audio recording functionality |
| **Panner** | Stereo positioning |

### Dynamic Mixing & Bus System
Professional-grade audio mixing:
- **Audio Buses** - Organize sounds into mix buses with independent controls
- **Bus Layout** - Synchronized with Godot's AudioServer
- **Channel Groups** - Group channels for collective mixing
- **Real-time Parameters** - Adjust volume, pitch, pan on the fly
- **3D Audio** - 3D positioning with various rolloff modes

### Performance Monitoring
Built-in integration with Godot's Performance Monitor:
- **CPU Usage** - Track FMOD mixer CPU consumption (DSP, Stream, Geometry, Update, Convolution)
- **File Usage** - Track streaming and sample bytes read

### Editor Integration
First-class Godot Editor support:
- **Audio Importer** - Import and configure audio assets directly in the editor
- **Audio Preview** - Preview audio files in the inspector
- **Custom Icons** - Visual distinction for all FMOD-related nodes and resources

## Requirements

- **Godot:** 4.1 or later
- **Platforms:** Windows, Android
- **Compiler:** C++17 (MSVC v145+ on Windows, NDK for Android)
- **FMOD:** Core API libraries (included in `src/thirdparty/fmod/`)

> **Note:** This plugin uses the **FMOD Core API** (low-level), not FMOD Studio API. For Studio API support, use [fmod-gdextension](https://github.com/utopia-rise/fmod-gdextension).

> **License Notice:** FMOD is a proprietary audio engine. You must obtain your own license from [fmod.com](https://www.fmod.com) for commercial use.

#### Available Options
- **Platforms:** `windows`, `android`
- **Targets:** `template_debug`, `template_release`
- **Architectures:** `x86_64`, `arm64`

#### Enable the Plugin
After building, go to `Project > Project Settings > Plugins` in Godot Editor, find **fmod_player** and enable it.

## Documentation

**Full documentation available at:** [Documentation](https://godot-fmodplayerdocs.readthedocs.io/en/latest/)

The documentation includes detailed usage guides, API reference, code examples, and platform-specific instructions.

## Acknowledgments

- [FMOD](https://www.fmod.com/) by Firelight Technologies Pty Ltd

<p align="center">
  <sub>FMOD is a trademark of Firelight Technologies Pty Ltd.<br>
  This plugin is not officially affiliated with or endorsed by Firelight Technologies.</sub>
</p>

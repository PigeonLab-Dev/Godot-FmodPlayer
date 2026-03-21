#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioStreamPlayer2D : Node2D
{

	private new static readonly StringName NativeName = new StringName("FmodAudioStreamPlayer2D");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioStreamPlayer2D object), please use the Instantiate() method instead.")]
	protected FmodAudioStreamPlayer2D() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioStreamPlayer2D"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioStreamPlayer2D"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioStreamPlayer2D"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioStreamPlayer2D"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioStreamPlayer2D Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioStreamPlayer2D wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioStreamPlayer2D);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioStreamPlayer2D).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioStreamPlayer2D)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioStreamPlayer2D"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioStreamPlayer2D" type.</returns>
	public new static FmodAudioStreamPlayer2D Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionSignalName
	{
		public new static readonly StringName Finished = "finished";
	}

	public new delegate void FinishedSignalHandler();
	private FinishedSignalHandler _finishedSignal;
	private Callable _finishedSignalCallable;
	public event FinishedSignalHandler FinishedSignal
	{
		add
		{
			if (_finishedSignal is null)
			{
				_finishedSignalCallable = Callable.From(() => 
					_finishedSignal?.Invoke());
				Connect(GDExtensionSignalName.Finished, _finishedSignalCallable);
			}
			_finishedSignal += value;
		}
		remove
		{
			_finishedSignal -= value;
			if (_finishedSignal is not null) return;
			Disconnect(GDExtensionSignalName.Finished, _finishedSignalCallable);
			_finishedSignalCallable = default;
		}
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Stream = "stream";
		public new static readonly StringName Playing = "playing";
		public new static readonly StringName StreamPaused = "stream_paused";
		public new static readonly StringName VolumeDb = "volume_db";
		public new static readonly StringName Pitch = "pitch";
		public new static readonly StringName AutoPlay = "auto_play";
		public new static readonly StringName Bus = "bus";
		public new static readonly StringName MaxDistance = "max_distance";
		public new static readonly StringName Attenuation = "attenuation";
		public new static readonly StringName PanningStrength = "panning_strength";
		public new static readonly StringName AreaMask = "area_mask";
		public new static readonly StringName MaxPolyphony = "max_polyphony";
	}

	public new FmodAudioStream Stream
	{
		get => FmodAudioStream.Bind(Get(GDExtensionPropertyName.Stream).As<Resource>());
		set => Set(GDExtensionPropertyName.Stream, value);
	}

	public new bool Playing
	{
		get => Get(GDExtensionPropertyName.Playing).As<bool>();
		set => Set(GDExtensionPropertyName.Playing, value);
	}

	public new bool StreamPaused
	{
		get => Get(GDExtensionPropertyName.StreamPaused).As<bool>();
		set => Set(GDExtensionPropertyName.StreamPaused, value);
	}

	public new double VolumeDb
	{
		get => Get(GDExtensionPropertyName.VolumeDb).As<double>();
		set => Set(GDExtensionPropertyName.VolumeDb, value);
	}

	public new double Pitch
	{
		get => Get(GDExtensionPropertyName.Pitch).As<double>();
		set => Set(GDExtensionPropertyName.Pitch, value);
	}

	public new bool AutoPlay
	{
		get => Get(GDExtensionPropertyName.AutoPlay).As<bool>();
		set => Set(GDExtensionPropertyName.AutoPlay, value);
	}

	public new StringName Bus
	{
		get => Get(GDExtensionPropertyName.Bus).As<StringName>();
		set => Set(GDExtensionPropertyName.Bus, value);
	}

	public new double MaxDistance
	{
		get => Get(GDExtensionPropertyName.MaxDistance).As<double>();
		set => Set(GDExtensionPropertyName.MaxDistance, value);
	}

	public new double Attenuation
	{
		get => Get(GDExtensionPropertyName.Attenuation).As<double>();
		set => Set(GDExtensionPropertyName.Attenuation, value);
	}

	public new double PanningStrength
	{
		get => Get(GDExtensionPropertyName.PanningStrength).As<double>();
		set => Set(GDExtensionPropertyName.PanningStrength, value);
	}

	public new long AreaMask
	{
		get => Get(GDExtensionPropertyName.AreaMask).As<long>();
		set => Set(GDExtensionPropertyName.AreaMask, value);
	}

	public new long MaxPolyphony
	{
		get => Get(GDExtensionPropertyName.MaxPolyphony).As<long>();
		set => Set(GDExtensionPropertyName.MaxPolyphony, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName Play = "play";
		public new static readonly StringName Seek = "seek";
		public new static readonly StringName Stop = "stop";
		public new static readonly StringName IsPlaying = "is_playing";
		public new static readonly StringName GetPlaybackPosition = "get_playback_position";
		public new static readonly StringName SetVolumeLinear = "set_volume_linear";
		public new static readonly StringName GetVolumeLinear = "get_volume_linear";
		public new static readonly StringName SetPitchScale = "set_pitch_scale";
		public new static readonly StringName GetPitchScale = "get_pitch_scale";
		public new static readonly StringName IsAutoplayEnabled = "is_autoplay_enabled";
		public new static readonly StringName HasStreamPlayback = "has_stream_playback";
		public new static readonly StringName GetStreamPlayback = "get_stream_playback";
	}

	public new void Play(double fromPosition = 0) => 
		Call(GDExtensionMethodName.Play, [fromPosition]);

	public new void Seek(double toPosition) => 
		Call(GDExtensionMethodName.Seek, [toPosition]);

	public new void Stop() => 
		Call(GDExtensionMethodName.Stop, []);

	public new bool IsPlaying() => 
		Call(GDExtensionMethodName.IsPlaying, []).As<bool>();

	public new double GetPlaybackPosition() => 
		Call(GDExtensionMethodName.GetPlaybackPosition, []).As<double>();

	public new void SetVolumeLinear(double volume) => 
		Call(GDExtensionMethodName.SetVolumeLinear, [volume]);

	public new double GetVolumeLinear() => 
		Call(GDExtensionMethodName.GetVolumeLinear, []).As<double>();

	public new void SetPitchScale(double pitch) => 
		Call(GDExtensionMethodName.SetPitchScale, [pitch]);

	public new double GetPitchScale() => 
		Call(GDExtensionMethodName.GetPitchScale, []).As<double>();

	public new bool IsAutoplayEnabled() => 
		Call(GDExtensionMethodName.IsAutoplayEnabled, []).As<bool>();

	public new bool HasStreamPlayback() => 
		Call(GDExtensionMethodName.HasStreamPlayback, []).As<bool>();

	public new FmodChannel GetStreamPlayback() => 
		FmodChannel.Bind(Call(GDExtensionMethodName.GetStreamPlayback, []).As<RefCounted>());

}

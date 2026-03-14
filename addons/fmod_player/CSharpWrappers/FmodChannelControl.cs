#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodChannelControl : RefCounted
{

	private new static readonly StringName NativeName = new StringName("FmodChannelControl");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodChannelControl object), please use the Instantiate() method instead.")]
	protected FmodChannelControl() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodChannelControl"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodChannelControl"/> wrapper type,
	/// a new instance of the <see cref="FmodChannelControl"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodChannelControl"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodChannelControl Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodChannelControl wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodChannelControl);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodChannelControl).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodChannelControl)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodChannelControl"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodChannelControl" type.</returns>
	public new static FmodChannelControl Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionSignalName
	{
		public new static readonly StringName CallbackReceived = "callback_received";
	}

	public new delegate void CallbackReceivedSignalHandler(long type);
	private CallbackReceivedSignalHandler _callbackReceivedSignal;
	private Callable _callbackReceivedSignalCallable;
	public event CallbackReceivedSignalHandler CallbackReceivedSignal
	{
		add
		{
			if (_callbackReceivedSignal is null)
			{
				_callbackReceivedSignalCallable = Callable.From((Variant type) => 
					_callbackReceivedSignal?.Invoke(type.As<long>()));
				Connect(GDExtensionSignalName.CallbackReceived, _callbackReceivedSignalCallable);
			}
			_callbackReceivedSignal += value;
		}
		remove
		{
			_callbackReceivedSignal -= value;
			if (_callbackReceivedSignal is not null) return;
			Disconnect(GDExtensionSignalName.CallbackReceived, _callbackReceivedSignalCallable);
			_callbackReceivedSignalCallable = default;
		}
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Pasued = "pasued";
		public new static readonly StringName Pitch = "pitch";
		public new static readonly StringName VolumeDb = "volume_db";
		public new static readonly StringName VolumeRamp = "volume_ramp";
		public new static readonly StringName Mute = "mute";
		public new static readonly StringName Arg3dCustomRolloff = "3d_custom_rolloff";
		public new static readonly StringName Arg3dDopplerLevel = "3d_doppler_level";
		public new static readonly StringName Arg3dLevel = "3d_level";
		public new static readonly StringName Arg3dMinDistance = "3d_min_distance";
		public new static readonly StringName Arg3dMaxDistance = "3d_max_distance";
		public new static readonly StringName Arg3dSpread = "3d_spread";
		public new static readonly StringName Gain = "gain";
	}

	public new bool Pasued
	{
		get => Get(GDExtensionPropertyName.Pasued).As<bool>();
		set => Set(GDExtensionPropertyName.Pasued, value);
	}

	public new double Pitch
	{
		get => Get(GDExtensionPropertyName.Pitch).As<double>();
		set => Set(GDExtensionPropertyName.Pitch, value);
	}

	public new double VolumeDb
	{
		get => Get(GDExtensionPropertyName.VolumeDb).As<double>();
		set => Set(GDExtensionPropertyName.VolumeDb, value);
	}

	public new bool VolumeRamp
	{
		get => Get(GDExtensionPropertyName.VolumeRamp).As<bool>();
		set => Set(GDExtensionPropertyName.VolumeRamp, value);
	}

	public new bool Mute
	{
		get => Get(GDExtensionPropertyName.Mute).As<bool>();
		set => Set(GDExtensionPropertyName.Mute, value);
	}

	public new Vector3[] Arg3dCustomRolloff
	{
		get => Get(GDExtensionPropertyName.Arg3dCustomRolloff).As<Vector3[]>();
		set => Set(GDExtensionPropertyName.Arg3dCustomRolloff, value);
	}

	public new double Arg3dDopplerLevel
	{
		get => Get(GDExtensionPropertyName.Arg3dDopplerLevel).As<double>();
		set => Set(GDExtensionPropertyName.Arg3dDopplerLevel, value);
	}

	public new double Arg3dLevel
	{
		get => Get(GDExtensionPropertyName.Arg3dLevel).As<double>();
		set => Set(GDExtensionPropertyName.Arg3dLevel, value);
	}

	public new double Arg3dMinDistance
	{
		get => Get(GDExtensionPropertyName.Arg3dMinDistance).As<double>();
		set => Set(GDExtensionPropertyName.Arg3dMinDistance, value);
	}

	public new double Arg3dMaxDistance
	{
		get => Get(GDExtensionPropertyName.Arg3dMaxDistance).As<double>();
		set => Set(GDExtensionPropertyName.Arg3dMaxDistance, value);
	}

	public new double Arg3dSpread
	{
		get => Get(GDExtensionPropertyName.Arg3dSpread).As<double>();
		set => Set(GDExtensionPropertyName.Arg3dSpread, value);
	}

	public new double Gain
	{
		get => Get(GDExtensionPropertyName.Gain).As<double>();
		set => Set(GDExtensionPropertyName.Gain, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName ChannelControlIsValid = "channel_control_is_valid";
		public new static readonly StringName ChannelControlIsNull = "channel_control_is_null";
		public new static readonly StringName IsPlaying = "is_playing";
		public new static readonly StringName Stop = "stop";
		public new static readonly StringName SetMode = "set_mode";
		public new static readonly StringName GetMode = "get_mode";
		public new static readonly StringName GetAudibility = "get_audibility";
		public new static readonly StringName Set3dAttributes = "set_3d_attributes";
		public new static readonly StringName Get3dAttributes = "get_3d_attributes";
		public new static readonly StringName Set3dConeOrientation = "set_3d_cone_orientation";
		public new static readonly StringName Get3dConeOrientation = "get_3d_cone_orientation";
		public new static readonly StringName Set3dConeSettings = "set_3d_cone_settings";
		public new static readonly StringName Get3dConeSettings = "get_3d_cone_settings";
		public new static readonly StringName Set3dDistanceFilter = "set_3d_distance_filter";
		public new static readonly StringName Get3dDistanceFilter = "get_3d_distance_filter";
		public new static readonly StringName Set3dOcclusion = "set_3d_occlusion";
		public new static readonly StringName Get3dOcclusion = "get_3d_occlusion";
		public new static readonly StringName SetPan = "set_pan";
		public new static readonly StringName SetMixLevelsInput = "set_mix_levels_input";
		public new static readonly StringName SetMixLevelsOutput = "set_mix_levels_output";
		public new static readonly StringName SetReverbProperties = "set_reverb_properties";
		public new static readonly StringName GetReverbProperties = "get_reverb_properties";
		public new static readonly StringName AddDsp = "add_dsp";
		public new static readonly StringName RemoveDsp = "remove_dsp";
		public new static readonly StringName GetNumDsps = "get_num_dsps";
		public new static readonly StringName GetDsp = "get_dsp";
		public new static readonly StringName SetDspIndex = "set_dsp_index";
		public new static readonly StringName GetDspIndex = "get_dsp_index";
		public new static readonly StringName GetDspClock = "get_dsp_clock";
		public new static readonly StringName SetDelay = "set_delay";
		public new static readonly StringName GetDelay = "get_delay";
		public new static readonly StringName AddFadePoint = "add_fade_point";
		public new static readonly StringName SetFadePointRamp = "set_fade_point_ramp";
		public new static readonly StringName RemoveFadePoints = "remove_fade_points";
		public new static readonly StringName GetFadePoints = "get_fade_points";
		public new static readonly StringName SetCallback = "set_callback";
		public new static readonly StringName ClearCallback = "clear_callback";
	}

	public new bool ChannelControlIsValid() => 
		Call(GDExtensionMethodName.ChannelControlIsValid, []).As<bool>();

	public new bool ChannelControlIsNull() => 
		Call(GDExtensionMethodName.ChannelControlIsNull, []).As<bool>();

	public new bool IsPlaying() => 
		Call(GDExtensionMethodName.IsPlaying, []).As<bool>();

	public new void Stop() => 
		Call(GDExtensionMethodName.Stop, []);

	public new void SetMode(FmodSystem.FmodMode mode) => 
		Call(GDExtensionMethodName.SetMode, [Variant.From(mode)]);

	public new FmodSystem.FmodMode GetMode() => 
		Call(GDExtensionMethodName.GetMode, []).As<FmodSystem.FmodMode>();

	public new double GetAudibility() => 
		Call(GDExtensionMethodName.GetAudibility, []).As<double>();

	public new void Set3dAttributes(Vector3 pos, Vector3 vel) => 
		Call(GDExtensionMethodName.Set3dAttributes, [pos, vel]);

	public new Godot.Collections.Dictionary Get3dAttributes() => 
		Call(GDExtensionMethodName.Get3dAttributes, []).As<Godot.Collections.Dictionary>();

	public new void Set3dConeOrientation(Vector3 orientation) => 
		Call(GDExtensionMethodName.Set3dConeOrientation, [orientation]);

	public new Vector3 Get3dConeOrientation() => 
		Call(GDExtensionMethodName.Get3dConeOrientation, []).As<Vector3>();

	public new void Set3dConeSettings(double insideConeAngle, double outsideConeAngle, double outsideVolumeDb) => 
		Call(GDExtensionMethodName.Set3dConeSettings, [insideConeAngle, outsideConeAngle, outsideVolumeDb]);

	public new Godot.Collections.Dictionary Get3dConeSettings() => 
		Call(GDExtensionMethodName.Get3dConeSettings, []).As<Godot.Collections.Dictionary>();

	public new void Set3dDistanceFilter(bool custom, double customLevel, double centerFreq) => 
		Call(GDExtensionMethodName.Set3dDistanceFilter, [custom, customLevel, centerFreq]);

	public new Godot.Collections.Dictionary Get3dDistanceFilter() => 
		Call(GDExtensionMethodName.Get3dDistanceFilter, []).As<Godot.Collections.Dictionary>();

	public new void Set3dOcclusion(double minDistance, double maxDistance) => 
		Call(GDExtensionMethodName.Set3dOcclusion, [minDistance, maxDistance]);

	public new Godot.Collections.Dictionary Get3dOcclusion() => 
		Call(GDExtensionMethodName.Get3dOcclusion, []).As<Godot.Collections.Dictionary>();

	public new void SetPan(double pan) => 
		Call(GDExtensionMethodName.SetPan, [pan]);

	public new void SetMixLevelsInput(float[] levels) => 
		Call(GDExtensionMethodName.SetMixLevelsInput, [levels]);

	public new void SetMixLevelsOutput(double frontLeft, double frontRight, double center, double lfe, double surroundLeft, double surroundRight, double backLeft, double backRight) => 
		Call(GDExtensionMethodName.SetMixLevelsOutput, [frontLeft, frontRight, center, lfe, surroundLeft, surroundRight, backLeft, backRight]);

	public new void SetReverbProperties(long instance, double wet) => 
		Call(GDExtensionMethodName.SetReverbProperties, [instance, wet]);

	public new double GetReverbProperties(long instance) => 
		Call(GDExtensionMethodName.GetReverbProperties, [instance]).As<double>();

	public new void AddDsp(long index, FmodDSP dsp) => 
		Call(GDExtensionMethodName.AddDsp, [index, dsp]);

	public new void RemoveDsp(FmodDSP dsp) => 
		Call(GDExtensionMethodName.RemoveDsp, [dsp]);

	public new long GetNumDsps() => 
		Call(GDExtensionMethodName.GetNumDsps, []).As<long>();

	public new FmodDSP GetDsp(long index) => 
		FmodDSP.Bind(Call(GDExtensionMethodName.GetDsp, [index]).As<RefCounted>());

	public new void SetDspIndex(FmodDSP dsp, long index) => 
		Call(GDExtensionMethodName.SetDspIndex, [dsp, index]);

	public new long GetDspIndex(FmodDSP dsp) => 
		Call(GDExtensionMethodName.GetDspIndex, [dsp]).As<long>();

	public new Godot.Collections.Dictionary GetDspClock() => 
		Call(GDExtensionMethodName.GetDspClock, []).As<Godot.Collections.Dictionary>();

	public new void SetDelay(long start, long end, bool stopChannels = true) => 
		Call(GDExtensionMethodName.SetDelay, [start, end, stopChannels]);

	public new Godot.Collections.Dictionary GetDelay() => 
		Call(GDExtensionMethodName.GetDelay, []).As<Godot.Collections.Dictionary>();

	public new void AddFadePoint(long dspclock, double volume) => 
		Call(GDExtensionMethodName.AddFadePoint, [dspclock, volume]);

	public new void SetFadePointRamp(long dspclock, double volume) => 
		Call(GDExtensionMethodName.SetFadePointRamp, [dspclock, volume]);

	public new void RemoveFadePoints(long start, long end) => 
		Call(GDExtensionMethodName.RemoveFadePoints, [start, end]);

	public new Godot.Collections.Dictionary GetFadePoints() => 
		Call(GDExtensionMethodName.GetFadePoints, []).As<Godot.Collections.Dictionary>();

	public new void SetCallback() => 
		Call(GDExtensionMethodName.SetCallback, []);

	public new void ClearCallback() => 
		Call(GDExtensionMethodName.ClearCallback, []);

}

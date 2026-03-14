#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodDSP : RefCounted
{

	private new static readonly StringName NativeName = new StringName("FmodDSP");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodDSP object), please use the Instantiate() method instead.")]
	protected FmodDSP() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodDSP"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodDSP"/> wrapper type,
	/// a new instance of the <see cref="FmodDSP"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodDSP"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodDSP Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodDSP wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodDSP);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodDSP).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodDSP)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodDSP"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodDSP" type.</returns>
	public new static FmodDSP Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public enum FmodDspType
	{
		Unknown = 0,
		Mixer = 1,
		Oscillator = 2,
		Lowpass = 3,
		Itlowpass = 4,
		Highpass = 5,
		Echo = 6,
		Fader = 7,
		Flange = 8,
		Distortion = 9,
		Normalize = 10,
		Limiter = 11,
		Parameq = 12,
		Pitchshift = 13,
		Chorus = 14,
		Itecho = 15,
		Compressor = 16,
		Sfxreverb = 17,
		LowpassSimple = 18,
		Delay = 19,
		Tremolo = 20,
		Send = 21,
		Return = 22,
		HighpassSimple = 23,
		Pan = 24,
		ThreeEq = 25,
		Fft = 26,
		LoudnessMeter = 27,
		Convolutionreverb = 28,
		Channelmix = 29,
		Transceiver = 30,
		Objectpan = 31,
		MultibandEq = 32,
		MultibandDynamics = 33,
		Max = 34,
		Forceint = 65536,
	}

	public enum FmodDspParameterDataType
	{
		User = 0,
		Overallgain = -1,
		Type3dattributes = -2,
		Sidechain = -3,
		Fft = -4,
		Type3dattributesMulti = -5,
		AttenuationRange = -6,
		DynamicResponse = -7,
		FiniteLength = -8,
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Active = "active";
		public new static readonly StringName Bypass = "bypass";
	}

	public new bool Active
	{
		get => Get(GDExtensionPropertyName.Active).As<bool>();
		set => Set(GDExtensionPropertyName.Active, value);
	}

	public new bool Bypass
	{
		get => Get(GDExtensionPropertyName.Bypass).As<bool>();
		set => Set(GDExtensionPropertyName.Bypass, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName AddInput = "add_input";
		public new static readonly StringName GetInput = "get_input";
		public new static readonly StringName GetOutput = "get_output";
		public new static readonly StringName GetNumInputs = "get_num_inputs";
		public new static readonly StringName GetNumOutputs = "get_num_outputs";
		public new static readonly StringName DisconnectAll = "disconnect_all";
		public new static readonly StringName DisconnectFrom = "disconnect_from";
		public new static readonly StringName SetChannelFormat = "set_channel_format";
		public new static readonly StringName GetChannelFormat = "get_channel_format";
		public new static readonly StringName GetOutputChannelFormat = "get_output_channel_format";
		public new static readonly StringName GetMeteringInfo = "get_metering_info";
		public new static readonly StringName SetMeteringEnabled = "set_metering_enabled";
		public new static readonly StringName GetMeteringEnabled = "get_metering_enabled";
		public new static readonly StringName GetDataParameterIndex = "get_data_parameter_index";
		public new static readonly StringName GetNumParameters = "get_num_parameters";
		public new static readonly StringName SetParameterBool = "set_parameter_bool";
		public new static readonly StringName GetParameterBool = "get_parameter_bool";
		public new static readonly StringName SetParameterData = "set_parameter_data";
		public new static readonly StringName GetParameterData = "get_parameter_data";
		public new static readonly StringName SetParameterFloat = "set_parameter_float";
		public new static readonly StringName GetParameterFloat = "get_parameter_float";
		public new static readonly StringName SetParameterInt = "set_parameter_int";
		public new static readonly StringName GetParameterInt = "get_parameter_int";
		public new static readonly StringName GetParameterInfo = "get_parameter_info";
		public new static readonly StringName SetWetDryMix = "set_wet_dry_mix";
		public new static readonly StringName GetWetDryMix = "get_wet_dry_mix";
		public new static readonly StringName GetIdle = "get_idle";
		public new static readonly StringName Reset = "reset";
		public new static readonly StringName Release = "release";
		public new static readonly StringName GetType = "get_type";
		public new static readonly StringName GetInfo = "get_info";
		public new static readonly StringName GetCpuUsage = "get_cpu_usage";
		public new static readonly StringName GetSystemObject = "get_system_object";
	}

	public new FmodDSPConnection AddInput(FmodDSP targetDsp, long type = 0) => 
		FmodDSPConnection.Bind(Call(GDExtensionMethodName.AddInput, [targetDsp, type]).As<RefCounted>());

	public new Godot.Collections.Dictionary GetInput(long index) => 
		Call(GDExtensionMethodName.GetInput, [index]).As<Godot.Collections.Dictionary>();

	public new Godot.Collections.Dictionary GetOutput(long index) => 
		Call(GDExtensionMethodName.GetOutput, [index]).As<Godot.Collections.Dictionary>();

	public new long GetNumInputs() => 
		Call(GDExtensionMethodName.GetNumInputs, []).As<long>();

	public new long GetNumOutputs() => 
		Call(GDExtensionMethodName.GetNumOutputs, []).As<long>();

	public new void DisconnectAll(bool inputs, bool outputs) => 
		Call(GDExtensionMethodName.DisconnectAll, [inputs, outputs]);

	public new void DisconnectFrom(FmodDSP target, FmodDSPConnection connection = null) => 
		Call(GDExtensionMethodName.DisconnectFrom, [target, connection]);

	public new void SetChannelFormat(long numchannels, FmodSystem.FmodSpeakerMode speakermode) => 
		Call(GDExtensionMethodName.SetChannelFormat, [numchannels, Variant.From(speakermode)]);

	public new Godot.Collections.Dictionary GetChannelFormat() => 
		Call(GDExtensionMethodName.GetChannelFormat, []).As<Godot.Collections.Dictionary>();

	public new Godot.Collections.Dictionary GetOutputChannelFormat(long inchannels, FmodSystem.FmodSpeakerMode inspeakermode) => 
		Call(GDExtensionMethodName.GetOutputChannelFormat, [inchannels, Variant.From(inspeakermode)]).As<Godot.Collections.Dictionary>();

	public new Godot.Collections.Dictionary GetMeteringInfo() => 
		Call(GDExtensionMethodName.GetMeteringInfo, []).As<Godot.Collections.Dictionary>();

	public new void SetMeteringEnabled(bool inputEnabled, bool outputEnabled) => 
		Call(GDExtensionMethodName.SetMeteringEnabled, [inputEnabled, outputEnabled]);

	public new Godot.Collections.Dictionary GetMeteringEnabled() => 
		Call(GDExtensionMethodName.GetMeteringEnabled, []).As<Godot.Collections.Dictionary>();

	public new long GetDataParameterIndex(FmodDSP.FmodDspParameterDataType dataType) => 
		Call(GDExtensionMethodName.GetDataParameterIndex, [Variant.From(dataType)]).As<long>();

	public new long GetNumParameters() => 
		Call(GDExtensionMethodName.GetNumParameters, []).As<long>();

	public new void SetParameterBool(long index, bool value) => 
		Call(GDExtensionMethodName.SetParameterBool, [index, value]);

	public new bool GetParameterBool(long index) => 
		Call(GDExtensionMethodName.GetParameterBool, [index]).As<bool>();

	public new void SetParameterData(long index, byte[] data) => 
		Call(GDExtensionMethodName.SetParameterData, [index, data]);

	public new byte[] GetParameterData(long index) => 
		Call(GDExtensionMethodName.GetParameterData, [index]).As<byte[]>();

	public new void SetParameterFloat(long index, double value) => 
		Call(GDExtensionMethodName.SetParameterFloat, [index, value]);

	public new double GetParameterFloat(long index) => 
		Call(GDExtensionMethodName.GetParameterFloat, [index]).As<double>();

	public new void SetParameterInt(long index, long value) => 
		Call(GDExtensionMethodName.SetParameterInt, [index, value]);

	public new long GetParameterInt(long index) => 
		Call(GDExtensionMethodName.GetParameterInt, [index]).As<long>();

	public new Godot.Collections.Dictionary GetParameterInfo(long index) => 
		Call(GDExtensionMethodName.GetParameterInfo, [index]).As<Godot.Collections.Dictionary>();

	public new void SetWetDryMix(double prewet, double postwet, double dry) => 
		Call(GDExtensionMethodName.SetWetDryMix, [prewet, postwet, dry]);

	public new Godot.Collections.Dictionary GetWetDryMix() => 
		Call(GDExtensionMethodName.GetWetDryMix, []).As<Godot.Collections.Dictionary>();

	public new bool GetIdle() => 
		Call(GDExtensionMethodName.GetIdle, []).As<bool>();

	public new void Reset() => 
		Call(GDExtensionMethodName.Reset, []);

	public new void Release() => 
		Call(GDExtensionMethodName.Release, []);

	public new FmodDSP.FmodDspType GetType() => 
		Call(GDExtensionMethodName.GetType, []).As<FmodDSP.FmodDspType>();

	public new Godot.Collections.Dictionary GetInfo() => 
		Call(GDExtensionMethodName.GetInfo, []).As<Godot.Collections.Dictionary>();

	public new Godot.Collections.Dictionary GetCpuUsage() => 
		Call(GDExtensionMethodName.GetCpuUsage, []).As<Godot.Collections.Dictionary>();

	public new FmodSystem GetSystemObject() => 
		FmodSystem.Bind(Call(GDExtensionMethodName.GetSystemObject, []).As<GodotObject>());

}

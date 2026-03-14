#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectEQ : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectEQ");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectEQ object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectEQ() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectEQ"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectEQ"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectEQ"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectEQ"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectEQ Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectEQ wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectEQ);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectEQ).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectEQ)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectEQ"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectEQ" type.</returns>
	public new static FmodAudioEffectEQ Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public enum PresetEnum
	{
		Preset6Bands = 0,
		Preset10Bands = 1,
		Preset21Bands = 2,
	}

	public enum FilterType
	{
		Disabled = 0,
		Lowpass12db = 1,
		Lowpass24db = 2,
		Lowpass48db = 3,
		Highpass12db = 4,
		Highpass24db = 5,
		Highpass48db = 6,
		Lowshelf = 7,
		Highshelf = 8,
		Peaking = 9,
		Bandpass = 10,
		Notch = 11,
		Allpass = 12,
		Lowpass6db = 13,
		Highpass6db = 14,
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Preset = "preset";
	}

	public new Variant Preset
	{
		get => Get(GDExtensionPropertyName.Preset).As<Variant>();
		set => Set(GDExtensionPropertyName.Preset, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName GetBandCount = "get_band_count";
		public new static readonly StringName SetBandGainDb = "set_band_gain_db";
		public new static readonly StringName GetBandGainDb = "get_band_gain_db";
		public new static readonly StringName SetBandFrequency = "set_band_frequency";
		public new static readonly StringName GetBandFrequency = "get_band_frequency";
		public new static readonly StringName SetBandFilterType = "set_band_filter_type";
		public new static readonly StringName GetBandFilterType = "get_band_filter_type";
		public new static readonly StringName SetBandQ = "set_band_q";
		public new static readonly StringName GetBandQ = "get_band_q";
	}

	public new long GetBandCount() => 
		Call(GDExtensionMethodName.GetBandCount, []).As<long>();

	public new void SetBandGainDb(long bandIdx, double gainDb) => 
		Call(GDExtensionMethodName.SetBandGainDb, [bandIdx, gainDb]);

	public new double GetBandGainDb(long bandIdx) => 
		Call(GDExtensionMethodName.GetBandGainDb, [bandIdx]).As<double>();

	public new void SetBandFrequency(long bandIdx, double frequency) => 
		Call(GDExtensionMethodName.SetBandFrequency, [bandIdx, frequency]);

	public new double GetBandFrequency(long bandIdx) => 
		Call(GDExtensionMethodName.GetBandFrequency, [bandIdx]).As<double>();

	public new void SetBandFilterType(long bandIdx, FmodAudioEffectEQ.FilterType filterType) => 
		Call(GDExtensionMethodName.SetBandFilterType, [bandIdx, Variant.From(filterType)]);

	public new FmodAudioEffectEQ.FilterType GetBandFilterType(long bandIdx) => 
		Call(GDExtensionMethodName.GetBandFilterType, [bandIdx]).As<FmodAudioEffectEQ.FilterType>();

	public new void SetBandQ(long bandIdx, double q) => 
		Call(GDExtensionMethodName.SetBandQ, [bandIdx, q]);

	public new double GetBandQ(long bandIdx) => 
		Call(GDExtensionMethodName.GetBandQ, [bandIdx]).As<double>();

}

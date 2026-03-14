#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectPitchShift : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectPitchShift");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectPitchShift object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectPitchShift() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectPitchShift"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectPitchShift"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectPitchShift"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectPitchShift"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectPitchShift Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectPitchShift wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectPitchShift);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectPitchShift).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectPitchShift)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectPitchShift"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectPitchShift" type.</returns>
	public new static FmodAudioEffectPitchShift Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public enum FftSizeEnum
	{
		Size256 = 0,
		Size512 = 1,
		Size1024 = 2,
		Size2048 = 3,
		Size4096 = 4,
		Max = 5,
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName PitchScale = "pitch_scale";
		public new static readonly StringName FftSize = "fft_size";
		public new static readonly StringName Oversampling = "oversampling";
	}

	public new double PitchScale
	{
		get => Get(GDExtensionPropertyName.PitchScale).As<double>();
		set => Set(GDExtensionPropertyName.PitchScale, value);
	}

	public new Variant FftSize
	{
		get => Get(GDExtensionPropertyName.FftSize).As<Variant>();
		set => Set(GDExtensionPropertyName.FftSize, value);
	}

	public new long Oversampling
	{
		get => Get(GDExtensionPropertyName.Oversampling).As<long>();
		set => Set(GDExtensionPropertyName.Oversampling, value);
	}

}

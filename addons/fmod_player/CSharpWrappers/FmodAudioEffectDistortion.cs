#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectDistortion : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectDistortion");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectDistortion object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectDistortion() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectDistortion"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectDistortion"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectDistortion"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectDistortion"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectDistortion Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectDistortion wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectDistortion);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectDistortion).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectDistortion)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectDistortion"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectDistortion" type.</returns>
	public new static FmodAudioEffectDistortion Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public enum ModeEnum
	{
		Clip = 0,
		Atan = 1,
		Lofi = 2,
		Overdrive = 3,
		Waveshape = 4,
		Bitcrush = 5,
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Mode = "mode";
		public new static readonly StringName PreGain = "pre_gain";
		public new static readonly StringName Drive = "drive";
		public new static readonly StringName PostGain = "post_gain";
		public new static readonly StringName KeepHfHz = "keep_hf_hz";
		public new static readonly StringName Oversample = "oversample";
	}

	public new Variant Mode
	{
		get => Get(GDExtensionPropertyName.Mode).As<Variant>();
		set => Set(GDExtensionPropertyName.Mode, value);
	}

	public new double PreGain
	{
		get => Get(GDExtensionPropertyName.PreGain).As<double>();
		set => Set(GDExtensionPropertyName.PreGain, value);
	}

	public new double Drive
	{
		get => Get(GDExtensionPropertyName.Drive).As<double>();
		set => Set(GDExtensionPropertyName.Drive, value);
	}

	public new double PostGain
	{
		get => Get(GDExtensionPropertyName.PostGain).As<double>();
		set => Set(GDExtensionPropertyName.PostGain, value);
	}

	public new double KeepHfHz
	{
		get => Get(GDExtensionPropertyName.KeepHfHz).As<double>();
		set => Set(GDExtensionPropertyName.KeepHfHz, value);
	}

	public new long Oversample
	{
		get => Get(GDExtensionPropertyName.Oversample).As<long>();
		set => Set(GDExtensionPropertyName.Oversample, value);
	}

}

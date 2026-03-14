#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectCompressor : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectCompressor");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectCompressor object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectCompressor() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectCompressor"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectCompressor"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectCompressor"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectCompressor"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectCompressor Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectCompressor wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectCompressor);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectCompressor).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectCompressor)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectCompressor"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectCompressor" type.</returns>
	public new static FmodAudioEffectCompressor Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName ThresholdDb = "threshold_db";
		public new static readonly StringName Ratio = "ratio";
		public new static readonly StringName GainDb = "gain_db";
		public new static readonly StringName AttackUs = "attack_us";
		public new static readonly StringName ReleaseMs = "release_ms";
		public new static readonly StringName Mix = "mix";
		public new static readonly StringName Sidechain = "sidechain";
	}

	public new double ThresholdDb
	{
		get => Get(GDExtensionPropertyName.ThresholdDb).As<double>();
		set => Set(GDExtensionPropertyName.ThresholdDb, value);
	}

	public new double Ratio
	{
		get => Get(GDExtensionPropertyName.Ratio).As<double>();
		set => Set(GDExtensionPropertyName.Ratio, value);
	}

	public new double GainDb
	{
		get => Get(GDExtensionPropertyName.GainDb).As<double>();
		set => Set(GDExtensionPropertyName.GainDb, value);
	}

	public new double AttackUs
	{
		get => Get(GDExtensionPropertyName.AttackUs).As<double>();
		set => Set(GDExtensionPropertyName.AttackUs, value);
	}

	public new double ReleaseMs
	{
		get => Get(GDExtensionPropertyName.ReleaseMs).As<double>();
		set => Set(GDExtensionPropertyName.ReleaseMs, value);
	}

	public new double Mix
	{
		get => Get(GDExtensionPropertyName.Mix).As<double>();
		set => Set(GDExtensionPropertyName.Mix, value);
	}

	public new StringName/* "Empty Enum Constant String" */ Sidechain
	{
		get => Get(GDExtensionPropertyName.Sidechain).As<StringName/* "Empty Enum Constant String" */>();
		set => Set(GDExtensionPropertyName.Sidechain, value);
	}

}

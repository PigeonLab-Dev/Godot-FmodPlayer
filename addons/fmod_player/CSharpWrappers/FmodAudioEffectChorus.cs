#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectChorus : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectChorus");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectChorus object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectChorus() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectChorus"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectChorus"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectChorus"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectChorus"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectChorus Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectChorus wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectChorus);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectChorus).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectChorus)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectChorus"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectChorus" type.</returns>
	public new static FmodAudioEffectChorus Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName VoiceCount = "voice_count";
		public new static readonly StringName Wet = "wet";
		public new static readonly StringName Dry = "dry";
		public new static readonly StringName Voice1DelayMs = "voice/1/delay_ms";
		public new static readonly StringName Voice2DelayMs = "voice/2/delay_ms";
		public new static readonly StringName Voice3DelayMs = "voice/3/delay_ms";
		public new static readonly StringName Voice4DelayMs = "voice/4/delay_ms";
		public new static readonly StringName Voice1RateHz = "voice/1/rate_hz";
		public new static readonly StringName Voice2RateHz = "voice/2/rate_hz";
		public new static readonly StringName Voice3RateHz = "voice/3/rate_hz";
		public new static readonly StringName Voice4RateHz = "voice/4/rate_hz";
		public new static readonly StringName Voice1DepthMs = "voice/1/depth_ms";
		public new static readonly StringName Voice2DepthMs = "voice/2/depth_ms";
		public new static readonly StringName Voice3DepthMs = "voice/3/depth_ms";
		public new static readonly StringName Voice4DepthMs = "voice/4/depth_ms";
		public new static readonly StringName Voice1LevelDb = "voice/1/level_db";
		public new static readonly StringName Voice2LevelDb = "voice/2/level_db";
		public new static readonly StringName Voice3LevelDb = "voice/3/level_db";
		public new static readonly StringName Voice4LevelDb = "voice/4/level_db";
		public new static readonly StringName Voice1CutoffHz = "voice/1/cutoff_hz";
		public new static readonly StringName Voice2CutoffHz = "voice/2/cutoff_hz";
		public new static readonly StringName Voice3CutoffHz = "voice/3/cutoff_hz";
		public new static readonly StringName Voice4CutoffHz = "voice/4/cutoff_hz";
		public new static readonly StringName Voice1Pan = "voice/1/pan";
		public new static readonly StringName Voice2Pan = "voice/2/pan";
		public new static readonly StringName Voice3Pan = "voice/3/pan";
		public new static readonly StringName Voice4Pan = "voice/4/pan";
	}

	public new long VoiceCount
	{
		get => Get(GDExtensionPropertyName.VoiceCount).As<long>();
		set => Set(GDExtensionPropertyName.VoiceCount, value);
	}

	public new double Wet
	{
		get => Get(GDExtensionPropertyName.Wet).As<double>();
		set => Set(GDExtensionPropertyName.Wet, value);
	}

	public new double Dry
	{
		get => Get(GDExtensionPropertyName.Dry).As<double>();
		set => Set(GDExtensionPropertyName.Dry, value);
	}

	public new double Voice1DelayMs
	{
		get => Get(GDExtensionPropertyName.Voice1DelayMs).As<double>();
		set => Set(GDExtensionPropertyName.Voice1DelayMs, value);
	}

	public new double Voice2DelayMs
	{
		get => Get(GDExtensionPropertyName.Voice2DelayMs).As<double>();
		set => Set(GDExtensionPropertyName.Voice2DelayMs, value);
	}

	public new double Voice3DelayMs
	{
		get => Get(GDExtensionPropertyName.Voice3DelayMs).As<double>();
		set => Set(GDExtensionPropertyName.Voice3DelayMs, value);
	}

	public new double Voice4DelayMs
	{
		get => Get(GDExtensionPropertyName.Voice4DelayMs).As<double>();
		set => Set(GDExtensionPropertyName.Voice4DelayMs, value);
	}

	public new double Voice1RateHz
	{
		get => Get(GDExtensionPropertyName.Voice1RateHz).As<double>();
		set => Set(GDExtensionPropertyName.Voice1RateHz, value);
	}

	public new double Voice2RateHz
	{
		get => Get(GDExtensionPropertyName.Voice2RateHz).As<double>();
		set => Set(GDExtensionPropertyName.Voice2RateHz, value);
	}

	public new double Voice3RateHz
	{
		get => Get(GDExtensionPropertyName.Voice3RateHz).As<double>();
		set => Set(GDExtensionPropertyName.Voice3RateHz, value);
	}

	public new double Voice4RateHz
	{
		get => Get(GDExtensionPropertyName.Voice4RateHz).As<double>();
		set => Set(GDExtensionPropertyName.Voice4RateHz, value);
	}

	public new double Voice1DepthMs
	{
		get => Get(GDExtensionPropertyName.Voice1DepthMs).As<double>();
		set => Set(GDExtensionPropertyName.Voice1DepthMs, value);
	}

	public new double Voice2DepthMs
	{
		get => Get(GDExtensionPropertyName.Voice2DepthMs).As<double>();
		set => Set(GDExtensionPropertyName.Voice2DepthMs, value);
	}

	public new double Voice3DepthMs
	{
		get => Get(GDExtensionPropertyName.Voice3DepthMs).As<double>();
		set => Set(GDExtensionPropertyName.Voice3DepthMs, value);
	}

	public new double Voice4DepthMs
	{
		get => Get(GDExtensionPropertyName.Voice4DepthMs).As<double>();
		set => Set(GDExtensionPropertyName.Voice4DepthMs, value);
	}

	public new double Voice1LevelDb
	{
		get => Get(GDExtensionPropertyName.Voice1LevelDb).As<double>();
		set => Set(GDExtensionPropertyName.Voice1LevelDb, value);
	}

	public new double Voice2LevelDb
	{
		get => Get(GDExtensionPropertyName.Voice2LevelDb).As<double>();
		set => Set(GDExtensionPropertyName.Voice2LevelDb, value);
	}

	public new double Voice3LevelDb
	{
		get => Get(GDExtensionPropertyName.Voice3LevelDb).As<double>();
		set => Set(GDExtensionPropertyName.Voice3LevelDb, value);
	}

	public new double Voice4LevelDb
	{
		get => Get(GDExtensionPropertyName.Voice4LevelDb).As<double>();
		set => Set(GDExtensionPropertyName.Voice4LevelDb, value);
	}

	public new double Voice1CutoffHz
	{
		get => Get(GDExtensionPropertyName.Voice1CutoffHz).As<double>();
		set => Set(GDExtensionPropertyName.Voice1CutoffHz, value);
	}

	public new double Voice2CutoffHz
	{
		get => Get(GDExtensionPropertyName.Voice2CutoffHz).As<double>();
		set => Set(GDExtensionPropertyName.Voice2CutoffHz, value);
	}

	public new double Voice3CutoffHz
	{
		get => Get(GDExtensionPropertyName.Voice3CutoffHz).As<double>();
		set => Set(GDExtensionPropertyName.Voice3CutoffHz, value);
	}

	public new double Voice4CutoffHz
	{
		get => Get(GDExtensionPropertyName.Voice4CutoffHz).As<double>();
		set => Set(GDExtensionPropertyName.Voice4CutoffHz, value);
	}

	public new double Voice1Pan
	{
		get => Get(GDExtensionPropertyName.Voice1Pan).As<double>();
		set => Set(GDExtensionPropertyName.Voice1Pan, value);
	}

	public new double Voice2Pan
	{
		get => Get(GDExtensionPropertyName.Voice2Pan).As<double>();
		set => Set(GDExtensionPropertyName.Voice2Pan, value);
	}

	public new double Voice3Pan
	{
		get => Get(GDExtensionPropertyName.Voice3Pan).As<double>();
		set => Set(GDExtensionPropertyName.Voice3Pan, value);
	}

	public new double Voice4Pan
	{
		get => Get(GDExtensionPropertyName.Voice4Pan).As<double>();
		set => Set(GDExtensionPropertyName.Voice4Pan, value);
	}

}

#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectDelay : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectDelay");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectDelay object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectDelay() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectDelay"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectDelay"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectDelay"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectDelay"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectDelay Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectDelay wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectDelay);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectDelay).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectDelay)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectDelay"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectDelay" type.</returns>
	public new static FmodAudioEffectDelay Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Dry = "dry";
		public new static readonly StringName Tap1Active = "tap1_active";
		public new static readonly StringName Tap1DelayMs = "tap1_delay_ms";
		public new static readonly StringName Tap1LevelDb = "tap1_level_db";
		public new static readonly StringName Tap1Pan = "tap1_pan";
		public new static readonly StringName Tap2Active = "tap2_active";
		public new static readonly StringName Tap2DelayMs = "tap2_delay_ms";
		public new static readonly StringName Tap2LevelDb = "tap2_level_db";
		public new static readonly StringName Tap2Pan = "tap2_pan";
		public new static readonly StringName FeedbackActive = "feedback_active";
		public new static readonly StringName FeedbackDelayMs = "feedback_delay_ms";
		public new static readonly StringName FeedbackLevelDb = "feedback_level_db";
		public new static readonly StringName FeedbackLowpass = "feedback_lowpass";
	}

	public new double Dry
	{
		get => Get(GDExtensionPropertyName.Dry).As<double>();
		set => Set(GDExtensionPropertyName.Dry, value);
	}

	public new bool Tap1Active
	{
		get => Get(GDExtensionPropertyName.Tap1Active).As<bool>();
		set => Set(GDExtensionPropertyName.Tap1Active, value);
	}

	public new double Tap1DelayMs
	{
		get => Get(GDExtensionPropertyName.Tap1DelayMs).As<double>();
		set => Set(GDExtensionPropertyName.Tap1DelayMs, value);
	}

	public new double Tap1LevelDb
	{
		get => Get(GDExtensionPropertyName.Tap1LevelDb).As<double>();
		set => Set(GDExtensionPropertyName.Tap1LevelDb, value);
	}

	public new double Tap1Pan
	{
		get => Get(GDExtensionPropertyName.Tap1Pan).As<double>();
		set => Set(GDExtensionPropertyName.Tap1Pan, value);
	}

	public new bool Tap2Active
	{
		get => Get(GDExtensionPropertyName.Tap2Active).As<bool>();
		set => Set(GDExtensionPropertyName.Tap2Active, value);
	}

	public new double Tap2DelayMs
	{
		get => Get(GDExtensionPropertyName.Tap2DelayMs).As<double>();
		set => Set(GDExtensionPropertyName.Tap2DelayMs, value);
	}

	public new double Tap2LevelDb
	{
		get => Get(GDExtensionPropertyName.Tap2LevelDb).As<double>();
		set => Set(GDExtensionPropertyName.Tap2LevelDb, value);
	}

	public new double Tap2Pan
	{
		get => Get(GDExtensionPropertyName.Tap2Pan).As<double>();
		set => Set(GDExtensionPropertyName.Tap2Pan, value);
	}

	public new bool FeedbackActive
	{
		get => Get(GDExtensionPropertyName.FeedbackActive).As<bool>();
		set => Set(GDExtensionPropertyName.FeedbackActive, value);
	}

	public new double FeedbackDelayMs
	{
		get => Get(GDExtensionPropertyName.FeedbackDelayMs).As<double>();
		set => Set(GDExtensionPropertyName.FeedbackDelayMs, value);
	}

	public new double FeedbackLevelDb
	{
		get => Get(GDExtensionPropertyName.FeedbackLevelDb).As<double>();
		set => Set(GDExtensionPropertyName.FeedbackLevelDb, value);
	}

	public new double FeedbackLowpass
	{
		get => Get(GDExtensionPropertyName.FeedbackLowpass).As<double>();
		set => Set(GDExtensionPropertyName.FeedbackLowpass, value);
	}

}

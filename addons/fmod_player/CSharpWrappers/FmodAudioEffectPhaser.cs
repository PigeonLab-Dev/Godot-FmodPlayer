#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectPhaser : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectPhaser");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectPhaser object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectPhaser() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectPhaser"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectPhaser"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectPhaser"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectPhaser"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectPhaser Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectPhaser wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectPhaser);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectPhaser).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectPhaser)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectPhaser"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectPhaser" type.</returns>
	public new static FmodAudioEffectPhaser Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Depth = "depth";
		public new static readonly StringName Feedback = "feedback";
		public new static readonly StringName RangeMaxHz = "range_max_hz";
		public new static readonly StringName RangeMinHz = "range_min_hz";
		public new static readonly StringName RateHz = "rate_hz";
	}

	public new double Depth
	{
		get => Get(GDExtensionPropertyName.Depth).As<double>();
		set => Set(GDExtensionPropertyName.Depth, value);
	}

	public new double Feedback
	{
		get => Get(GDExtensionPropertyName.Feedback).As<double>();
		set => Set(GDExtensionPropertyName.Feedback, value);
	}

	public new double RangeMaxHz
	{
		get => Get(GDExtensionPropertyName.RangeMaxHz).As<double>();
		set => Set(GDExtensionPropertyName.RangeMaxHz, value);
	}

	public new double RangeMinHz
	{
		get => Get(GDExtensionPropertyName.RangeMinHz).As<double>();
		set => Set(GDExtensionPropertyName.RangeMinHz, value);
	}

	public new double RateHz
	{
		get => Get(GDExtensionPropertyName.RateHz).As<double>();
		set => Set(GDExtensionPropertyName.RateHz, value);
	}

}

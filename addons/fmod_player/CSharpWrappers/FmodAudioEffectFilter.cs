#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectFilter : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectFilter");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectFilter object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectFilter() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectFilter"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectFilter"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectFilter"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectFilter"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectFilter Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectFilter wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectFilter);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectFilter).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectFilter)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectFilter"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectFilter" type.</returns>
	public new static FmodAudioEffectFilter Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public enum FilterDb
	{
		Filter6db = 0,
		Filter12db = 1,
		Filter18db = 2,
		Filter24db = 3,
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName CutoffHz = "cutoff_hz";
		public new static readonly StringName Db = "db";
		public new static readonly StringName Gain = "gain";
		public new static readonly StringName Resonance = "resonance";
	}

	public new double CutoffHz
	{
		get => Get(GDExtensionPropertyName.CutoffHz).As<double>();
		set => Set(GDExtensionPropertyName.CutoffHz, value);
	}

	public new Variant Db
	{
		get => Get(GDExtensionPropertyName.Db).As<Variant>();
		set => Set(GDExtensionPropertyName.Db, value);
	}

	public new double Gain
	{
		get => Get(GDExtensionPropertyName.Gain).As<double>();
		set => Set(GDExtensionPropertyName.Gain, value);
	}

	public new double Resonance
	{
		get => Get(GDExtensionPropertyName.Resonance).As<double>();
		set => Set(GDExtensionPropertyName.Resonance, value);
	}

}

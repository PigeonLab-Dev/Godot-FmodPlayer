#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectHardLimiter : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectHardLimiter");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectHardLimiter object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectHardLimiter() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectHardLimiter"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectHardLimiter"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectHardLimiter"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectHardLimiter"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectHardLimiter Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectHardLimiter wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectHardLimiter);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectHardLimiter).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectHardLimiter)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectHardLimiter"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectHardLimiter" type.</returns>
	public new static FmodAudioEffectHardLimiter Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName CeilingDb = "ceiling_db";
		public new static readonly StringName PreGainDb = "pre_gain_db";
		public new static readonly StringName Release = "release";
	}

	public new double CeilingDb
	{
		get => Get(GDExtensionPropertyName.CeilingDb).As<double>();
		set => Set(GDExtensionPropertyName.CeilingDb, value);
	}

	public new double PreGainDb
	{
		get => Get(GDExtensionPropertyName.PreGainDb).As<double>();
		set => Set(GDExtensionPropertyName.PreGainDb, value);
	}

	public new double Release
	{
		get => Get(GDExtensionPropertyName.Release).As<double>();
		set => Set(GDExtensionPropertyName.Release, value);
	}

}

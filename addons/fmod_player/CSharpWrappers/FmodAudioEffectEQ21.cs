#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectEQ21 : FmodAudioEffectEQ
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectEQ21");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectEQ21 object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectEQ21() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectEQ21"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectEQ21"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectEQ21"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectEQ21"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectEQ21 Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectEQ21 wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectEQ21);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectEQ21).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectEQ21)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectEQ21"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectEQ21" type.</returns>
	public new static FmodAudioEffectEQ21 Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

}

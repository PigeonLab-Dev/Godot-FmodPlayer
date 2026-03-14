#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public abstract partial class FmodAudioEffect : Resource
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffect");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffect object), please use the Instantiate() method instead.")]
	protected FmodAudioEffect() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffect"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffect"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffect"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffect"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffect Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffect wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffect);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffect).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffect)InstanceFromId(instanceId);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName ApplyTo = "apply_to";
		public new static readonly StringName RemoveFromBus = "remove_from_bus";
		public new static readonly StringName GetBus = "get_bus";
	}

	public new void ApplyTo(FmodChannelGroup bus) => 
		Call(GDExtensionMethodName.ApplyTo, [bus]);

	public new void RemoveFromBus(FmodChannelGroup bus) => 
		Call(GDExtensionMethodName.RemoveFromBus, [bus]);

	public new FmodChannelGroup GetBus() => 
		FmodChannelGroup.Bind(Call(GDExtensionMethodName.GetBus, []).As<RefCounted>());

}

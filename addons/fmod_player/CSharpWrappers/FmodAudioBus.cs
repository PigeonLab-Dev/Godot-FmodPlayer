#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioBus : RefCounted
{

	private new static readonly StringName NativeName = new StringName("FmodAudioBus");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioBus object), please use the Instantiate() method instead.")]
	protected FmodAudioBus() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioBus"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioBus"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioBus"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioBus"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioBus Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioBus wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioBus);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioBus).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioBus)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioBus"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioBus" type.</returns>
	public new static FmodAudioBus Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Volume = "volume";
		public new static readonly StringName Mute = "mute";
		public new static readonly StringName Solo = "solo";
		public new static readonly StringName Bypass = "bypass";
	}

	public new double Volume
	{
		get => Get(GDExtensionPropertyName.Volume).As<double>();
		set => Set(GDExtensionPropertyName.Volume, value);
	}

	public new bool Mute
	{
		get => Get(GDExtensionPropertyName.Mute).As<bool>();
		set => Set(GDExtensionPropertyName.Mute, value);
	}

	public new bool Solo
	{
		get => Get(GDExtensionPropertyName.Solo).As<bool>();
		set => Set(GDExtensionPropertyName.Solo, value);
	}

	public new bool Bypass
	{
		get => Get(GDExtensionPropertyName.Bypass).As<bool>();
		set => Set(GDExtensionPropertyName.Bypass, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName InitBus = "init_bus";
		public new static readonly StringName SetParent = "set_parent";
		public new static readonly StringName GetBus = "get_bus";
		public new static readonly StringName GetParent = "get_parent";
		public new static readonly StringName ApplyMute = "apply_mute";
		public new static readonly StringName AddEffect = "add_effect";
		public new static readonly StringName RemoveEffect = "remove_effect";
		public new static readonly StringName GetEffect = "get_effect";
	}

	public new void InitBus(string name, FmodChannelGroup parent = null) => 
		Call(GDExtensionMethodName.InitBus, [name, parent]);

	public new void SetParent(FmodChannelGroup parent) => 
		Call(GDExtensionMethodName.SetParent, [parent]);

	public new FmodChannelGroup GetBus() => 
		FmodChannelGroup.Bind(Call(GDExtensionMethodName.GetBus, []).As<RefCounted>());

	public new FmodChannelGroup GetParent() => 
		FmodChannelGroup.Bind(Call(GDExtensionMethodName.GetParent, []).As<RefCounted>());

	public new void ApplyMute(bool unnamedArg0) => 
		Call(GDExtensionMethodName.ApplyMute, [unnamedArg0]);

	public new void AddEffect(FmodAudioEffect effect, long index) => 
		Call(GDExtensionMethodName.AddEffect, [effect, index]);

	public new void RemoveEffect(long index) => 
		Call(GDExtensionMethodName.RemoveEffect, [index]);

	public new FmodAudioEffect GetEffect(long index) => 
		FmodAudioEffect.Bind(Call(GDExtensionMethodName.GetEffect, [index]).As<Resource>());

}

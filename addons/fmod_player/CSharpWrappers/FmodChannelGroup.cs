#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodChannelGroup : FmodChannelControl
{

	private new static readonly StringName NativeName = new StringName("FmodChannelGroup");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodChannelGroup object), please use the Instantiate() method instead.")]
	protected FmodChannelGroup() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodChannelGroup"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodChannelGroup"/> wrapper type,
	/// a new instance of the <see cref="FmodChannelGroup"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodChannelGroup"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodChannelGroup Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodChannelGroup wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodChannelGroup);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodChannelGroup).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodChannelGroup)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodChannelGroup"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodChannelGroup" type.</returns>
	public new static FmodChannelGroup Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName ChannelGroupIsValid = "channel_group_is_valid";
		public new static readonly StringName ChannelGroupIsNull = "channel_group_is_null";
		public new static readonly StringName GetNumChannels = "get_num_channels";
		public new static readonly StringName GetChannel = "get_channel";
		public new static readonly StringName AddGroup = "add_group";
		public new static readonly StringName GetNumGroups = "get_num_groups";
		public new static readonly StringName GetGroup = "get_group";
		public new static readonly StringName GetParentGroup = "get_parent_group";
		public new static readonly StringName GetName = "get_name";
		public new static readonly StringName Release = "release";
	}

	public new bool ChannelGroupIsValid() => 
		Call(GDExtensionMethodName.ChannelGroupIsValid, []).As<bool>();

	public new bool ChannelGroupIsNull() => 
		Call(GDExtensionMethodName.ChannelGroupIsNull, []).As<bool>();

	public new long GetNumChannels() => 
		Call(GDExtensionMethodName.GetNumChannels, []).As<long>();

	public new FmodChannel GetChannel(long index) => 
		FmodChannel.Bind(Call(GDExtensionMethodName.GetChannel, [index]).As<RefCounted>());

	public new void AddGroup(FmodChannelGroup childGroup, bool propagatedspclock = true) => 
		Call(GDExtensionMethodName.AddGroup, [childGroup, propagatedspclock]);

	public new long GetNumGroups() => 
		Call(GDExtensionMethodName.GetNumGroups, []).As<long>();

	public new FmodChannelGroup GetGroup(long index) => 
		FmodChannelGroup.Bind(Call(GDExtensionMethodName.GetGroup, [index]).As<RefCounted>());

	public new FmodChannelGroup GetParentGroup() => 
		FmodChannelGroup.Bind(Call(GDExtensionMethodName.GetParentGroup, []).As<RefCounted>());

	public new string GetName() => 
		Call(GDExtensionMethodName.GetName, []).As<string>();

	public new void Release() => 
		Call(GDExtensionMethodName.Release, []);

}

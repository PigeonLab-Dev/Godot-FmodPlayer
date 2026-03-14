#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodServer : GodotObject
{

	private new static readonly StringName NativeName = new StringName("FmodServer");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodServer object), please use the Instantiate() method instead.")]
	protected FmodServer() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodServer"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodServer"/> wrapper type,
	/// a new instance of the <see cref="FmodServer"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodServer"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodServer Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodServer wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodServer);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodServer).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodServer)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodServer"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodServer" type.</returns>
	public new static FmodServer Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName GetSingleton = "get_singleton";
		public new static readonly StringName GetMainSystem = "get_main_system";
		public new static readonly StringName GetMasterChannelGroup = "get_master_channel_group";
		public new static readonly StringName GetAudioBusLayout = "get_audio_bus_layout";
	}

	public new static FmodServer GetSingleton() => 
		FmodServer.Bind(ClassDB.ClassCallStatic(NativeName, GDExtensionMethodName.GetSingleton, []).As<GodotObject>());

	public new static FmodSystem GetMainSystem() => 
		FmodSystem.Bind(ClassDB.ClassCallStatic(NativeName, GDExtensionMethodName.GetMainSystem, []).As<GodotObject>());

	public new static FmodChannelGroup GetMasterChannelGroup() => 
		FmodChannelGroup.Bind(ClassDB.ClassCallStatic(NativeName, GDExtensionMethodName.GetMasterChannelGroup, []).As<RefCounted>());

	public new static FmodAudioBusLayout GetAudioBusLayout() => 
		FmodAudioBusLayout.Bind(ClassDB.ClassCallStatic(NativeName, GDExtensionMethodName.GetAudioBusLayout, []).As<Resource>());

}

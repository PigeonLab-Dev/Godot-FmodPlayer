#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioBusLayout : Resource
{

	private new static readonly StringName NativeName = new StringName("FmodAudioBusLayout");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioBusLayout object), please use the Instantiate() method instead.")]
	protected FmodAudioBusLayout() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioBusLayout"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioBusLayout"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioBusLayout"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioBusLayout"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioBusLayout Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioBusLayout wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioBusLayout);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioBusLayout).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioBusLayout)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioBusLayout"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioBusLayout" type.</returns>
	public new static FmodAudioBusLayout Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName CreateAudioBus = "create_audio_bus";
		public new static readonly StringName GetAudioBus = "get_audio_bus";
		public new static readonly StringName HasAudioBus = "has_audio_bus";
		public new static readonly StringName RemoveAudioBus = "remove_audio_bus";
		public new static readonly StringName SetBusVolume = "set_bus_volume";
		public new static readonly StringName GetBusVolume = "get_bus_volume";
		public new static readonly StringName SetBusSolo = "set_bus_solo";
		public new static readonly StringName BusIsSolo = "bus_is_solo";
		public new static readonly StringName SetBusMute = "set_bus_mute";
		public new static readonly StringName BusIsMute = "bus_is_mute";
		public new static readonly StringName SetBusBypass = "set_bus_bypass";
		public new static readonly StringName BusIsBypass = "bus_is_bypass";
		public new static readonly StringName AddBusEffect = "add_bus_effect";
		public new static readonly StringName RemoveBusEffect = "remove_bus_effect";
		public new static readonly StringName GetBusEffect = "get_bus_effect";
		public new static readonly StringName SyncFromAudioServer = "sync_from_audio_server";
	}

	public new void CreateAudioBus(string name, FmodAudioBus parent = null) => 
		Call(GDExtensionMethodName.CreateAudioBus, [name, parent]);

	public new FmodAudioBus GetAudioBus(string name) => 
		FmodAudioBus.Bind(Call(GDExtensionMethodName.GetAudioBus, [name]).As<RefCounted>());

	public new bool HasAudioBus(string name) => 
		Call(GDExtensionMethodName.HasAudioBus, [name]).As<bool>();

	public new void RemoveAudioBus(string name) => 
		Call(GDExtensionMethodName.RemoveAudioBus, [name]);

	public new void SetBusVolume(string name, double volumeDb) => 
		Call(GDExtensionMethodName.SetBusVolume, [name, volumeDb]);

	public new double GetBusVolume(string name) => 
		Call(GDExtensionMethodName.GetBusVolume, [name]).As<double>();

	public new void SetBusSolo(string name, bool mute) => 
		Call(GDExtensionMethodName.SetBusSolo, [name, mute]);

	public new bool BusIsSolo(string name) => 
		Call(GDExtensionMethodName.BusIsSolo, [name]).As<bool>();

	public new void SetBusMute(string name, bool mute) => 
		Call(GDExtensionMethodName.SetBusMute, [name, mute]);

	public new bool BusIsMute(string name) => 
		Call(GDExtensionMethodName.BusIsMute, [name]).As<bool>();

	public new void SetBusBypass(string name, bool mute) => 
		Call(GDExtensionMethodName.SetBusBypass, [name, mute]);

	public new bool BusIsBypass(string name) => 
		Call(GDExtensionMethodName.BusIsBypass, [name]).As<bool>();

	public new void AddBusEffect(string busName, FmodAudioEffect effect, long index = 0) => 
		Call(GDExtensionMethodName.AddBusEffect, [busName, effect, index]);

	public new void RemoveBusEffect(string busName, long index) => 
		Call(GDExtensionMethodName.RemoveBusEffect, [busName, index]);

	public new FmodAudioEffect GetBusEffect(string busName, long index) => 
		FmodAudioEffect.Bind(Call(GDExtensionMethodName.GetBusEffect, [busName, index]).As<Resource>());

	public new void SyncFromAudioServer() => 
		Call(GDExtensionMethodName.SyncFromAudioServer, []);

}

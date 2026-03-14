#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodChannel : FmodChannelControl
{

	private new static readonly StringName NativeName = new StringName("FmodChannel");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodChannel object), please use the Instantiate() method instead.")]
	protected FmodChannel() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodChannel"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodChannel"/> wrapper type,
	/// a new instance of the <see cref="FmodChannel"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodChannel"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodChannel Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodChannel wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodChannel);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodChannel).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodChannel)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodChannel"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodChannel" type.</returns>
	public new static FmodChannel Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionSignalName
	{
		public new static readonly StringName Ended = "ended";
		public new static readonly StringName Virtualvoice = "virtualvoice";
		public new static readonly StringName Syncpoint = "syncpoint";
		public new static readonly StringName Occlusion = "occlusion";
	}

	public new delegate void EndedSignalHandler();
	private EndedSignalHandler _endedSignal;
	private Callable _endedSignalCallable;
	public event EndedSignalHandler EndedSignal
	{
		add
		{
			if (_endedSignal is null)
			{
				_endedSignalCallable = Callable.From(() => 
					_endedSignal?.Invoke());
				Connect(GDExtensionSignalName.Ended, _endedSignalCallable);
			}
			_endedSignal += value;
		}
		remove
		{
			_endedSignal -= value;
			if (_endedSignal is not null) return;
			Disconnect(GDExtensionSignalName.Ended, _endedSignalCallable);
			_endedSignalCallable = default;
		}
	}

	public new delegate void VirtualvoiceSignalHandler(bool isBecomingVirtual);
	private VirtualvoiceSignalHandler _virtualvoiceSignal;
	private Callable _virtualvoiceSignalCallable;
	public event VirtualvoiceSignalHandler VirtualvoiceSignal
	{
		add
		{
			if (_virtualvoiceSignal is null)
			{
				_virtualvoiceSignalCallable = Callable.From((Variant isBecomingVirtual) => 
					_virtualvoiceSignal?.Invoke(isBecomingVirtual.As<bool>()));
				Connect(GDExtensionSignalName.Virtualvoice, _virtualvoiceSignalCallable);
			}
			_virtualvoiceSignal += value;
		}
		remove
		{
			_virtualvoiceSignal -= value;
			if (_virtualvoiceSignal is not null) return;
			Disconnect(GDExtensionSignalName.Virtualvoice, _virtualvoiceSignalCallable);
			_virtualvoiceSignalCallable = default;
		}
	}

	public new delegate void SyncpointSignalHandler(long syncPointIndex);
	private SyncpointSignalHandler _syncpointSignal;
	private Callable _syncpointSignalCallable;
	public event SyncpointSignalHandler SyncpointSignal
	{
		add
		{
			if (_syncpointSignal is null)
			{
				_syncpointSignalCallable = Callable.From((Variant syncPointIndex) => 
					_syncpointSignal?.Invoke(syncPointIndex.As<long>()));
				Connect(GDExtensionSignalName.Syncpoint, _syncpointSignalCallable);
			}
			_syncpointSignal += value;
		}
		remove
		{
			_syncpointSignal -= value;
			if (_syncpointSignal is not null) return;
			Disconnect(GDExtensionSignalName.Syncpoint, _syncpointSignalCallable);
			_syncpointSignalCallable = default;
		}
	}

	public new delegate void OcclusionSignalHandler(double directOcclusion, double reverbOcclusion);
	private OcclusionSignalHandler _occlusionSignal;
	private Callable _occlusionSignalCallable;
	public event OcclusionSignalHandler OcclusionSignal
	{
		add
		{
			if (_occlusionSignal is null)
			{
				_occlusionSignalCallable = Callable.From((Variant directOcclusion, Variant reverbOcclusion) => 
					_occlusionSignal?.Invoke(directOcclusion.As<double>(), reverbOcclusion.As<double>()));
				Connect(GDExtensionSignalName.Occlusion, _occlusionSignalCallable);
			}
			_occlusionSignal += value;
		}
		remove
		{
			_occlusionSignal -= value;
			if (_occlusionSignal is not null) return;
			Disconnect(GDExtensionSignalName.Occlusion, _occlusionSignalCallable);
			_occlusionSignalCallable = default;
		}
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Frequency = "frequency";
		public new static readonly StringName Priority = "priority";
		public new static readonly StringName ChannelGroup = "channel_group";
		public new static readonly StringName LoopCount = "loop_count";
	}

	public new double Frequency
	{
		get => Get(GDExtensionPropertyName.Frequency).As<double>();
		set => Set(GDExtensionPropertyName.Frequency, value);
	}

	public new long Priority
	{
		get => Get(GDExtensionPropertyName.Priority).As<long>();
		set => Set(GDExtensionPropertyName.Priority, value);
	}

	public new FmodChannelGroup ChannelGroup
	{
		get => FmodChannelGroup.Bind(Get(GDExtensionPropertyName.ChannelGroup).As<RefCounted>());
		set => Set(GDExtensionPropertyName.ChannelGroup, value);
	}

	public new long LoopCount
	{
		get => Get(GDExtensionPropertyName.LoopCount).As<long>();
		set => Set(GDExtensionPropertyName.LoopCount, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName ChannelIsValid = "channel_is_valid";
		public new static readonly StringName ChannelIsNull = "channel_is_null";
		public new static readonly StringName SetPosition = "set_position";
		public new static readonly StringName GetPosition = "get_position";
		public new static readonly StringName SetLoopPoints = "set_loop_points";
		public new static readonly StringName GetLoopPoints = "get_loop_points";
		public new static readonly StringName IsVirtual = "is_virtual";
		public new static readonly StringName GetCurrentSound = "get_current_sound";
		public new static readonly StringName GetIndex = "get_index";
	}

	public new bool ChannelIsValid() => 
		Call(GDExtensionMethodName.ChannelIsValid, []).As<bool>();

	public new bool ChannelIsNull() => 
		Call(GDExtensionMethodName.ChannelIsNull, []).As<bool>();

	public new void SetPosition(long position, FmodSystem.FmodTimeUnit timeunit = FmodSystem.FmodTimeUnit.Ms) => 
		Call(GDExtensionMethodName.SetPosition, [position, Variant.From(timeunit)]);

	public new long GetPosition(FmodSystem.FmodTimeUnit timeunit = FmodSystem.FmodTimeUnit.Ms) => 
		Call(GDExtensionMethodName.GetPosition, [Variant.From(timeunit)]).As<long>();

	public new void SetLoopPoints(long start, long end, FmodSystem.FmodTimeUnit timeunit = FmodSystem.FmodTimeUnit.Ms) => 
		Call(GDExtensionMethodName.SetLoopPoints, [start, end, Variant.From(timeunit)]);

	public new Godot.Collections.Dictionary GetLoopPoints(FmodSystem.FmodTimeUnit unnamedArg0) => 
		Call(GDExtensionMethodName.GetLoopPoints, [Variant.From(unnamedArg0)]).As<Godot.Collections.Dictionary>();

	public new bool IsVirtual() => 
		Call(GDExtensionMethodName.IsVirtual, []).As<bool>();

	public new FmodSound GetCurrentSound() => 
		FmodSound.Bind(Call(GDExtensionMethodName.GetCurrentSound, []).As<RefCounted>());

	public new long GetIndex() => 
		Call(GDExtensionMethodName.GetIndex, []).As<long>();

}

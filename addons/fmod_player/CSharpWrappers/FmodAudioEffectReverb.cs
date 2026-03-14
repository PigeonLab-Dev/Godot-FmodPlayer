#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectReverb : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectReverb");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectReverb object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectReverb() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectReverb"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectReverb"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectReverb"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectReverb"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectReverb Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectReverb wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectReverb);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectReverb).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectReverb)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectReverb"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectReverb" type.</returns>
	public new static FmodAudioEffectReverb Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Damping = "damping";
		public new static readonly StringName Dry = "dry";
		public new static readonly StringName Hipass = "hipass";
		public new static readonly StringName PredelayFeedback = "predelay_feedback";
		public new static readonly StringName PredelayMsec = "predelay_msec";
		public new static readonly StringName RoomSize = "room_size";
		public new static readonly StringName Spread = "spread";
		public new static readonly StringName Wet = "wet";
	}

	public new double Damping
	{
		get => Get(GDExtensionPropertyName.Damping).As<double>();
		set => Set(GDExtensionPropertyName.Damping, value);
	}

	public new double Dry
	{
		get => Get(GDExtensionPropertyName.Dry).As<double>();
		set => Set(GDExtensionPropertyName.Dry, value);
	}

	public new double Hipass
	{
		get => Get(GDExtensionPropertyName.Hipass).As<double>();
		set => Set(GDExtensionPropertyName.Hipass, value);
	}

	public new double PredelayFeedback
	{
		get => Get(GDExtensionPropertyName.PredelayFeedback).As<double>();
		set => Set(GDExtensionPropertyName.PredelayFeedback, value);
	}

	public new double PredelayMsec
	{
		get => Get(GDExtensionPropertyName.PredelayMsec).As<double>();
		set => Set(GDExtensionPropertyName.PredelayMsec, value);
	}

	public new double RoomSize
	{
		get => Get(GDExtensionPropertyName.RoomSize).As<double>();
		set => Set(GDExtensionPropertyName.RoomSize, value);
	}

	public new double Spread
	{
		get => Get(GDExtensionPropertyName.Spread).As<double>();
		set => Set(GDExtensionPropertyName.Spread, value);
	}

	public new double Wet
	{
		get => Get(GDExtensionPropertyName.Wet).As<double>();
		set => Set(GDExtensionPropertyName.Wet, value);
	}

}

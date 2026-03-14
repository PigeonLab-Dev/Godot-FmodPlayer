#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodSoundGroup : RefCounted
{

	private new static readonly StringName NativeName = new StringName("FmodSoundGroup");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodSoundGroup object), please use the Instantiate() method instead.")]
	protected FmodSoundGroup() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodSoundGroup"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodSoundGroup"/> wrapper type,
	/// a new instance of the <see cref="FmodSoundGroup"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodSoundGroup"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodSoundGroup Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodSoundGroup wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodSoundGroup);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodSoundGroup).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodSoundGroup)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodSoundGroup"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodSoundGroup" type.</returns>
	public new static FmodSoundGroup Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public enum BehaviorEnum
	{
		Fail = 0,
		Mute = 1,
		StealLowest = 2,
		Max = 3,
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName MaxAudible = "max_audible";
		public new static readonly StringName Behavior = "behavior";
		public new static readonly StringName MuteFadeSpeed = "mute_fade_speed";
		public new static readonly StringName VolumeDb = "volume_db";
	}

	public new long MaxAudible
	{
		get => Get(GDExtensionPropertyName.MaxAudible).As<long>();
		set => Set(GDExtensionPropertyName.MaxAudible, value);
	}

	public new FmodSoundGroup.BehaviorEnum Behavior
	{
		get => Get(GDExtensionPropertyName.Behavior).As<FmodSoundGroup.BehaviorEnum>();
		set => Set(GDExtensionPropertyName.Behavior, Variant.From(value));
	}

	public new double MuteFadeSpeed
	{
		get => Get(GDExtensionPropertyName.MuteFadeSpeed).As<double>();
		set => Set(GDExtensionPropertyName.MuteFadeSpeed, value);
	}

	public new double VolumeDb
	{
		get => Get(GDExtensionPropertyName.VolumeDb).As<double>();
		set => Set(GDExtensionPropertyName.VolumeDb, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName SoundGroupIsValid = "sound_group_is_valid";
		public new static readonly StringName SoundGroupIsNull = "sound_group_is_null";
		public new static readonly StringName GetNumSounds = "get_num_sounds";
		public new static readonly StringName GetSound = "get_sound";
		public new static readonly StringName GetNumPlaying = "get_num_playing";
		public new static readonly StringName Stop = "stop";
		public new static readonly StringName GetName = "get_name";
		public new static readonly StringName Release = "release";
	}

	public new bool SoundGroupIsValid() => 
		Call(GDExtensionMethodName.SoundGroupIsValid, []).As<bool>();

	public new bool SoundGroupIsNull() => 
		Call(GDExtensionMethodName.SoundGroupIsNull, []).As<bool>();

	public new long GetNumSounds() => 
		Call(GDExtensionMethodName.GetNumSounds, []).As<long>();

	public new FmodSound GetSound(long index) => 
		FmodSound.Bind(Call(GDExtensionMethodName.GetSound, [index]).As<RefCounted>());

	public new long GetNumPlaying() => 
		Call(GDExtensionMethodName.GetNumPlaying, []).As<long>();

	public new void Stop() => 
		Call(GDExtensionMethodName.Stop, []);

	public new string GetName() => 
		Call(GDExtensionMethodName.GetName, []).As<string>();

	public new void Release() => 
		Call(GDExtensionMethodName.Release, []);

}

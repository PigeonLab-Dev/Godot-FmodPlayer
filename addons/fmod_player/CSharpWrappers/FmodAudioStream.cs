#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioStream : Resource
{

	private new static readonly StringName NativeName = new StringName("FmodAudioStream");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioStream object), please use the Instantiate() method instead.")]
	protected FmodAudioStream() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioStream"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioStream"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioStream"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioStream"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioStream Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioStream wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioStream);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioStream).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioStream)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioStream"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioStream" type.</returns>
	public new static FmodAudioStream Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public enum CreateMode
	{
		Default = 0,
		Stream = 1,
		Sample = 2,
		Loop = 4,
		LoopBidi = 8,
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName AudioData = "audio_data";
		public new static readonly StringName ModeFlags = "mode_flags";
	}

	public new byte[] AudioData
	{
		get => Get(GDExtensionPropertyName.AudioData).As<byte[]>();
		set => Set(GDExtensionPropertyName.AudioData, value);
	}

	public new long ModeFlags
	{
		get => Get(GDExtensionPropertyName.ModeFlags).As<long>();
		set => Set(GDExtensionPropertyName.ModeFlags, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName GetSound = "get_sound";
		public new static readonly StringName GetLength = "get_length";
		public new static readonly StringName IsDataLoaded = "is_data_loaded";
		public new static readonly StringName Clear = "clear";
		public new static readonly StringName LoadFromFile = "load_from_file";
	}

	public new FmodSound GetSound() => 
		FmodSound.Bind(Call(GDExtensionMethodName.GetSound, []).As<RefCounted>());

	public new double GetLength() => 
		Call(GDExtensionMethodName.GetLength, []).As<double>();

	public new bool IsDataLoaded() => 
		Call(GDExtensionMethodName.IsDataLoaded, []).As<bool>();

	public new void Clear() => 
		Call(GDExtensionMethodName.Clear, []);

	public new static FmodAudioStream LoadFromFile(string path, long flags = 1) => 
		FmodAudioStream.Bind(ClassDB.ClassCallStatic(NativeName, GDExtensionMethodName.LoadFromFile, [path, flags]).As<Resource>());

}

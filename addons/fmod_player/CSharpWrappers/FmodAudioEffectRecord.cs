#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectRecord : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectRecord");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectRecord object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectRecord() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectRecord"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectRecord"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectRecord"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectRecord"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectRecord Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectRecord wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectRecord);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectRecord).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectRecord)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectRecord"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectRecord" type.</returns>
	public new static FmodAudioEffectRecord Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public enum FormatEnum
	{
		Format8Bits = 0,
		Format16Bits = 1,
		ImaAdpcm = 2,
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Format = "format";
		public new static readonly StringName RecordingActive = "recording_active";
	}

	public new Variant Format
	{
		get => Get(GDExtensionPropertyName.Format).As<Variant>();
		set => Set(GDExtensionPropertyName.Format, value);
	}

	public new bool RecordingActive
	{
		get => Get(GDExtensionPropertyName.RecordingActive).As<bool>();
		set => Set(GDExtensionPropertyName.RecordingActive, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName GetRecording = "get_recording";
	}

	public new AudioStreamWav GetRecording() => 
		Call(GDExtensionMethodName.GetRecording, []).As<AudioStreamWav>();

}

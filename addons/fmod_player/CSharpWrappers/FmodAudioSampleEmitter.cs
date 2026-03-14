#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioSampleEmitter : Node
{

	private new static readonly StringName NativeName = new StringName("FmodAudioSampleEmitter");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioSampleEmitter object), please use the Instantiate() method instead.")]
	protected FmodAudioSampleEmitter() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioSampleEmitter"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioSampleEmitter"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioSampleEmitter"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioSampleEmitter"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioSampleEmitter Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioSampleEmitter wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioSampleEmitter);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioSampleEmitter).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioSampleEmitter)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioSampleEmitter"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioSampleEmitter" type.</returns>
	public new static FmodAudioSampleEmitter Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Stream = "stream";
		public new static readonly StringName AutoEmit = "auto_emit";
	}

	public new FmodAudioStream Stream
	{
		get => FmodAudioStream.Bind(Get(GDExtensionPropertyName.Stream).As<Resource>());
		set => Set(GDExtensionPropertyName.Stream, value);
	}

	public new bool AutoEmit
	{
		get => Get(GDExtensionPropertyName.AutoEmit).As<bool>();
		set => Set(GDExtensionPropertyName.AutoEmit, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName Emit = "emit";
	}

	public new void Emit() => 
		Call(GDExtensionMethodName.Emit, []);

}

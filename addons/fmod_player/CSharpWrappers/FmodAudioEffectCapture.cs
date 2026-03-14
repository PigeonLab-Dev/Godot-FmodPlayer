#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectCapture : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectCapture");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectCapture object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectCapture() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectCapture"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectCapture"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectCapture"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectCapture"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectCapture Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectCapture wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectCapture);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectCapture).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectCapture)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectCapture"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectCapture" type.</returns>
	public new static FmodAudioEffectCapture Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName BufferLength = "buffer_length";
	}

	public new double BufferLength
	{
		get => Get(GDExtensionPropertyName.BufferLength).As<double>();
		set => Set(GDExtensionPropertyName.BufferLength, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName CanGetBuffer = "can_get_buffer";
		public new static readonly StringName ClearBuffer = "clear_buffer";
		public new static readonly StringName GetBuffer = "get_buffer";
		public new static readonly StringName GetDiscardedFrames = "get_discarded_frames";
		public new static readonly StringName GetFramesAvailable = "get_frames_available";
		public new static readonly StringName GetPushedFrames = "get_pushed_frames";
	}

	public new bool CanGetBuffer(long frames) => 
		Call(GDExtensionMethodName.CanGetBuffer, [frames]).As<bool>();

	public new void ClearBuffer() => 
		Call(GDExtensionMethodName.ClearBuffer, []);

	public new Vector2[] GetBuffer(long frames) => 
		Call(GDExtensionMethodName.GetBuffer, [frames]).As<Vector2[]>();

	public new long GetDiscardedFrames() => 
		Call(GDExtensionMethodName.GetDiscardedFrames, []).As<long>();

	public new long GetFramesAvailable() => 
		Call(GDExtensionMethodName.GetFramesAvailable, []).As<long>();

	public new long GetPushedFrames() => 
		Call(GDExtensionMethodName.GetPushedFrames, []).As<long>();

}

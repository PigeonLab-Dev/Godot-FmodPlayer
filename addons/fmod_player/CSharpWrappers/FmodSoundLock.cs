#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodSoundLock : RefCounted
{

	private new static readonly StringName NativeName = new StringName("FmodSoundLock");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodSoundLock object), please use the Instantiate() method instead.")]
	protected FmodSoundLock() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodSoundLock"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodSoundLock"/> wrapper type,
	/// a new instance of the <see cref="FmodSoundLock"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodSoundLock"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodSoundLock Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodSoundLock wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodSoundLock);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodSoundLock).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodSoundLock)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodSoundLock"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodSoundLock" type.</returns>
	public new static FmodSoundLock Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName Unlock = "unlock";
		public new static readonly StringName GetData1 = "get_data1";
		public new static readonly StringName GetData2 = "get_data2";
		public new static readonly StringName IsLocked = "is_locked";
	}

	public new void Unlock() => 
		Call(GDExtensionMethodName.Unlock, []);

	public new byte[] GetData1() => 
		Call(GDExtensionMethodName.GetData1, []).As<byte[]>();

	public new byte[] GetData2() => 
		Call(GDExtensionMethodName.GetData2, []).As<byte[]>();

	public new bool IsLocked() => 
		Call(GDExtensionMethodName.IsLocked, []).As<bool>();

}

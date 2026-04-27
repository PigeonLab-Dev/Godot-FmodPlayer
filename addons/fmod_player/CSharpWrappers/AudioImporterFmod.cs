#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;

namespace FmodPlayer;

[Tool]
public partial class AudioImporterFmod : EditorImportPlugin
{
	private new static readonly StringName NativeName = new StringName("AudioImporterFmod");
	protected AudioImporterFmod() { }
	private static CSharpScript _wrapperScriptAsset;
	public new static AudioImporterFmod Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject)) return null;
		if (godotObject is AudioImporterFmod wrapperScriptInstance) return wrapperScriptInstance;
#if DEBUG
		var expectedType = typeof(AudioImporterFmod);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif
		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(AudioImporterFmod).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}
		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (AudioImporterFmod)InstanceFromId(instanceId);
	}
	public new static AudioImporterFmod Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());
}

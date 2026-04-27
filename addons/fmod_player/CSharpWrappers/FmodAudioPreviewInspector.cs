#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioPreviewInspector : EditorInspectorPlugin
{
	private new static readonly StringName NativeName = new StringName("FmodAudioPreviewInspector");
	protected FmodAudioPreviewInspector() { }
	private static CSharpScript _wrapperScriptAsset;
	public new static FmodAudioPreviewInspector Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject)) return null;
		if (godotObject is FmodAudioPreviewInspector wrapperScriptInstance) return wrapperScriptInstance;
		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioPreviewInspector).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}
		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioPreviewInspector)InstanceFromId(instanceId);
	}
	public new static FmodAudioPreviewInspector Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());
}

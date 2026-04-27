#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioPreviewProperty : ColorRect
{
	private new static readonly StringName NativeName = new StringName("FmodAudioPreviewProperty");
	protected FmodAudioPreviewProperty() { }
	private static CSharpScript _wrapperScriptAsset;
	public new static FmodAudioPreviewProperty Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject)) return null;
		if (godotObject is FmodAudioPreviewProperty wrapperScriptInstance) return wrapperScriptInstance;
		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioPreviewProperty).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}
		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioPreviewProperty)InstanceFromId(instanceId);
	}
	public new static FmodAudioPreviewProperty Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());
}

#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;

namespace FmodPlayer;

[Tool]
public partial class FmodReverbZone3DGizmoPlugin : EditorNode3DGizmoPlugin
{
	private new static readonly StringName NativeName = new StringName("FmodReverbZone3DGizmoPlugin");
	protected FmodReverbZone3DGizmoPlugin() { }
	private static CSharpScript _wrapperScriptAsset;
	public new static FmodReverbZone3DGizmoPlugin Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject)) return null;
		if (godotObject is FmodReverbZone3DGizmoPlugin wrapperScriptInstance) return wrapperScriptInstance;
		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodReverbZone3DGizmoPlugin).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}
		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodReverbZone3DGizmoPlugin)InstanceFromId(instanceId);
	}
	public new static FmodReverbZone3DGizmoPlugin Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());
}

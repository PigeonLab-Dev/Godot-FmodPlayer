#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;

namespace FmodPlayer;

[Tool]
public partial class FmodGeometryMeshToolsPlugin : EditorPlugin
{
	private new static readonly StringName NativeName = new StringName("FmodGeometryMeshToolsPlugin");
	protected FmodGeometryMeshToolsPlugin() { }
	private static CSharpScript _wrapperScriptAsset;
	public new static FmodGeometryMeshToolsPlugin Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject)) return null;
		if (godotObject is FmodGeometryMeshToolsPlugin wrapperScriptInstance) return wrapperScriptInstance;
		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodGeometryMeshToolsPlugin).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}
		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodGeometryMeshToolsPlugin)InstanceFromId(instanceId);
	}
	public new static FmodGeometryMeshToolsPlugin Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());
}

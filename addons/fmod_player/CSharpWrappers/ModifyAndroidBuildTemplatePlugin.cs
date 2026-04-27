#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;

namespace FmodPlayer;

[Tool]
public partial class ModifyAndroidBuildTemplatePlugin : EditorPlugin
{
	private new static readonly StringName NativeName = new StringName("ModifyAndroidBuildTemplatePlugin");
	protected ModifyAndroidBuildTemplatePlugin() { }
	private static CSharpScript _wrapperScriptAsset;
	public new static ModifyAndroidBuildTemplatePlugin Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject)) return null;
		if (godotObject is ModifyAndroidBuildTemplatePlugin wrapperScriptInstance) return wrapperScriptInstance;
		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(ModifyAndroidBuildTemplatePlugin).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}
		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (ModifyAndroidBuildTemplatePlugin)InstanceFromId(instanceId);
	}
	public new static ModifyAndroidBuildTemplatePlugin Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());
}

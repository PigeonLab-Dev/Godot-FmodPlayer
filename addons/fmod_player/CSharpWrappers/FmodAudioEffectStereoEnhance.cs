#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioEffectStereoEnhance : FmodAudioEffect
{

	private new static readonly StringName NativeName = new StringName("FmodAudioEffectStereoEnhance");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioEffectStereoEnhance object), please use the Instantiate() method instead.")]
	protected FmodAudioEffectStereoEnhance() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodAudioEffectStereoEnhance"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodAudioEffectStereoEnhance"/> wrapper type,
	/// a new instance of the <see cref="FmodAudioEffectStereoEnhance"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodAudioEffectStereoEnhance"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodAudioEffectStereoEnhance Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioEffectStereoEnhance wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioEffectStereoEnhance);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioEffectStereoEnhance).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioEffectStereoEnhance)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodAudioEffectStereoEnhance"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodAudioEffectStereoEnhance" type.</returns>
	public new static FmodAudioEffectStereoEnhance Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName PanPullout = "pan_pullout";
		public new static readonly StringName TimePulloutMs = "time_pullout_ms";
		public new static readonly StringName Surround = "surround";
	}

	public new double PanPullout
	{
		get => Get(GDExtensionPropertyName.PanPullout).As<double>();
		set => Set(GDExtensionPropertyName.PanPullout, value);
	}

	public new double TimePulloutMs
	{
		get => Get(GDExtensionPropertyName.TimePulloutMs).As<double>();
		set => Set(GDExtensionPropertyName.TimePulloutMs, value);
	}

	public new double Surround
	{
		get => Get(GDExtensionPropertyName.Surround).As<double>();
		set => Set(GDExtensionPropertyName.Surround, value);
	}

}

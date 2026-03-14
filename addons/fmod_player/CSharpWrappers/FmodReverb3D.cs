#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodReverb3D : RefCounted
{

	private new static readonly StringName NativeName = new StringName("FmodReverb3D");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodReverb3D object), please use the Instantiate() method instead.")]
	protected FmodReverb3D() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodReverb3D"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodReverb3D"/> wrapper type,
	/// a new instance of the <see cref="FmodReverb3D"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodReverb3D"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodReverb3D Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodReverb3D wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodReverb3D);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodReverb3D).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodReverb3D)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodReverb3D"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodReverb3D" type.</returns>
	public new static FmodReverb3D Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Active = "active";
		public new static readonly StringName DecayTime = "decay_time";
		public new static readonly StringName EarlyDelay = "early_delay";
		public new static readonly StringName LateDelay = "late_delay";
		public new static readonly StringName HfReference = "hf_reference";
		public new static readonly StringName HfDecayRatio = "hf_decay_ratio";
		public new static readonly StringName Diffusion = "diffusion";
		public new static readonly StringName Density = "density";
		public new static readonly StringName LowShelfFrequency = "low_shelf_frequency";
		public new static readonly StringName LowShelfGain = "low_shelf_gain";
		public new static readonly StringName HighCut = "high_cut";
		public new static readonly StringName EarlyLateMix = "early_late_mix";
		public new static readonly StringName WetLevel = "wet_level";
	}

	public new bool Active
	{
		get => Get(GDExtensionPropertyName.Active).As<bool>();
		set => Set(GDExtensionPropertyName.Active, value);
	}

	public new double DecayTime
	{
		get => Get(GDExtensionPropertyName.DecayTime).As<double>();
		set => Set(GDExtensionPropertyName.DecayTime, value);
	}

	public new double EarlyDelay
	{
		get => Get(GDExtensionPropertyName.EarlyDelay).As<double>();
		set => Set(GDExtensionPropertyName.EarlyDelay, value);
	}

	public new double LateDelay
	{
		get => Get(GDExtensionPropertyName.LateDelay).As<double>();
		set => Set(GDExtensionPropertyName.LateDelay, value);
	}

	public new double HfReference
	{
		get => Get(GDExtensionPropertyName.HfReference).As<double>();
		set => Set(GDExtensionPropertyName.HfReference, value);
	}

	public new double HfDecayRatio
	{
		get => Get(GDExtensionPropertyName.HfDecayRatio).As<double>();
		set => Set(GDExtensionPropertyName.HfDecayRatio, value);
	}

	public new double Diffusion
	{
		get => Get(GDExtensionPropertyName.Diffusion).As<double>();
		set => Set(GDExtensionPropertyName.Diffusion, value);
	}

	public new double Density
	{
		get => Get(GDExtensionPropertyName.Density).As<double>();
		set => Set(GDExtensionPropertyName.Density, value);
	}

	public new double LowShelfFrequency
	{
		get => Get(GDExtensionPropertyName.LowShelfFrequency).As<double>();
		set => Set(GDExtensionPropertyName.LowShelfFrequency, value);
	}

	public new double LowShelfGain
	{
		get => Get(GDExtensionPropertyName.LowShelfGain).As<double>();
		set => Set(GDExtensionPropertyName.LowShelfGain, value);
	}

	public new double HighCut
	{
		get => Get(GDExtensionPropertyName.HighCut).As<double>();
		set => Set(GDExtensionPropertyName.HighCut, value);
	}

	public new double EarlyLateMix
	{
		get => Get(GDExtensionPropertyName.EarlyLateMix).As<double>();
		set => Set(GDExtensionPropertyName.EarlyLateMix, value);
	}

	public new double WetLevel
	{
		get => Get(GDExtensionPropertyName.WetLevel).As<double>();
		set => Set(GDExtensionPropertyName.WetLevel, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName Reverb3dIsValid = "reverb_3d_is_valid";
		public new static readonly StringName Reverb3dIsNull = "reverb_3d_is_null";
		public new static readonly StringName Set3dAttributes = "set_3d_attributes";
		public new static readonly StringName Get3dAttributes = "get_3d_attributes";
		public new static readonly StringName Release = "release";
	}

	public new bool Reverb3dIsValid() => 
		Call(GDExtensionMethodName.Reverb3dIsValid, []).As<bool>();

	public new bool Reverb3dIsNull() => 
		Call(GDExtensionMethodName.Reverb3dIsNull, []).As<bool>();

	public new void Set3dAttributes(Vector3 position, double minDistance, double maxDistance) => 
		Call(GDExtensionMethodName.Set3dAttributes, [position, minDistance, maxDistance]);

	public new Godot.Collections.Dictionary Get3dAttributes() => 
		Call(GDExtensionMethodName.Get3dAttributes, []).As<Godot.Collections.Dictionary>();

	public new void Release() => 
		Call(GDExtensionMethodName.Release, []);

}

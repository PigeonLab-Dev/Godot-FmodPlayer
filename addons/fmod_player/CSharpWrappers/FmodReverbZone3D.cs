#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;

namespace FmodPlayer;

[Tool]
public partial class FmodReverbZone3D : Node3D
{
	private new static readonly StringName NativeName = new StringName("FmodReverbZone3D");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodReverbZone3D object), please use the Instantiate() method instead.")]
	protected FmodReverbZone3D() { }

	private static CSharpScript _wrapperScriptAsset;

	public new static FmodReverbZone3D Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;
		if (godotObject is FmodReverbZone3D wrapperScriptInstance)
			return wrapperScriptInstance;
#if DEBUG
		var expectedType = typeof(FmodReverbZone3D);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif
		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodReverbZone3D).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}
		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodReverbZone3D)InstanceFromId(instanceId);
	}

	public new static FmodReverbZone3D Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public enum Preset
	{
		Custom = 0, Off = 1, Generic = 2, PaddedCell = 3, Room = 4, Bathroom = 5, LivingRoom = 6,
		StoneRoom = 7, Auditorium = 8, ConcertHall = 9, Cave = 10, Arena = 11, Hangar = 12,
		Hallway = 13, StoneCorridor = 14, Alley = 15, Forest = 16, City = 17, Mountains = 18, Underwater = 19,
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Active = "active";
		public new static readonly StringName SyncTransform = "sync_transform";
		public new static readonly StringName MinDistance = "min_distance";
		public new static readonly StringName MaxDistance = "max_distance";
		public new static readonly StringName Preset = "preset";
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

	public new bool Active { get => Get(GDExtensionPropertyName.Active).As<bool>(); set => Set(GDExtensionPropertyName.Active, value); }
	public new bool SyncTransform { get => Get(GDExtensionPropertyName.SyncTransform).As<bool>(); set => Set(GDExtensionPropertyName.SyncTransform, value); }
	public new double MinDistance { get => Get(GDExtensionPropertyName.MinDistance).As<double>(); set => Set(GDExtensionPropertyName.MinDistance, value); }
	public new double MaxDistance { get => Get(GDExtensionPropertyName.MaxDistance).As<double>(); set => Set(GDExtensionPropertyName.MaxDistance, value); }
	public new Preset ReverbPreset { get => Get(GDExtensionPropertyName.Preset).As<Preset>(); set => Set(GDExtensionPropertyName.Preset, Variant.From(value)); }
	public new double DecayTime { get => Get(GDExtensionPropertyName.DecayTime).As<double>(); set => Set(GDExtensionPropertyName.DecayTime, value); }
	public new double EarlyDelay { get => Get(GDExtensionPropertyName.EarlyDelay).As<double>(); set => Set(GDExtensionPropertyName.EarlyDelay, value); }
	public new double LateDelay { get => Get(GDExtensionPropertyName.LateDelay).As<double>(); set => Set(GDExtensionPropertyName.LateDelay, value); }
	public new double HfReference { get => Get(GDExtensionPropertyName.HfReference).As<double>(); set => Set(GDExtensionPropertyName.HfReference, value); }
	public new double HfDecayRatio { get => Get(GDExtensionPropertyName.HfDecayRatio).As<double>(); set => Set(GDExtensionPropertyName.HfDecayRatio, value); }
	public new double Diffusion { get => Get(GDExtensionPropertyName.Diffusion).As<double>(); set => Set(GDExtensionPropertyName.Diffusion, value); }
	public new double Density { get => Get(GDExtensionPropertyName.Density).As<double>(); set => Set(GDExtensionPropertyName.Density, value); }
	public new double LowShelfFrequency { get => Get(GDExtensionPropertyName.LowShelfFrequency).As<double>(); set => Set(GDExtensionPropertyName.LowShelfFrequency, value); }
	public new double LowShelfGain { get => Get(GDExtensionPropertyName.LowShelfGain).As<double>(); set => Set(GDExtensionPropertyName.LowShelfGain, value); }
	public new double HighCut { get => Get(GDExtensionPropertyName.HighCut).As<double>(); set => Set(GDExtensionPropertyName.HighCut, value); }
	public new double EarlyLateMix { get => Get(GDExtensionPropertyName.EarlyLateMix).As<double>(); set => Set(GDExtensionPropertyName.EarlyLateMix, value); }
	public new double WetLevel { get => Get(GDExtensionPropertyName.WetLevel).As<double>(); set => Set(GDExtensionPropertyName.WetLevel, value); }

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName GetReverb = "get_reverb";
	}

	public new FmodReverb3D GetReverb() => FmodReverb3D.Bind(Call(GDExtensionMethodName.GetReverb, []).As<RefCounted>());
}

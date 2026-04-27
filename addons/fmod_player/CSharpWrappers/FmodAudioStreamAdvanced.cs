#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodAudioStreamAdvanced : FmodAudioStream
{
	private new static readonly StringName NativeName = new StringName("FmodAudioStreamAdvanced");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodAudioStreamAdvanced object), please use the Instantiate() method instead.")]
	protected FmodAudioStreamAdvanced() { }

	private static CSharpScript _wrapperScriptAsset;

	public new static FmodAudioStreamAdvanced Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodAudioStreamAdvanced wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodAudioStreamAdvanced);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodAudioStreamAdvanced).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodAudioStreamAdvanced)InstanceFromId(instanceId);
	}

	public new static FmodAudioStreamAdvanced Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName LengthOverride = "length_override";
		public new static readonly StringName FileOffset = "file_offset";
		public new static readonly StringName DecodeBufferSize = "decode_buffer_size";
		public new static readonly StringName InitialSubsound = "initial_subsound";
		public new static readonly StringName NumSubsounds = "num_subsounds";
		public new static readonly StringName InclusionList = "inclusion_list";
		public new static readonly StringName SuggestedSoundType = "suggested_sound_type";
		public new static readonly StringName DefaultFrequency = "default_frequency";
		public new static readonly StringName NumChannels = "num_channels";
		public new static readonly StringName Format = "format";
		public new static readonly StringName DlsName = "dls_name";
		public new static readonly StringName EncryptionKey = "encryption_key";
		public new static readonly StringName MaxPolyphony = "max_polyphony";
		public new static readonly StringName InitialSeekPosition = "initial_seek_position";
		public new static readonly StringName InitialSeekPostype = "initial_seek_postype";
		public new static readonly StringName IgnoreSetFileSystem = "ignore_set_file_system";
	}

	public new long LengthOverride { get => Get(GDExtensionPropertyName.LengthOverride).As<long>(); set => Set(GDExtensionPropertyName.LengthOverride, value); }
	public new long FileOffset { get => Get(GDExtensionPropertyName.FileOffset).As<long>(); set => Set(GDExtensionPropertyName.FileOffset, value); }
	public new long DecodeBufferSize { get => Get(GDExtensionPropertyName.DecodeBufferSize).As<long>(); set => Set(GDExtensionPropertyName.DecodeBufferSize, value); }
	public new long InitialSubsound { get => Get(GDExtensionPropertyName.InitialSubsound).As<long>(); set => Set(GDExtensionPropertyName.InitialSubsound, value); }
	public new long NumSubsounds { get => Get(GDExtensionPropertyName.NumSubsounds).As<long>(); set => Set(GDExtensionPropertyName.NumSubsounds, value); }
	public new int[] InclusionList { get => Get(GDExtensionPropertyName.InclusionList).As<int[]>(); set => Set(GDExtensionPropertyName.InclusionList, value); }
	public new FmodSound.FmodSoundType SuggestedSoundType { get => Get(GDExtensionPropertyName.SuggestedSoundType).As<FmodSound.FmodSoundType>(); set => Set(GDExtensionPropertyName.SuggestedSoundType, Variant.From(value)); }
	public new long DefaultFrequency { get => Get(GDExtensionPropertyName.DefaultFrequency).As<long>(); set => Set(GDExtensionPropertyName.DefaultFrequency, value); }
	public new long NumChannels { get => Get(GDExtensionPropertyName.NumChannels).As<long>(); set => Set(GDExtensionPropertyName.NumChannels, value); }
	public new FmodSound.FmodSoundFormat Format { get => Get(GDExtensionPropertyName.Format).As<FmodSound.FmodSoundFormat>(); set => Set(GDExtensionPropertyName.Format, Variant.From(value)); }
	public new string DlsName { get => Get(GDExtensionPropertyName.DlsName).As<string>(); set => Set(GDExtensionPropertyName.DlsName, value); }
	public new string EncryptionKey { get => Get(GDExtensionPropertyName.EncryptionKey).As<string>(); set => Set(GDExtensionPropertyName.EncryptionKey, value); }
	public new long MaxPolyphony { get => Get(GDExtensionPropertyName.MaxPolyphony).As<long>(); set => Set(GDExtensionPropertyName.MaxPolyphony, value); }
	public new long InitialSeekPosition { get => Get(GDExtensionPropertyName.InitialSeekPosition).As<long>(); set => Set(GDExtensionPropertyName.InitialSeekPosition, value); }
	public new FmodSystem.FmodTimeUnit InitialSeekPostype { get => Get(GDExtensionPropertyName.InitialSeekPostype).As<FmodSystem.FmodTimeUnit>(); set => Set(GDExtensionPropertyName.InitialSeekPostype, Variant.From(value)); }
	public new bool IgnoreSetFileSystem { get => Get(GDExtensionPropertyName.IgnoreSetFileSystem).As<bool>(); set => Set(GDExtensionPropertyName.IgnoreSetFileSystem, value); }

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName LoadFromFile = "load_from_file";
	}

	public new static FmodAudioStreamAdvanced LoadFromFile(string path, long flags = 1) =>
		FmodAudioStreamAdvanced.Bind(ClassDB.ClassCallStatic(NativeName, GDExtensionMethodName.LoadFromFile, [path, flags]).As<Resource>());
}

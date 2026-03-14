#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodDSPConnection : RefCounted
{

	private new static readonly StringName NativeName = new StringName("FmodDSPConnection");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodDSPConnection object), please use the Instantiate() method instead.")]
	protected FmodDSPConnection() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodDSPConnection"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodDSPConnection"/> wrapper type,
	/// a new instance of the <see cref="FmodDSPConnection"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodDSPConnection"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodDSPConnection Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodDSPConnection wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodDSPConnection);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodDSPConnection).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodDSPConnection)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodDSPConnection"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodDSPConnection" type.</returns>
	public new static FmodDSPConnection Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public enum FmodDspConnectionType
	{
		DspconnectionTypeStandard = 0,
		DspconnectionTypeSidechain = 1,
		DspconnectionTypeSend = 2,
		DspconnectionTypeSendSidechain = 3,
		DspconnectionTypePreallocated = 4,
		DspconnectionTypeMax = 5,
		DspconnectionTypeForceint = 65536,
	}

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Mix = "mix";
	}

	public new double Mix
	{
		get => Get(GDExtensionPropertyName.Mix).As<double>();
		set => Set(GDExtensionPropertyName.Mix, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName SetMixMatrix = "set_mix_matrix";
		public new static readonly StringName GetMixMatrix = "get_mix_matrix";
		public new static readonly StringName GetInput = "get_input";
		public new static readonly StringName GetOutput = "get_output";
		public new static readonly StringName GetType = "get_type";
	}

	public new void SetMixMatrix(float[] matrix, long outchannels, long inchannels, long inchannelHop = 0) => 
		Call(GDExtensionMethodName.SetMixMatrix, [matrix, outchannels, inchannels, inchannelHop]);

	public new float[] GetMixMatrix(long outchannels, long inchannels, long inchannelHop = 0) => 
		Call(GDExtensionMethodName.GetMixMatrix, [outchannels, inchannels, inchannelHop]).As<float[]>();

	public new FmodDSP GetInput() => 
		FmodDSP.Bind(Call(GDExtensionMethodName.GetInput, []).As<RefCounted>());

	public new FmodDSP GetOutput() => 
		FmodDSP.Bind(Call(GDExtensionMethodName.GetOutput, []).As<RefCounted>());

	public new FmodDSPConnection.FmodDspConnectionType GetType() => 
		Call(GDExtensionMethodName.GetType, []).As<FmodDSPConnection.FmodDspConnectionType>();

}

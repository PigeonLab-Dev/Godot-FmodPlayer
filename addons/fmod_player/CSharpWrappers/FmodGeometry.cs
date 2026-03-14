#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;
using Godot.Collections;

namespace FmodPlayer;

[Tool]
public partial class FmodGeometry : RefCounted
{

	private new static readonly StringName NativeName = new StringName("FmodGeometry");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodGeometry object), please use the Instantiate() method instead.")]
	protected FmodGeometry() { }

	private static CSharpScript _wrapperScriptAsset;

	/// <summary>
	/// Try to cast the script on the supplied <paramref name="godotObject"/> to the <see cref="FmodGeometry"/> wrapper type,
	/// if no script has attached to the type, or the script attached to the type does not inherit the <see cref="FmodGeometry"/> wrapper type,
	/// a new instance of the <see cref="FmodGeometry"/> wrapper script will get attaches to the <paramref name="godotObject"/>.
	/// </summary>
	/// <remarks>The developer should only supply the <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</remarks>
	/// <param name="godotObject">The <paramref name="godotObject"/> that represents the correct underlying GDExtension type.</param>
	/// <returns>The existing or a new instance of the <see cref="FmodGeometry"/> wrapper script attached to the supplied <paramref name="godotObject"/>.</returns>
	public new static FmodGeometry Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;

		if (godotObject is FmodGeometry wrapperScriptInstance)
			return wrapperScriptInstance;

#if DEBUG
		var expectedType = typeof(FmodGeometry);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif

		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodGeometry).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}

		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodGeometry)InstanceFromId(instanceId);
	}

	/// <summary>
	/// Creates an instance of the GDExtension <see cref="FmodGeometry"/> type, and attaches a wrapper script instance to it.
	/// </summary>
	/// <returns>The wrapper instance linked to the underlying GDExtension "FmodGeometry" type.</returns>
	public new static FmodGeometry Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName Position = "position";
		public new static readonly StringName Rotation = "rotation";
		public new static readonly StringName Scale = "scale";
		public new static readonly StringName Active = "active";
	}

	public new Vector3 Position
	{
		get => Get(GDExtensionPropertyName.Position).As<Vector3>();
		set => Set(GDExtensionPropertyName.Position, value);
	}

	public new Vector3 Rotation
	{
		get => Get(GDExtensionPropertyName.Rotation).As<Vector3>();
		set => Set(GDExtensionPropertyName.Rotation, value);
	}

	public new Vector3 Scale
	{
		get => Get(GDExtensionPropertyName.Scale).As<Vector3>();
		set => Set(GDExtensionPropertyName.Scale, value);
	}

	public new bool Active
	{
		get => Get(GDExtensionPropertyName.Active).As<bool>();
		set => Set(GDExtensionPropertyName.Active, value);
	}

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName GeometryIsValid = "geometry_is_valid";
		public new static readonly StringName GeometryIsNull = "geometry_is_null";
		public new static readonly StringName SetPolygonAttributes = "set_polygon_attributes";
		public new static readonly StringName GetPolygonAttributes = "get_polygon_attributes";
		public new static readonly StringName GetPolygonNumVertices = "get_polygon_num_vertices";
		public new static readonly StringName SetPolygonVertex = "set_polygon_vertex";
		public new static readonly StringName GetPolygonVertex = "get_polygon_vertex";
		public new static readonly StringName AddPolygon = "add_polygon";
		public new static readonly StringName GetMaxPolygons = "get_max_polygons";
		public new static readonly StringName Save = "save";
		public new static readonly StringName GetSaveSize = "get_save_size";
		public new static readonly StringName Release = "release";
	}

	public new bool GeometryIsValid() => 
		Call(GDExtensionMethodName.GeometryIsValid, []).As<bool>();

	public new bool GeometryIsNull() => 
		Call(GDExtensionMethodName.GeometryIsNull, []).As<bool>();

	public new void SetPolygonAttributes(long index, double directOcclusion, double reverbOcclusion, bool doubleSided) => 
		Call(GDExtensionMethodName.SetPolygonAttributes, [index, directOcclusion, reverbOcclusion, doubleSided]);

	public new Godot.Collections.Dictionary GetPolygonAttributes(long index) => 
		Call(GDExtensionMethodName.GetPolygonAttributes, [index]).As<Godot.Collections.Dictionary>();

	public new long GetPolygonNumVertices(long index) => 
		Call(GDExtensionMethodName.GetPolygonNumVertices, [index]).As<long>();

	public new void SetPolygonVertex(long index, long vertexIndex, Vector3 vertex) => 
		Call(GDExtensionMethodName.SetPolygonVertex, [index, vertexIndex, vertex]);

	public new Vector3 GetPolygonVertex(long index, long vertexIndex) => 
		Call(GDExtensionMethodName.GetPolygonVertex, [index, vertexIndex]).As<Vector3>();

	public new long AddPolygon(double directOcclusion, double reverbOcclusion, bool doubleDided, long numVertices, Vector3 vertices) => 
		Call(GDExtensionMethodName.AddPolygon, [directOcclusion, reverbOcclusion, doubleDided, numVertices, vertices]).As<long>();

	public new Godot.Collections.Dictionary GetMaxPolygons() => 
		Call(GDExtensionMethodName.GetMaxPolygons, []).As<Godot.Collections.Dictionary>();

	public new byte[] Save() => 
		Call(GDExtensionMethodName.Save, []).As<byte[]>();

	public new long GetSaveSize() => 
		Call(GDExtensionMethodName.GetSaveSize, []).As<long>();

	public new void Release() => 
		Call(GDExtensionMethodName.Release, []);

}

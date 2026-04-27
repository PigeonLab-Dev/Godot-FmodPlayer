#pragma warning disable CS0109
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Godot;

namespace FmodPlayer;

[Tool]
public partial class FmodGeometryInstance3D : StaticBody3D
{
	private new static readonly StringName NativeName = new StringName("FmodGeometryInstance3D");

	[Obsolete("Wrapper types cannot be constructed with constructors (it only instantiate the underlying FmodGeometryInstance3D object), please use the Instantiate() method instead.")]
	protected FmodGeometryInstance3D() { }

	private static CSharpScript _wrapperScriptAsset;

	public new static FmodGeometryInstance3D Bind(GodotObject godotObject)
	{
		if (!IsInstanceValid(godotObject))
			return null;
		if (godotObject is FmodGeometryInstance3D wrapperScriptInstance)
			return wrapperScriptInstance;
#if DEBUG
		var expectedType = typeof(FmodGeometryInstance3D);
		var currentObjectClassName = godotObject.GetClass();
		if (!ClassDB.IsParentClass(expectedType.Name, currentObjectClassName))
			throw new InvalidOperationException($"The supplied GodotObject ({currentObjectClassName}) is not the {expectedType.Name} type.");
#endif
		if (_wrapperScriptAsset is null)
		{
			var scriptPathAttribute = typeof(FmodGeometryInstance3D).GetCustomAttributes<ScriptPathAttribute>().FirstOrDefault();
			if (scriptPathAttribute is null) throw new UnreachableException();
			_wrapperScriptAsset = ResourceLoader.Load<CSharpScript>(scriptPathAttribute.Path);
		}
		var instanceId = godotObject.GetInstanceId();
		godotObject.SetScript(_wrapperScriptAsset);
		return (FmodGeometryInstance3D)InstanceFromId(instanceId);
	}

	public new static FmodGeometryInstance3D Instantiate() => Bind(ClassDB.Instantiate(NativeName).As<GodotObject>());

	public enum SourceMode { Auto = 0, CollisionShapes = 1, MeshInstance = 2, MeshResource = 3 }

	public new static class GDExtensionPropertyName
	{
		public new static readonly StringName SourceMode = "source_mode";
		public new static readonly StringName SourceNodePath = "source_node_path";
		public new static readonly StringName Mesh = "mesh";
		public new static readonly StringName DirectOcclusion = "direct_occlusion";
		public new static readonly StringName ReverbOcclusion = "reverb_occlusion";
		public new static readonly StringName DoubleSided = "double_sided";
		public new static readonly StringName Active = "active";
		public new static readonly StringName AutoRebuild = "auto_rebuild";
		public new static readonly StringName SyncTransform = "sync_transform";
		public new static readonly StringName RecursiveSourceScan = "recursive_source_scan";
		public new static readonly StringName PrimitiveSegments = "primitive_segments";
		public new static readonly StringName ShowDebugGizmo = "show_debug_gizmo";
	}

	public new SourceMode Source { get => Get(GDExtensionPropertyName.SourceMode).As<SourceMode>(); set => Set(GDExtensionPropertyName.SourceMode, Variant.From(value)); }
	public new NodePath SourceNodePath { get => Get(GDExtensionPropertyName.SourceNodePath).As<NodePath>(); set => Set(GDExtensionPropertyName.SourceNodePath, value); }
	public new Mesh Mesh { get => Get(GDExtensionPropertyName.Mesh).As<Mesh>(); set => Set(GDExtensionPropertyName.Mesh, value); }
	public new double DirectOcclusion { get => Get(GDExtensionPropertyName.DirectOcclusion).As<double>(); set => Set(GDExtensionPropertyName.DirectOcclusion, value); }
	public new double ReverbOcclusion { get => Get(GDExtensionPropertyName.ReverbOcclusion).As<double>(); set => Set(GDExtensionPropertyName.ReverbOcclusion, value); }
	public new bool DoubleSided { get => Get(GDExtensionPropertyName.DoubleSided).As<bool>(); set => Set(GDExtensionPropertyName.DoubleSided, value); }
	public new bool Active { get => Get(GDExtensionPropertyName.Active).As<bool>(); set => Set(GDExtensionPropertyName.Active, value); }
	public new bool AutoRebuild { get => Get(GDExtensionPropertyName.AutoRebuild).As<bool>(); set => Set(GDExtensionPropertyName.AutoRebuild, value); }
	public new bool SyncTransform { get => Get(GDExtensionPropertyName.SyncTransform).As<bool>(); set => Set(GDExtensionPropertyName.SyncTransform, value); }
	public new bool RecursiveSourceScan { get => Get(GDExtensionPropertyName.RecursiveSourceScan).As<bool>(); set => Set(GDExtensionPropertyName.RecursiveSourceScan, value); }
	public new long PrimitiveSegments { get => Get(GDExtensionPropertyName.PrimitiveSegments).As<long>(); set => Set(GDExtensionPropertyName.PrimitiveSegments, value); }
	public new bool ShowDebugGizmo { get => Get(GDExtensionPropertyName.ShowDebugGizmo).As<bool>(); set => Set(GDExtensionPropertyName.ShowDebugGizmo, value); }

	public new static class GDExtensionMethodName
	{
		public new static readonly StringName RebuildGeometry = "rebuild_geometry";
		public new static readonly StringName ClearGeometry = "clear_geometry";
		public new static readonly StringName HasValidGeometry = "has_valid_geometry";
		public new static readonly StringName SyncGeometryTransform = "sync_geometry_transform";
		public new static readonly StringName GetPolygonCount = "get_polygon_count";
		public new static readonly StringName GetVertexCount = "get_vertex_count";
		public new static readonly StringName GetGeometry = "get_geometry";
	}

	public new void RebuildGeometry() => Call(GDExtensionMethodName.RebuildGeometry, []);
	public new void ClearGeometry() => Call(GDExtensionMethodName.ClearGeometry, []);
	public new bool HasValidGeometry() => Call(GDExtensionMethodName.HasValidGeometry, []).As<bool>();
	public new void SyncGeometryTransform() => Call(GDExtensionMethodName.SyncGeometryTransform, []);
	public new long GetPolygonCount() => Call(GDExtensionMethodName.GetPolygonCount, []).As<long>();
	public new long GetVertexCount() => Call(GDExtensionMethodName.GetVertexCount, []).As<long>();
	public new FmodGeometry GetGeometry() => FmodGeometry.Bind(Call(GDExtensionMethodName.GetGeometry, []).As<RefCounted>());
}

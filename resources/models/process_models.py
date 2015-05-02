import glob
import bpy
import mathutils

# Fix Kenney's 3D objects (http://kenney.itch.io/kenney-donation) in Three.js
# Three.js' object loader barfs when object is not made of simple quads.

# This script uses Blender command-line to load each asset, converts to tris,
# then convert to quads (to remove some vertices), and re-saves (overwrites!)

# To run:
# > cd <path to assets>
# > blender -b -P fixQuads.py

def fixObjs(folder):
  for obj in glob.glob(folder + '*.obj'):
    bpy.ops.import_scene.obj(filepath=obj)
    model = bpy.data.objects[len(bpy.data.objects) - 1]

    bpy.context.scene.objects.active = model

    # center geometry
    bpy.ops.object.mode_set(mode='OBJECT')
    bpy.ops.object.origin_set(type='GEOMETRY_ORIGIN')
    bpy.ops.object.origin_set(type='ORIGIN_CENTER_OF_MASS')
    bpy.ops.object.mode_set(mode='EDIT')

    bpy.ops.mesh.quads_convert_to_tris()
    bpy.ops.mesh.tris_convert_to_quads()
    bpy.ops.object.mode_set(mode='OBJECT')

    mesh_obj = bpy.context.active_object

    minz = 999999.0

    for vertex in mesh_obj.data.vertices:
        # object vertices are in object space, translate to world space
        v_world = mesh_obj.matrix_world * mathutils.Vector((vertex.co[0],vertex.co[1],vertex.co[2]))

        if v_world[2] < minz:
            minz = v_world[2]

    mesh_obj.location.z = mesh_obj.location.z - minz

    bpy.ops.export_scene.obj(filepath=obj, use_selection=True)

fixObjs('nature/')
fixObjs('town/')

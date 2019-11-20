import sys
import timeit

sys.path.append("@CMAKE_INSTALL_PREFIX@/MrGeo")
binary_path = '@CMAKE_INSTALL_PREFIX@/MrGeo/libMrGeo.so.@RELOAD_COUNTER@'

import bpy
import math

import MrGeo_blender
import MrGeo_interface
from importlib import reload
reload(MrGeo_blender)
reload(MrGeo_interface)


surface_data = MrGeo_interface.SurfaceData()

MrGeo_blender.init_poly_surface(surface_data)

bpy.ops.object.editmode_toggle()
bpy.ops.object.editmode_toggle()

fourierSpin = 10
num_frames = 100
num_lines = 3

clib = MrGeo_interface.Clib(binary_path)(surface_data, fourierSpin)

MrGeo_blender.update_control(clib, surface_data)
clib.compute_samples()
MrGeo_blender.update_samples(clib, surface_data)

#clib.test_bases()
#clib.torus_test()
clib.interpolate()
clib.fill(num_lines,num_frames)

num_objects = clib.get_num_objects()
num_frames = clib.get_num_frames()

MrGeo_blender.cleanUp()
arrow_mesh = MrGeo_blender.load_template("@CMAKE_INSTALL_PREFIX@/MrGeo/arrow_mesh.stl")
x_vertex_mesh = MrGeo_blender.init_path_tracing("@CMAKE_INSTALL_PREFIX@/MrGeo/x_vertex_mesh.stl", num_frames)
def run():
    for i in range(0, num_objects):
        arrow_obj = MrGeo_blender.addArrow(num_frames, arrow_mesh, \
           str(i), \
           clib.get_scales(i), \
           clib.get_locations(i), \
           clib.get_rotations(i))
        if (i+1) % (num_objects/num_lines) == 0:
            MrGeo_blender.addTraceObj(arrow_obj, x_vertex_mesh)

print(timeit.timeit(run, number=1))



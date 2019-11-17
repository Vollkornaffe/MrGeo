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

template_mesh = bpy.data.meshes["template_mesh"]

surface_data = MrGeo_interface.SurfaceData()

MrGeo_blender.init_poly_surface(surface_data)

bpy.ops.object.editmode_toggle()
bpy.ops.object.editmode_toggle()

fourierSpin = 4

clib = MrGeo_interface.Clib(binary_path)(surface_data, fourierSpin)

MrGeo_blender.update_control(clib, surface_data)
clib.compute_samples()
MrGeo_blender.update_samples(clib, surface_data)

##clib.test_bases()
#clib.torus_test()
#clib.fill(3,500)
#
#num_objects = clib.get_num_objects()
#num_frames = clib.get_num_frames()
#
#MrGeo_blender.cleanUp()
#
#def run():
#    for i in range(0, num_objects):
#        MrGeo_blender.addArrow(num_frames, template_mesh, str(i), \
#                                       clib.get_scales(i), \
#                                       clib.get_locations(i), \
#                                       clib.get_rotations(i))
#
#print(timeit.timeit(run, number=1))



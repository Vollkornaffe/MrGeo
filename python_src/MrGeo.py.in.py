import sys

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

fourierSpin = 4

clib = MrGeo_interface.Clib(binary_path)(fourierSpin)

clib.test_bases()

num_objects = clib.get_num_objects()
num_frames = clib.get_num_frames()

MrGeo_blender.cleanUp()
for i in range(0, num_objects):
    MrGeo_blender.addArrow(num_frames, template_mesh, str(i),\
            clib.get_scales(i),\
            clib.get_locations(i),\
            clib.get_rotations(i))
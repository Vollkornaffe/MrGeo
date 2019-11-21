import bpy

def cleanUp():

    for obj in bpy.data.objects:
        if obj.name.startswith("term_"):
            bpy.data.objects.remove(obj)

    for obj in bpy.data.objects:
        if obj.name.startswith("line_"):
            bpy.data.objects.remove(obj)

    # clean up from previous runs
    for obj in bpy.data.objects:
        if obj.users == 0:
            bpy.data.objects.remove(obj)
    for action in bpy.data.actions:
        # somehow there are always users...
        #if action.users == 0:
        #    bpy.data.actions.remove(action)
        bpy.data.actions.remove(action)
    for mesh in bpy.data.meshes:
        if mesh.users == 0:
            bpy.data.meshes.remove(mesh)
    for material in bpy.data.materials:
        if material.users == 0:
            bpy.data.materials.remove(material)
    for obj in bpy.data.particles:
        bpy.data.particles.remove(obj)

def init_materials(num_lines):
    for i in range(0, num_lines):
        # colorful material
        mat = bpy.data.materials.new("line_material_"+str(i))
        mat.diffuse_color = colorsys.hsv_to_rgb(i/num_lines,1,1)[:] + (1.0,)

def addTraceObj(x_vertex_mesh, name, arrow_obj, num_frames, material):

    # object to render for the particles
    particle_render_obj = bpy.ops.mesh.primitive_ico_sphere_add(subdivisions=0)
    bpy.context.object.name = "line_render_"+name
    bpy.context.object.data.name = "line_render_"+name
    bpy.context.object.hide_viewport = True
    bpy.context.object.hide_render = True
    bpy.data.objects["line_render_"+name].active_material = material

    # set up particles

    particle_settings = bpy.data.particles.new("line_particles_"+name)
    particle_settings.normal_factor = 0
    particle_settings.effector_weights.gravity = 0
    particle_settings.frame_end = num_frames
    particle_settings.count = num_frames
    particle_settings.lifetime = num_frames
    particle_settings.render_type = "OBJECT"
    particle_settings.emit_from = "VERT"
    particle_settings.instance_object = bpy.data.objects["line_render_"+name]

    # object to trace out the line & to emit the particles
    new_obj = bpy.data.objects.new(name="line_tracer_"+name, object_data=x_vertex_mesh)
    new_obj.constraints.new("COPY_TRANSFORMS")
    new_obj.constraints[0].target = arrow_obj
    new_obj.modifiers.new("dummy", type="PARTICLE_SYSTEM")
    new_obj.particle_systems[0].settings = bpy.data.particles["line_particles_"+name]

    bpy.context.view_layer.active_layer_collection.collection.objects.link(new_obj)

    return new_obj

def addArrow(\
    num_frames,\
    arrow_mesh,\
    name,\
    scales,\
    positions,\
    rotations):

    new_obj = bpy.data.objects.new(name="term_"+name, object_data=arrow_mesh)

    new_obj.rotation_mode = "QUATERNION"

    new_obj.animation_data_create()
    new_action = new_obj.animation_data.action = bpy.data.actions.new(name="action_"+name)

    fcu_sca_x = new_action.fcurves.new(data_path="scale", index=0)
    fcu_sca_y = new_action.fcurves.new(data_path="scale", index=1)
    fcu_sca_z = new_action.fcurves.new(data_path="scale", index=2)

    fcu_loc_x = new_action.fcurves.new(data_path="location", index=0)
    fcu_loc_y = new_action.fcurves.new(data_path="location", index=1)
    fcu_loc_z = new_action.fcurves.new(data_path="location", index=2)

    fcu_rot_w = new_action.fcurves.new(data_path="rotation_quaternion", index=0)
    fcu_rot_x = new_action.fcurves.new(data_path="rotation_quaternion", index=1)
    fcu_rot_y = new_action.fcurves.new(data_path="rotation_quaternion", index=2)
    fcu_rot_z = new_action.fcurves.new(data_path="rotation_quaternion", index=3)

    fcu_sca_x.keyframe_points.add(num_frames)
    fcu_sca_y.keyframe_points.add(num_frames)
    fcu_sca_z.keyframe_points.add(num_frames)

    fcu_loc_x.keyframe_points.add(num_frames)
    fcu_loc_y.keyframe_points.add(num_frames)
    fcu_loc_z.keyframe_points.add(num_frames)

    fcu_rot_w.keyframe_points.add(num_frames)
    fcu_rot_x.keyframe_points.add(num_frames)
    fcu_rot_y.keyframe_points.add(num_frames)
    fcu_rot_z.keyframe_points.add(num_frames)


    def make_list(array, idx):
        l = [0] * 2 * num_frames
        i = 0
        for kf in range(0, num_frames):
            l[i] = kf
            i += 1
            l[i] = array[kf][idx]
            i += 1
        return l

    fcu_sca_x.keyframe_points.foreach_set('co', make_list(scales, 0))
    fcu_sca_y.keyframe_points.foreach_set('co', make_list(scales, 1))
    fcu_sca_z.keyframe_points.foreach_set('co', make_list(scales, 2))

    fcu_loc_x.keyframe_points.foreach_set('co', make_list(positions, 0))
    fcu_loc_y.keyframe_points.foreach_set('co', make_list(positions, 1))
    fcu_loc_z.keyframe_points.foreach_set('co', make_list(positions, 2))

    fcu_rot_w.keyframe_points.foreach_set('co', make_list(rotations, 0))
    fcu_rot_x.keyframe_points.foreach_set('co', make_list(rotations, 1))
    fcu_rot_y.keyframe_points.foreach_set('co', make_list(rotations, 2))
    fcu_rot_z.keyframe_points.foreach_set('co', make_list(rotations, 3))


    fcu_sca_x.update()
    fcu_sca_y.update()
    fcu_sca_z.update()

    fcu_loc_x.update()
    fcu_loc_y.update()
    fcu_loc_z.update()

    fcu_rot_w.update()
    fcu_rot_x.update()
    fcu_rot_y.update()
    fcu_rot_z.update()

    bpy.context.view_layer.active_layer_collection.collection.objects.link(new_obj)

    return new_obj

def init_poly_surface(surfaceData):

    if ("Control" in bpy.data.objects):
        bpy.data.objects.remove(bpy.data.objects["Control"])
    if ("Samples" in bpy.data.objects):
        bpy.data.objects.remove(bpy.data.objects["Samples"])
    if ("Control" in bpy.data.meshes):
        bpy.data.meshes.remove(bpy.data.meshes["Control"])
    if ("Samples" in bpy.data.meshes):
        bpy.data.meshes.remove(bpy.data.meshes["Samples"])

    bpy.ops.mesh.primitive_torus_add(major_segments=surfaceData.u_numControl, minor_segments=surfaceData.v_numControl, major_radius=2, minor_radius=1)
    bpy.ops.object.editmode_toggle()
    bpy.ops.mesh.select_all(action='SELECT')
    bpy.ops.mesh.delete(type='ONLY_FACE')
    bpy.ops.object.editmode_toggle()
    bpy.context.object.name = "Control"
    bpy.context.object.data.name = "Control"

    bpy.ops.mesh.primitive_torus_add(major_segments=surfaceData.u_numSamples, minor_segments=surfaceData.v_numSamples, major_radius=2, minor_radius=1)
    #bpy.ops.object.editmode_toggle()
    #bpy.ops.mesh.select_all(action='SELECT')
    #bpy.ops.mesh.delete(type='ONLY_FACE')
    #bpy.ops.object.editmode_toggle()
    bpy.context.object.name = "Samples"
    bpy.context.object.data.name = "Samples"

def update_control(clib, surfaceData):

    bpy.ops.object.editmode_toggle()
    bpy.ops.object.editmode_toggle()

    controlPoly = bpy.data.meshes['Control']
    samplesPoly = bpy.data.meshes['Samples']

    for j in range(0, surfaceData.u_numControl):
        control_ptr = clib.get_control(j)
        for i in range(0, surfaceData.v_numControl):
            v = controlPoly.vertices[i + j * surfaceData.v_numControl]
            control_ptr[i*3 + 0] = v.co.x
            control_ptr[i*3 + 1] = v.co.y
            control_ptr[i*3 + 2] = v.co.z

def update_samples(clib, surfaceData):

    bpy.ops.object.editmode_toggle()
    bpy.ops.object.editmode_toggle()

    controlPoly = bpy.data.meshes['Control']
    samplesPoly = bpy.data.meshes['Samples']

    for j in range(0, surfaceData.u_numSamples):
        samples_ptr = clib.get_samples(j)
        for i in range(0, surfaceData.v_numSamples):
            v = samplesPoly.vertices[i + j * surfaceData.v_numSamples]
            v.co.x = samples_ptr[i*3 + 0]
            v.co.y = samples_ptr[i*3 + 1]
            v.co.z = samples_ptr[i*3 + 2]

    samplesPoly.update()

def load_arrow_mesh(path):
    if ("arrow_mesh" in bpy.data.objects):
        bpy.data.objects.remove(bpy.data.objects["arrow_mesh"])
    if ("arrow_mesh" in bpy.data.meshes):
        bpy.data.meshes.remove(bpy.data.meshes["arrow_mesh"])
    bpy.ops.import_mesh.stl(filepath=path)
    bpy.context.object.name = "arrow_mesh"
    bpy.context.object.data.name = "arrow_mesh"
    bpy.context.object.hide_viewport = True
    bpy.context.object.hide_render = True

    return bpy.context.object.data

def load_x_vertex_mesh(path, num_frames):


    if ("x_vertex" in bpy.data.objects):
        bpy.data.objects.remove(bpy.data.objects["x_vertex"])
    if ("x_vertex" in bpy.data.meshes):
        bpy.data.meshes.remove(bpy.data.meshes["x_vertex"])
    bpy.ops.import_mesh.stl(filepath=path)
    bpy.context.object.name = "x_vertex"
    bpy.context.object.data.name = "x_vertex"
    bpy.context.object.hide_viewport = True
    bpy.context.object.hide_render = True

    return bpy.context.object.data

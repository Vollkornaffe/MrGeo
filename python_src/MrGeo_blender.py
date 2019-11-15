import bpy

def cleanUp():
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


def addArrow(\
    num_frames,\
    template_mesh,\
    name,\
    scales,\
    positions,\
    rotations):

    new_obj = bpy.data.objects.new(name="object_"+name, object_data=template_mesh)

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

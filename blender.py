import bpy
import os

## THE FOLLOWING THREE STATEMENTS ARE THE PARAMETERS
# Name of file
objName = 'demo3'
# Path to file
path = "/Users/xuchen/Documents/ECE496/"+objName+".ply"
# Output file name (will default go to blender folder)
outputFile = "test"

# Import model mesh file to blender (different for different extensions)
bpy.ops.import_mesh.ply(filepath=path)

# Setup
scene = bpy.data.scenes["Scene"]
context = bpy.context
pi = 3.14159265

#Set background color to white
bpy.data.worlds["World"].use_sky_paper = True
bpy.data.worlds["World"].horizon_color = (1.0,1.0,1.0)

# Sets imported object as the currently active object
bpy.context.scene.objects.active = bpy.data.objects[objName]
obj = bpy.context.active_object

# Temporary Solution for vertex of object too close to camera (move object back in terms of z axis)
#bpy.context.active_object.location.z = bpy.context.active_object.location.z - 200



# Add material to object and set to use vertex color paint
mat = bpy.data.materials.new(name="Material")
mat.use_vertex_color_paint = True
obj.data.materials.append(mat)
bpy.ops.object.shade_smooth()

# Sets all objects to shadeless
for m in bpy.data.materials:
    m.use_shadeless = True;


# Angles for camera
rx = 0
ry = 0
rz = 0

scene.camera.rotation_euler[0] = rx*(pi/180.0)
scene.camera.rotation_euler[1] = ry*(pi/180.0)
scene.camera.rotation_euler[2] = rz*(pi/180.0)

# Location of camera
scene.camera.location.x = 0
scene.camera.location.y = 0
scene.camera.location.z = 400

# Camera Clipping
bpy.data.cameras["Camera"].clip_end = 450

# Go through the camera, Print camera view to file
cams = [c for c in context.scene.objects if c.type == 'CAMERA']
for c in cams:
	context.scene.camera = c
	print("Render ", objName, context.scene.name, c.name)
	context.scene.render.filepath = outputFile
	bpy.ops.render.render(write_still=True)


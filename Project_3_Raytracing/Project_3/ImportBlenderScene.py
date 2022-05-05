
import bpy

obj = bpy.context.active_object
mesh = obj.data
data = bpy.data.meshes['Torus']



f = open("./Project_3/blenderScene.txt","w")
    
# Find all the Triangles in the poly mesh
tri_count = 0
for poly in mesh.polygons:
    if len(poly.vertices) == 3:
        tri_count += 1
        
        # print the three point of the triangle
        f.write('Triangle {\n')
        counter = 0

        # Create the 3 cordinites for the vertices of the triangles. 
        for idx in poly.vertices:
            f.write('vertex'+ str(counter)+ ' ' + str(data.vertices[idx].co[0]) + ' ' + str(data.vertices[idx].co[1]) + ' ' + str(data.vertices[idx].co[2])+'\n')
            counter+=1

        # Create the textures
        f.write('tex_xy_0 1 0\n')
        f.write('tex_xy_1 0 0\n')
        f.write('tex_xy_2 0 1\n')
        f.write('materialIndex 1\n')
        f.write('}\n')
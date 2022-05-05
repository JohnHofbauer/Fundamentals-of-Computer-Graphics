John Hofbauer
F21_Graphics_p1_Description

To run the program open the Project_2.exe Program 
The keybinds
	
	T - Toggle the camera to the track (while the camera is on the rolocoster, the w,a,s,d and
		mouse rotation will not work.)
	N - Toggle on and off the drawing the normals. 
	H - Toggle Drawing the heightmap 
	Q - Toggle quaterines
	
	W - Translate the camera Forwards 
	S - Translate the camera Backwards 
	A - Translate the camera Left 
	D - Translate the camera Right 
	
	Mouse_Rotation - Change the orientation of the camera

File name and Line number starting where each part of my project is located

	Project2.hpp
		Line 62 - Added the required new verables for the track and volocity and position for the camera
		
	Project2.cpp
		Line 282 - Included a diffrent heightmap (sorce below) 
		Line 282 - Removed the drawing of the boxes at every point
		Line 375 - Toggle the camera to the postion on the track determaned by the computer time, 
			the force of gravity and the current velocity. this along with the current height are used
			to find the next postiton of the camera.
		Line 436 - Draw the normals of the track.
		Line 514 - Added the IO function for the T key to toggle the camera to the track
	
	track.hpp
		Line 73 - Function get_camera_orentation, returns the correct camera orentation for that position
			of the track. 
		Line 92 - Function height_diffrence, return the defirence of heigh the cart on the track went to 
			determan the new velocity with physics. 
		Line 96 - Function to get the direct distnace diffrence in 3d space-skyboxes
		Line 100 - Function velocity, get the velocity of the cart 
		Line 104 - deistance moved, return the distacne the cart must move with the current velocity
		Line 108 - returns the whole track length so the camear knows when to restart. 
		Line 114 - Draws the drack and scails it. 
		Line 155 - gets the 4 points that are needed for to interpolate from the vertex list. (makes sure the 
			poins to not exceed the 
		Line 218 - Print function for testing the interpolation funcion. 
		Line 241 - Function interpolate, will use Catmull-Rom spline to interpolate 4 points in 3d space. 
		Line 311 - Defines a orenation to start will at the beginning of the track, also the same orentation that 
			the cart ends with so that the transition is seamless. Then a forloop is used to devide each point into
			10 subpoints using the interpolation function and the cross product for the orentation defined in class. 
			the orentation are then sent to make track part. 
		Line 406 - make vertex is the same as heightmap make vertex, this time we include a normal cordante. 
		Line 426 - make triangle takes 3 points and makes makes the vertices then pushes them back into the verticies list.
		Line 466 - make rectange takes 4 points and devides them into two triangles fliping the normal on one one of them.
		Line 466 - make rail plank, interpolates the function of a circle to make 2 circles on each side of the track, then
			connects them useing make rectange. 
		Line 493 - make rail part, creates the rails by interpolating polor cordanated of a circle and conecting them. 
		Line 555 - setup track, creates the VBO buffer for the vertices and the VAO pointer to the buffer, this also defines
			the vertices attribute pointer for the postition, normal, and texture cordantes. 
		
List of everything in the Submission
|   Project_2_ Hofbauer_John.mp4
|   Project_2.md
|   README.md
|
+---Headers
|       camera.hpp
|       heightmap.hpp
|       mesh.hpp
|       model.hpp
|       Project2.hpp
|       rc_spline.h
|       shader.hpp
|       track.hpp
|
+---Media
|   +---cart
|   |       roller.mtl
|   |       roller.obj
|   |       roller.obj.blend
|   |       roller.obj.blend1
|   |       roller.obj1
|   |
|   +---heightmaps
|   |       grass.jpg
|   |       hflab4.jpg
|   |       spiral.jpg
|   |
|   +---skybox
|   |       back.jpg
|   |       back.png
|   |       bottom.jpg
|   |       bottom.png
|   |       front.jpg
|   |       front.png
|   |       left.jpg
|   |       left.png
|   |       right.jpg
|   |       right.png
|   |       top.jpg
|   |       top.png
|   |
|   +---spline
|   |       track-backup.sp
|   |       track.sp
|   |
|   +---spline_parts
|   |       magic.sp
|   |       negx.sp
|   |       negy.sp
|   |       negz.sp
|   |       pointsgenerator.py
|   |       testTrack.sp
|   |       turn-loop.sp
|   |       turn-negx-negy.sp
|   |       turn-negx-negz.sp
|   |       turn-negx-y.sp
|   |       turn-negy-x.sp
|   |       turn-negy-x2.sp
|   |       turn-screw.sp
|   |       turn-screw2.sp
|   |       turn-x-negy.sp
|   |       turn-x-y.sp
|   |       turn-x-y2.sp
|   |       turn-x-z.sp
|   |       turn-x-z2.sp
|   |       turn-y-negx.sp
|   |       x.sp
|   |       y.sp
|   |       z.sp
|   |       zig.sp
|   |
|   \---textures
|           awesomeface.png
|           container.jpg
|           container2.png
|           container2_specular.png
|           fire.jpg
|           wall.jpg
|
+---Shaders
|       lightingShader_basic.frag
|       lightingShader_basic.frag~
|       lightingShader_basic.vert
|       lightingShader_basic.vert~
|       lightingShader_nMap.frag
|       lightingShader_nMap.frag~
|       lightingShader_nMap.vert
|       lightingShader_nMap.vert~
|       lightingShader_specular.frag
|       lightingShader_specular.vert
|       lightingShader_specular.vert~
|       normal.frag
|       normal.frag~
|       normal.geom
|       normal.geom~
|       normal.vert
|       normal.vert~
|       reflectionShader.frag
|       reflectionShader.vert
|       skyboxShader.frag
|       skyboxShader.vert
|
\---Sources
        Project2.cpp
        rc_spline.cpp



EXTRA CREDIT
	I have used polor cordanates of a circle to interpolate the rails and make them as round as posible within reason
		I can change the resolution of the rails to make them seem more round by adding more faces. (currently set to 10
		for preformence)
	I have added a rolocoster cart model that will ride around the track without the camera. 
		
	

Recorces

	Skybox - https://opengameart.org/content/ulukais-space-skyboxes
	heighmap texture - https://creativemarket.com/hhh316/247499-Lush-Seamless-Grass-Texture#fullscreen
	coster cart - https://free3d.com/3d-model/roller-coaster-9242.html
	fire texture - https://www.texturex.com/fire-textures/fire-texture-hot-flame-red-burn-stock-photo-wallpaper/
	
John Hofbauer
Computer_Graphics_p3_Description

To run the project
	1) open the exacutable project 3.exe
	2) type the command 
		".\Project_3.exe SampleScenes/singleTriangleNoLight.ray n"
		OR
		".\Project_3.exe SampleScenes/singleSphereLighted.ray n" in powershell
		"Project_3.exe SampleScenes/singleSphereLighted.ray n" in cmd
		
	   To name the output image use a 3rd argument
		".\Project_3.exe SampleScenes/reflectionTest.ray y name"
		
	3) to run the other files
		- To run the importBlenderScene.py The file needs to be opened in blender with
		a object, (curnetly set to a torus) seleted in the scene in object mode. 
		this will create the triangle objects that can then be used in the ray file 
		
		- to run the CreateRayAnimation python file type "python CreateRayAnimation.py" in 
		comand prompt and the ray files will be placed in the media folder under my animamations
		
		- to run the createVideo.m matlab script open in matlab and change the file path and the number
		of files that are needed to create the video. 
		
		- to run the runrayfolder.py change the path to the folder with the ray files and then run the file im cmd
		
	
File name and Line number starting where each part of my project is located
	Project3.cpp
		Line 23 - Created a getDirection fucntion that inputs all the peramiters needed
			(float fovx, float fovy, glm::vec3 screenRight, glm::vec3 screenDown, glm::vec3 dir, int x, int y)
			to calulate the direction of the ray that goes out of the camera into the screen. 
		Line 184 - Created starting verables dir (starting direction) and left and right rays for screenDown and 
			ScreenRight.
		Line 254 - Callign the get Direction funciton for each x and y pixel, and then importing it into the raytracing fucniton.
		Line 328 - added the funconality to change the output image with a 3rd comand line argument. 
		
	scene.cpp
		Line 53 - adding the ambient light ot the answer times the objects color. 
		Line 65 - For each of the light calculate the defuse color, by checking the normal of the object to the light, and
			the normal of the object doing the dot project. Adding the light color times the diffues to the answer.
		Line 114 - adeed the ability to detect shadows when there is an object inbetween the light and the interseiton, 
			however, if the object is tranpartent then there is less or no shadow. 
		Line 145 - Created a recursion depth of 3 for the reflection and refracted rays. 
		Line 151 - Calculated the reflection ray and used recursion to find the color of the intersection point
		Line 202 - Calculated the refraction ray and used recuseion to find the color of the intesection. 
			
	triangle.cpp
		Line 20 - Created the testIntersection fucntion using the matrix from the textbook and bayacentr4ic cordinates, then
			solving with Cramer's rule. This returns the distance to the intersection.
		Line 131 - getNormal fucntion, reutrns the normal of the triangle 
		Line 149 - getTextureCoords function, returns the normilized texture cordanits(between 0 and 1) of the intersection of the 
			Triangle, the interseciton is then used to find the poin on the texX0 and texY0 that the point corasponds to. 
			
		
	sphere.cpp
		Line 12 - testIntersection for the sphear object, this uses the quadratic formula and reruns the frist intersection on the
			sphear. 
		Line 37 - getNormal funnction, reuturns the normal on the surface of the circle using the equation givne. 
		Line 79 - getTextureCoords function, returns the normilized texture cordanits for the intersection of the circle, using the projection
			to find phi ( the angle up and down from 0 to pi and theda the angle around from 0 to 2 pi) this is then normilized to be between 0 and 1.
		
List of everything in the Submission
	│   CreateRayAnimation.py
	│   ImportBlenderScene.py
	│   Project_3.exe
	│   RunRayFolder.py
	│
	├───Headers
	│       light.h
	│       material.h
	│       mesh.hpp
	│       model.hpp
	│       Project3.hpp
	│       rtObject.h
	│       rtObjGroup.h
	│       scene.h
	│       shader.hpp
	│       sphere.h
	│       triangle.h
	│
	├───Media
	│   ├───my_scenes
	│   │   │   kaleidoscope.ray
	│   │   │   kaleidoscope2.ray
	│   │   │
	│   │   └───animation
	│   ├───SampleScenes
	│   │       reflection&refraction.jpg
	│   │       reflection&refraction.ray
	│   │       reflectionandrefraction.ray
	│   │       reflectionTest.jpg
	│   │       reflectionTest.ray
	│   │       reflectiveSpheres&Tris.jpg
	│   │       reflectiveSpheres&Tris.ray
	│   │       reflectiveSpheresandTris.ray
	│   │       refractionTest.jpg
	│   │       refractionTest.ray
	│   │       refractPrism.jpg
	│   │       refractPrism.ray
	│   │       singleSphereLighted.jpg
	│   │       singleSphereLighted.ray
	│   │       singleTriangleNoLight.jpg
	│   │       singleTriangleNoLight.ray
	│   │       testEverything.jpg
	│   │       testEverything.ray
	│   │       textureSphereTest.jpg
	│   │       textureSphereTest.ray
	│   │       textureTriTest.jpg
	│   │       textureTriTest.ray
	│   │
	│   └───textures
	│           awesomeface.png
	│           check1024.jpg
	│           container.jpg
	│           container2.png
	│           container2_specular.png
	│           wall.jpg
	│           waterlilies.jpg
	│
	├───Shaders
	│       screenShader.frag
	│       screenShader.vert
	│
	└───Sources
			Project3.cpp
			rtObject.cpp
			rtObjGroup.cpp
			scene.cpp
			sphere.cpp
			triangle.cpp
			
Extra Credit 

	- I created a python program the creates ray files for the animation by interpolating
	a funciton and setting the position of the sphere's x and y to the cos and sin of theda
	
	- I created a python program that runs all the ray files in a folder, running one at a time,
	and wating for them to finish. The Images are then named after the ray file that made them. 
	
	- I created a python program that can take a scene of a tours from blender and inport the triangels 
	into a ray file format. 
	
	- I used a videomaker libray in a matlab script that combines the ray images into a video.
			
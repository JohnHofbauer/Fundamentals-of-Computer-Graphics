#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include <shader.hpp>
#include <rc_spline.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <deque>
#include <list>
#include <queue>

struct Orientation {
	// Front
	glm::vec3 Front;
	// Up
	glm::vec3 Up;
	// Right
	glm::vec3 Right;
	// origin
	glm::vec3 origin;
};


class Track
{
public:

	// VAO
	unsigned int VAO;

	// Control Points Loading Class for loading from File
	rc_Spline g_Track;

	// Vector of control points
	std::vector<glm::vec3> controlPoints;

	// Track data
	std::vector<Vertex> vertices;
	// indices for EBO
	std::vector<unsigned int> indices;

	// hmax for camera
	float hmax = 0.0f;


	// constructor, just use same VBO as before, 
	Track(const char* trackPath)
	{

		// load Track data
		load_track(trackPath);

		create_track();

		setup_track();
	}

	// GLoabla verableas for prevous orentaion. 
	glm::vec3 prev_up = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 prev_front = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec3 prev_right = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));


	Orientation get_camera_orentation(float s) {


		// Create the orentation.
		Orientation camera_ori;
		camera_ori.origin = get_point(s);
		camera_ori.Front = glm::normalize(camera_ori.origin - get_point(s - 0.1f));  //Nn+1 = Bn x Tn+1
		camera_ori.Right = glm::normalize(glm::cross(prev_front, prev_up)); //Bn+1 = Tn+1 x Nn+1
		camera_ori.Up = glm::normalize(glm::cross(camera_ori.Right, camera_ori.Front));

		// Update global values
		prev_up = camera_ori.Up;
		prev_front = camera_ori.Front;
		prev_right = camera_ori.Right;

		return camera_ori;

	}

	float height_diffrence(glm::vec3 prevous_height, glm::vec3 next_height) {
		return (float)(prevous_height.y - next_height.y);
	}

	float pos_diffrence(glm::vec3 prevous_pos, glm::vec3 next_pos) {
		return (float)sqrt(pow(prevous_pos.x - prevous_pos.x, 2) + pow(prevous_pos.y - prevous_pos.y, 2) + pow(prevous_pos.z - prevous_pos.z, 2));
	}

	float velocity(float delta_height, float prevous_velocity) {
		return (float)sqrt(2.0f * 9.81f * (delta_height) + pow(prevous_velocity, 2));
	}

	float distance_moved(float delta_time, float velocity) {
		return (float)(velocity * delta_time);
	}

	float return_track_length() {
		return (float)(controlPoints.size() - 2);
	}


	// render the mesh
	void Draw(Shader shader, unsigned int textureID)
	{


		// Set the shader properties
		shader.use();
		glm::mat4 model;
		shader.setMat4("model", model);


		// Set material properties
		shader.setVec3("material.specular", 0.3f, 0.3f, 0.3f);
		shader.setFloat("material.shininess", 64.0f);


		// active proper texture unit before binding
		glActiveTexture(GL_TEXTURE0);
		// and finally bind the textures
		glBindTexture(GL_TEXTURE_2D, textureID);
		glBindVertexArray(VAO);



		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		//model = glm::translate(model, glm::vec3(-0.5f, -0.8f, -0.5f));

		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(Vertex) * vertices.size());


		

		// always good practice to set everything back to defaults once configured.
		glActiveTexture(GL_TEXTURE0);
	}

	// give a positive float s, find the point by interpolation
	// determine pA, pB, pC, pD based on the integer of s
	// determine u based on the decimal of s
	// E.g. s=1.5 is the at the halfway point between the 1st and 2nd control point,
	//		the 4 control points are:[0,1,2,3], with u=0.5
	glm::vec3 get_point(float s)
	{

		if (s < 0.0f || s > return_track_length() - 1) { s =  s - return_track_length(); }

		int pA = floor(s) - 1;
		if (floor(s) < 1) { pA = floor(controlPoints.size()-1); }

		int pB = floor(s);

		int pC = floor(s) + 1;
		if (floor(s) > controlPoints.size()) {	pC = floor(s) - controlPoints.size() + 1; }

		int pD = floor(s) + 2;
		if (floor(s) > controlPoints.size()) { pD = floor(s) - controlPoints.size() + 2; }
		
		float u = s - floor(s);

		//printf("Controal points a (%f, %f, %f)\n Controal points b (%f, %f, %f)\n Controal points c (%f, %f, %f)\n Controal points d (%f, %f, %f)\n", controlPoints[pA].x, controlPoints[pA].y, controlPoints[pA].z, controlPoints[pB].x, controlPoints[pB].y, controlPoints[pB].z, controlPoints[pC].x, controlPoints[pC].y, controlPoints[pC].z, controlPoints[pD].x, controlPoints[pD].y, controlPoints[pD].z);
		//printf("Controal points b (%f, %f, %f)\n Controal points c (%f, %f, %f)\n", controlPoints[pB].x, controlPoints[pB].y, controlPoints[pB].z, controlPoints[pC].x, controlPoints[pC].y, controlPoints[pC].z);
		//printf("u = %f\n", u);


		return interpolate(controlPoints[pA], controlPoints[pB], controlPoints[pC], controlPoints[pD], 0.5f, u);
	}


	void delete_buffers()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		//glDeleteBuffers(1, &EBO);
	}

private:


	/*  Render data  */
	unsigned int VBO, EBO;

	void load_track(const char* trackPath)
	{
		// Set folder path for our projects (easier than repeatedly defining it)
		g_Track.folder = "../Project_2/Media/";

		// Load the control points
		g_Track.loadSplineFrom(trackPath);



		/*glm::vec3 test = interpolate(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1), 0.5f, 0.0f);
		printf("Test interpolate = (%f, %f, %f)\n", test.x, test.y, test.z);
		test = interpolate(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1), 0.5f, 0.25f);
		printf("Test interpolate = (%f, %f, %f)\n", test.x, test.y, test.z);
		test = interpolate(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1), 0.5f, 0.5f);
		printf("Test interpolate = (%f, %f, %f)\n", test.x, test.y, test.z);
		test = interpolate(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1), 0.5f, 0.75f);
		printf("Test interpolate = (%f, %f, %f)\n", test.x, test.y, test.z);
		test = interpolate(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1), 0.5f, 1.0f);
		printf("Test interpolate = (%f, %f, %f)\n", test.x, test.y, test.z);
		*/

	}
	void printmat4(glm::mat4 points) {
		printf("Printing matrix 4\n");
		printf("[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n", points[0][0], points[0][1], points[0][2], points[0][3], points[1][0], points[1][1], points[1][2], points[1][3], points[2][0], points[2][1], points[2][2], points[2][3], points[3][0], points[3][1], points[3][2], points[3][3]);
	}
	void printmat4Column(glm::mat4 points) {
		printf("Printing matrix 4\n");
		printf("[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n", points[0][0], points[1][0], points[2][0], points[3][0], points[0][1], points[1][1], points[2][1], points[3][2], points[0][3], points[1][3], points[2][3], points[3][2], points[0][3], points[1][3], points[2][3], points[3][3]);
	}
	void printmat4x3(glm::mat4 points) {
		printf("Printing matrix 4 by 3\n");
		printf("[%f, %f, %f]\n[%f, %f, %f]\n[%f, %f, %f]\n[%f, %f, %f]\n", points[0][0], points[0][1], points[0][2], points[1][0], points[1][1], points[1][2], points[2][0], points[2][1], points[2][2], points[3][0], points[3][1], points[3][2]);
	}
	void printmat4x3Colum(glm::mat4 points) {
		printf("Printing matrix 4 by 3\n");
		printf("[%f, %f, %f]\n[%f, %f, %f]\n[%f, %f, %f]\n[%f, %f, %f]\n", points[0][0], points[1][0], points[2][0], points[0][1], points[1][1], points[2][1], points[0][3], points[1][3], points[2][3], points[0][3], points[1][3], points[2][3]);
	}


	// Implement the Catmull-Rom Spline here
	//	Given 4 points, a tau and the u value 
	//	u in range of [0,1]  
	//	Since you can just use linear algebra from glm, just make the vectors and matrices and multiply them.  
	//	This should not be a very complicated function
	glm::vec3 interpolate(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD, float tau, float u)
	{

		glm::mat4x3 points = glm::mat4x3(pointA, pointB, pointC, pointD);
		//COLUMN first  glm::mat4x3 points = glm::mat4x3(pointA.x, pointB.x, pointC.x, pointD.x, pointA.y, pointB.y, pointC.y, pointD.y, pointA.z, pointB.z, pointC.z, pointD.z);
		//printmat4x3(points);
		glm::mat4 interpolation = glm::mat4(glm::vec4(0, 1, 0, 0), glm::vec4(-tau, 0, tau, 0), glm::vec4(2 * tau, tau - 3, 3 - 2 * tau, -tau), glm::vec4(-tau, 2 - tau, tau - 2, tau));
		// COLUMN first glm::mat4 interpolation = glm::mat4(0, -tau, 2*tau, -tau, 1, 0, tau-3, 2-tau, 0, tau, 3-2*tau, tau-2, 0, 0, -tau, tau);
		//printmat4(interpolation);
		glm::vec4 uVector = glm::vec4(1, u, pow(u, 2), pow(u, 3));
		glm::vec3 newpoint = points * interpolation * uVector;

		// Just returning the first point at the moment, you need to return the interpolated point.  
		return newpoint;
	}

	// Here is the class where you will make the vertices or positions of the necessary objects of the track (calling subfunctions)
	//  For example, to make a basic roller coster:
	//    First, make the vertices for each rail here (and indices for the EBO if you do it that way).  
	//        You need the XYZ world coordinates, the Normal Coordinates, and the texture coordinates.
	//        The normal coordinates are necessary for the lighting to work.  
	//    Second, make vector of transformations for the planks across the rails
	void create_track()
	{
		// Create the vertices and indices (optional) for the rails
		//    One trick in creating these is to move along the spline and 
		//    shift left and right (from the forward direction of the spline) 
		//     to find the 3D coordinates of the rails.



		// Create the plank transformations or just creating the planks vertices
		//   Remember, you have to make planks be on the rails and in the same rotational direction 
		//       (look at the pictures from the project description to give you ideas).  



		// Here is just visualizing of using the control points to set the box transformatins with boxes. 
		//       You can take this code out for your rollercoster, this is just showing you how to access the control points
		//glm::vec3 currentpos = glm::vec3(-2.0f, 0.0f, -2.0f);



		/* iterate throught  the points	g_Track.points() returns the vector containing all the control points */

		glm::vec3 currentpos = glm::vec3(-2.0f, 0.0f, -2.0f);

		/* iterate throught  the points	g_Track.points() returns the vector containing all the control points */
		for (pointVectorIter ptsiter = g_Track.points().begin(); ptsiter != g_Track.points().end(); ptsiter++)
		{
			/* get the next point from the iterator */
			glm::vec3 pt(*ptsiter);

			// Print the Box
			std::cout << pt.x << "  " << pt.y << "  " << pt.z << std::endl;

			/* now just the uninteresting code that is no use at all for this project */
			currentpos += pt;
			//  Mutliplying by two and translating (in initialization) just to move the boxes further apart.  
			controlPoints.push_back(currentpos);

		}
		//controlPoints.push_back(glm::vec3(-1.0f, 0.0f, -2.0f)); // push back the starting pos
		//controlPoints.push_back(glm::vec3(-1.0f, 0.0f, -2.0f)); // push back the starting pos

		// for loop for all the points in the xontrolpoints vectro (array ) 
		// set the currnt to the prv_orgion and the net to the cur_orgion 
		//for (int i = 0; i < counter; i++) {
		//	printf("%f, %f, %f\n", vectorlist[i].x,  vectorlist[i].y, vectorlist[i].z);
		//}
		glm::vec3 up = glm::normalize(glm::vec3(0.0f, 1.0f, 0.1f));
		glm::vec3 front = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 right = glm::normalize(glm::cross(front, up));

		Orientation prev;
		float start = 3.1f;
		float trackstep = 0.1f;
		prev.origin = get_point(start);

		// fabracate the first postion 
		prev.Up = up;
		prev.Front = front;
		// cross prduct of up and front
		prev.Right = right;

		Orientation first;
		first.Front = prev.Front;
		first.Right = prev.Right;
		first.Up = prev.Up;
		first.origin = prev.origin;

		

		//printf("[%f, %f, %f]\n", prev.origin.x, prev.origin.y, prev.origin.z);

		for (float s = start + trackstep; s <= controlPoints.size()-3; s += trackstep) {


			// do the math to create the next pisition
			Orientation next;

			next.origin = get_point(s);
			//printf("distance s = %f \n", s);
			//printf("get_point(s) x=%f   y=%f     z=%f\n", get_point(s).x, get_point(s).y, get_point(s).z);

			// Tn+1  = T(sn+1)
			next.Front = glm::normalize(next.origin - prev.origin); // i might have to normilize this
			//Nn+1 = Bn x Tn+1
			next.Right = glm::normalize(glm::cross(next.Front, prev.Up));
			//Bn+1 = Tn+1 x Nn+1
			next.Up = glm::normalize(glm::cross(next.Right, next.Front)); // TRACJ WIDTH < THICK >


			if (s == controlPoints.size() - 3) {
				next.Up = first.Up;
			}

			glm::vec2 offset = glm::vec2(0.03f, 0.03f);
			float raildistance = 0.5f;

			//return interpolate(, , controlPoints[pC], controlPoints[pD], 0.5f, u);
			// Make left rail
			prev.origin -= prev.Right * raildistance;
			next.origin -= next.Right * raildistance;
			makeRailPart(prev, next, offset);
			// make right rail
			prev.origin += prev.Right * raildistance;
			next.origin += next.Right * raildistance;
			makeRailPart(prev, next, offset);
			//make rail tie

			Orientation left = next;
			left.origin -= next.Right * raildistance;
			makeRailPlank(left, next, offset);


			//printf("[%f; %f; %f]\n ", prev.origin.x, prev.origin.y, prev.origin.z);
			//printf("[%f; %f; %f]\n ", next.origin.x, next.origin.y, next.origin.z);
			//printf("0rigin next x=%f   y=%f     z=%f\n", next.origin.x, next.origin.y, next.origin.z);


			// Print the Box
			//std::cout << pt.x << "  " << pt.y << "  " << pt.z << std::endl;
			prev.origin = next.origin;
			prev.Right = next.Right;
			prev.Up = next.Up;
			prev.Front = next.Front;
		}
		
		/*for (float s = 0; s <= (float)controlPoints.size()-1; s += 1) {

			glm::vec3 oof = get_point(s);
			//if (s < 0.0f || s > return_track_length() - 1) { s = s - return_track_length(); }
			glm::vec3 oof2 = controlPoints[s];

			printf("point %f [%f; %f; %f]  should match [%f; %f; %f]\n", s, oof.x, oof.y, oof.z, oof2.x, oof2.y, oof2.z);
		}*/

	}


	

	// Make Vertex:  take x and y position return a new vertex for that position which includes 
	//  the position and the texture coordinates
	Vertex make_vertex(glm::vec3 point) {

		//printf("x=%i y=%0.2f z=%i",x, y, z);
		Vertex v;
		//XYZ coords
		v.Position.x = point.x;
		v.Position.y = point.y;
		v.Position.z = point.z;

		// Setting normal to default, calculate later.  
		v.Normal = glm::vec3(0.0f, 0.0f, 0.0f);

		//Texture Coords
		v.TexCoords.x = point.x;
		v.TexCoords.y = point.y;

		return v;
	}
	// Given 3 Points, create a triangle and push it into vertices (and EBO if you are using one)
		// Optional boolean to flip the normal if you need to
	void make_triangle(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC,bool flipNormal)
	{
		Vertex a = make_vertex(pointA);
		Vertex b = make_vertex(pointB);
		Vertex c = make_vertex(pointC);
		if (flipNormal) {
			set_normals(a, b, c);
			vertices.push_back(a);
			vertices.push_back(b);
			vertices.push_back(c);
		}
		else {
			set_normals(c, b, a);
			vertices.push_back(c);
			vertices.push_back(b);
			vertices.push_back(a);
		}
		

		//while here, add normals. 
		
	}
	void print_vertices(int amount) {
		for (int i = 0; i < amount; i++) {
			printf("point x=%f   y=%f     z=%f\n", vertices[i].Position.x, vertices[i].Position.y, vertices[i].Position.z);
		}
	}

	// Given 3 Points, create a triangle and push it into vertices (and EBO if you are using one)
		// Optional boolean to flip the normal if you need to
	void make_rectangle(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD, bool flipNormal)
	{

		make_triangle(pointA, pointB, pointC, flipNormal);
		make_triangle(pointB, pointC, pointD, !flipNormal);

		//print_vertices(6);
		//printf("verticy amount= %i\n", (int)vertices.size());
		
	}
	void makeRailPlank(Orientation ori_prev, Orientation ori_cur, glm::vec2 offset)
	{
		ori_prev.Front = ori_prev.Front * offset.x;
		ori_cur.Front = ori_cur.Front * offset.x;
		ori_prev.Up = ori_prev.Up * offset.y;
		ori_cur.Up = ori_cur.Up * offset.y;

		float circle_res = M_PI / 10;
		for (float i = 0; i < 2 * M_PI; i += circle_res) {
			float width = cos(i);
			float height = sin(i);
			//printf("Circle points angle  = %f,  width = %f, height = %f\n", i, width, height);

			glm::vec3 bottomleft = ori_prev.origin + ori_prev.Front * width + ori_prev.Up * height;
			glm::vec3 bottomright = ori_cur.origin + ori_cur.Front * width + ori_cur.Up * height;

			width = cos(i + circle_res);
			height = sin(i + circle_res);
			glm::vec3 topleft = ori_prev.origin + ori_prev.Front * width + ori_prev.Up * height;
			glm::vec3 topright = ori_cur.origin + ori_cur.Front * width + ori_cur.Up * height;

			make_rectangle(bottomleft * 0.1f, bottomright * 0.1f, topleft * 0.1f, topright * 0.1f, FALSE);

		}

	}
	// Given two orintations, create the rail between them.  Offset can be useful if you want to call this for more than for multiple rails
	void makeRailPart(Orientation ori_prev, Orientation ori_cur, glm::vec2 offset)
	{
		ori_prev.Right = ori_prev.Right * offset.x;
		ori_cur.Right = ori_cur.Right * offset.x;
		ori_prev.Up = ori_prev.Up * offset.y;
		ori_cur.Up = ori_cur.Up * offset.y;

		// Create the two circles of points for the prevous position and the current position. 
		// First two points will be to the right.

		
		float circle_res = M_PI/10;
		for (float i = 0; i < 2*M_PI; i+=circle_res) {
			float width = cos(i);
			float height = sin(i);
			//printf("Circle points angle  = %f,  width = %f, height = %f\n", i, width, height);

			glm::vec3 bottomleft = ori_prev.origin + ori_prev.Right * width + ori_prev.Up * height;
			glm::vec3 bottomright = ori_cur.origin + ori_cur.Right * width + ori_cur.Up * height;

			width = cos(i + circle_res);
			height = sin(i + circle_res);
			glm::vec3 topleft = ori_prev.origin + ori_prev.Right * width + ori_prev.Up * height;
			glm::vec3 topright = ori_cur.origin + ori_cur.Right * width + ori_cur.Up * height ;

			//glm::vec3 bottomleft = (ori_prev.origin + ori_prev.Right * width) + (offset.x + ori_prev.Up * height) * offset.y;
			//glm::vec3 bottomright = (ori_cur.origin + ori_cur.Right * width) + (offset.x + ori_cur.Up * height) * offset.y;

			//width = cos(i + circle_res);
			//height = sin(i + circle_res);
			//glm::vec3 topleft = (ori_prev.origin + ori_prev.Right * width) + (offset.x + ori_prev.Up * height) * offset.y;
			//glm::vec3 topright = (ori_cur.origin + ori_cur.Right * width) + (offset.x + ori_cur.Up * height) * offset.y;

			make_rectangle(bottomleft * 0.1f, bottomright * 0.1f, topleft * 0.1f, topright * 0.1f, TRUE);

		}

		//printf("Orientation prev = x: %f, y: %f, z: %f\n", ori_prev.origin.x, ori_prev.origin.y, ori_prev.origin.z);
		//printf("Orientation next = x: %f, y: %f, z: %f\n", ori_cur.origin.x, ori_cur.origin.y, ori_cur.origin.z);

		// plot right axsis.
		//make_triangle(ori_prev.origin * 0.1f, ori_cur.origin * 0.1f, (ori_cur.origin + ori_cur.Right) * 0.1f, FALSE);
		//make_triangle(ori_prev.origin * 0.1f, ori_cur.origin * 0.1f, (ori_cur.origin + ori_cur.Up) * 0.1f, FALSE);

		// pass each squear in the cilinder side into the make rectangle fuction. 
		//make_rectangle((ori_prev.origin + ori_prev.Right * offset.x)  * 0.1f, (ori_prev.origin + ori_prev.Up * offset.y)  * 0.1f, (ori_cur.origin + ori_cur.Right * offset.x) * 0.1f, (ori_cur.origin + ori_cur.Up * offset.y) * 0.1f, FALSE);

		

	}

	// Find the normal for each triangle uisng the cross product and then add it to all three vertices of the triangle.  
	//   The normalization of all the triangles happens in the shader whi ch averages all norms of adjacent triangles.   
	//   Order of the triangles matters here since you want to normal facing out of the object.  
	void set_normals(Vertex &p1, Vertex &p2, Vertex &p3)
	{
		glm::vec3 normal = glm::cross(p2.Position - p1.Position, p3.Position - p1.Position);
		p1.Normal += normal;
		p2.Normal += normal;
		p3.Normal += normal;
	}

	void setup_track()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		//glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/3/2 array which
		// again translates to 3/3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normal coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}

};

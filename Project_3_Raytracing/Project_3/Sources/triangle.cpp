#include "triangle.h"

//constructor given  center, radius, and material
triangle::triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, float tx0, float tx1, float tx2, float ty0, float ty1, float ty2, int m, scene* s) : rtObject(s)
{
	point0 = p0;
	point1 = p1;
	point2 = p2;

	texX0 = tx0;
	texX1 = tx1;
	texX2 = tx2;
	texY0 = ty0;
	texY1 = ty1;
	texY2 = ty2;
	matIndex = m;
	myScene = s;
}

float triangle::testIntersection(glm::vec3 eye, glm::vec3 dir)
{
	
	//see the book/slides for a description of how to use Cramer's rule to solve
	//for the intersection(s) of a line and a plane, implement it here and
	//return the minimum distance (if barycentric coordinates indicate it hit
	//the triangle) otherwise 9999999
	

	float j = point0.x - eye.x;
	float k = point0.y - eye.y;
	float l = point0.z - eye.z;


	float a = point0.x - point1.x;
	float b = point0.y - point1.y;
	float c = point0.z - point1.z;
	float d = point0.x - point2.x;
	float e = point0.y - point2.y;
	float f = point0.z - point2.z;
	float g = dir.x;
	float h = dir.y;
	float i = dir.z;

	float eihf = (e * i - h * f);
	float gfdi = (g * f - d * i);
	float jcal = (j * c - a * l);
	float blkc = (b * l - k * c);
	float akjb = (a * k - j * b);
	float dheg = (d * h - e * g);
	

	float M = a * eihf + b * gfdi + c * dheg;

	/*
	a = point0.x - point1.x;
	b = point0.y - point1.y;
	c = point0.z - point1.z;
	d = point0.x - point2.x;
	e = point0.y - point2.y;
	f = point0.z - point2.z;
	g = point0.x - eye.x;
	h = point0.y - eye.y;
	i = point0.z - eye.z;
	*/

	float t = -1.0f * ((f * akjb + e * jcal + d * blkc) / M);
	//printf("distance:%f\n", t);
	if (t < 0) {
		return 9999999;
	}
	/*
	a = point0.x - eye.x;
	b = point0.y - eye.y;
	c = point0.z - eye.z;
	d = point0.x - point2.x;
	e = point0.y - point2.y;
	f = point0.z - point2.z;
	g = dir.x;
	h = dir.y;
	i = dir.z;
	*/

	//M = a * (e * i - h * f) + b * (g * f - d * i) + c * (d * h - e * g);
	float beta = (j * eihf + k * gfdi + l * dheg) / M;

	if (beta < 0 || beta > 1) {
		return 9999999;
	}

	/*
	a = point0.x - point1.x;
	b = point0.y - point1.y;
	c = point0.z - point1.z;
	d = point0.x - eye.x;
	e = point0.y - eye.y;
	f = point0.z - eye.z;
	g = dir.x;
	h = dir.y;
	i = dir.z;
	*/

	//M = a * (e * i - h * f) + b * (g * f - d * i) + c * (d * h - e * g);
	float gamma = (i * akjb + h * jcal + g * blkc) / M;

	if (gamma < 0 || gamma > 1 - beta) {
		return 9999999;
	}
	/*
	float eihf = (e * i - h * f);
	float gfdi = (g * f - d * i);
	float jcal = (j * c - a * l);
	float blkc = (b * l - k * c);
	float akjb = (a * k - j * b);
	float dheg = (d * h - e * g);
	*/
	float epsilon = 0.00005f;
	if (t < epsilon) {
		t = testIntersection(eye + dir * epsilon, dir);
	}


	return t;


	//glm::mat3 A = glm::mat3(a, b, c, d, e, f, g, h, i);
	//float beta = glm::determinant(test)/glm::determinant(A);

	
	//eye to point0 + beta * bary1 + gamma * barry2 = eye + t * dir
	// Cramer's rule give us 
	// [a d g] [beta]    [j]
	// [b e h] [gamma] = [k]
	// [c g i] [t]       [l]
}

glm::vec3 triangle::getNormal(glm::vec3 eye, glm::vec3 dir, float dist)
{
	//construct the barycentric coordinates for the plane
	glm::vec3 bary1 = point1 - point0;
	glm::vec3 bary2 = point2 - point0;

	//cross them to get the normal to the plane
	//note that the normal points in the direction given by right-hand rule
	//(this can be important for refraction to know whether you are entering or leaving a material)
	glm::vec3 normal = glm::normalize(glm::cross(bary1, bary2));
	return normal;
}

glm::vec2 triangle::getTextureCoords(glm::vec3 eye, glm::vec3 dir, float dist)
{
	//find alpha and beta (parametric distance along barycentric coordinates)
	//use these in combination with the known texture surface location of the vertices
	//to find the texture surface location of the point you are seeing
	
	float j = point0.x - eye.x;
	float k = point0.y - eye.y;
	float l = point0.z - eye.z;


	float a = point0.x - point1.x;
	float b = point0.y - point1.y;
	float c = point0.z - point1.z;
	float d = point0.x - point2.x;
	float e = point0.y - point2.y;
	float f = point0.z - point2.z;
	float g = dir.x;
	float h = dir.y;
	float i = dir.z;

	float eihf = (e * i - h * f);
	float gfdi = (g * f - d * i);
	float jcal = (j * c - a * l);
	float blkc = (b * l - k * c);
	float akjb = (a * k - j * b);
	float dheg = (d * h - e * g);

	glm::vec3 normal = getNormal(eye, dir, dist);

	float M = a * eihf + b * gfdi + c * dheg;

	float beta = (j * eihf + k * gfdi + l * dheg) / M;

	float gamma = (i * akjb + h * jcal + g * blkc) / M;

	if (glm::dot(normal, dir) < 0.0f) {// normal pointing into the screen

		//beta = 1.0f - beta;
		//gamma = 1.0f - gamma;
	}
	else {
		//beta = 1.0f - beta;
		//beta = 1.0f - beta;
	}
	
	glm::vec2 A (texX0, texY0);
	glm::vec2 B (texX1, texY1);
	glm::vec2 C (texX2, texY2);

	glm::vec2 AB = B - A;
	glm::vec2 AC = C - A;

	glm::vec2 coords = A + AB * beta + AC * gamma;
	return coords;
}
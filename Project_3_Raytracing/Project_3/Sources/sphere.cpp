#include "sphere.h"
#define _USE_MATH_DEFINES
#include <math.h>

//constructor given  center, radius, and material
sphere::sphere(glm::vec3 c, float r, int m, scene* s) : rtObject(s)
{
	center = c;
	radius = r;
	matIndex = m;
	myScene = s;
}

float sphere::testIntersection(glm::vec3 eye, glm::vec3 dir)
{
	
	//see the book for a description of how to use the quadratic rule to solve
	//for the intersection(s) of a line and a sphere, implement it here and
	//return the minimum positive distance or 9999999 if none
	float a = glm::dot(dir, dir);
	float b = glm::dot(dir, (eye - center));
	float c = glm::dot((eye - center), (eye - center)) - radius * radius;

	float firstintersection = (-b - sqrt(b*b - a * c))/a;
	float seccondintersection = (-b + sqrt(b * b - a * c)) / a;



	// Return the first intersection if it is the smallest positive intersection. 
	if (firstintersection > 0) {
		if (firstintersection < seccondintersection || seccondintersection < 0) {

			float epsilon = 0.00005f;
			if (firstintersection < epsilon) {
				firstintersection = testIntersection(eye + dir * epsilon, dir);
			}

			return firstintersection;
		}
	}
	
	// retrun the seccond intersecion if it is the only posiive intersection. Or less(closer) then the first.
	if (seccondintersection > 0) {

		float epsilon = 0.00005f;
		if (seccondintersection < epsilon) {
			seccondintersection = testIntersection(eye + dir * epsilon, dir);
		}

		return seccondintersection;
	}




	return 9999999;
}

glm::vec3 sphere::getNormal(glm::vec3 eye, glm::vec3 dir, float dist)
{
	//once you can test for intersection,
	//simply add (distance * view direction) to the eye location to get surface location,
	//then subtract the center location of the sphere to get the normal out from the sphere
	//return center;
	glm::vec3 normal = (eye + dir * dist) - center;

	//dont forget to normalize
	normal = glm::normalize(normal);
	
	return normal;
}

glm::vec3 sphere::getCenter()
{

	return center;
}

glm::vec2 sphere::getTextureCoords(glm::vec3 eye, glm::vec3 dir, float dist)
{
	//find the normal as in getNormal
	glm::vec3 normal = getNormal(eye, dir, dist);

	//use these to find spherical coordinates
	glm::vec3 x(1, 0, 0);
	glm::vec3 z(0, 0, 1);

	//phi is the angle down from z
	//theta is the angle from x curving toward y
	//hint: angle between two vectors is the acos() of the dot product

	//find phi using normal and z (angle between z and the normal)
	float phi = glm::acos(glm::dot(z, normal));

	//find the x-y projection of the normal
	glm::vec3 y(0, 1, 0);
	glm::vec3 xproj = glm::dot(x, normal) * x;
	glm::vec3 yproj = glm::dot(y, normal) * y;

	glm::vec3 proj = glm::normalize(xproj + yproj);

	//find theta using the x-y projection and x
	float theta = glm::acos(glm::dot(x, proj));

	//if x-y projection is in quadrant 3 or 4, then theta=2*PI-theta
	if (theta < 0) {
		theta = 2 * M_PI - theta;
	}

	//return coordinates scaled to be between 0 and 1
	glm::vec2 coords = glm::vec2(theta / (2 * M_PI), phi / M_PI);

	return coords;
}
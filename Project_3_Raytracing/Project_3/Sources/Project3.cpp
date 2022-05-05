/*
Project 3 Submission for CMPSC458
Name: Joe Smith
psu id: xyz123
*/

#include <Project3.hpp>
#include <omp.h>
#define CLUSTER true


//  Modify this preamble based on your code.  It should contain all the functionality of your project.  
std::string preamble =
"Project 3 code: Raytracing \n"
"Sample Input:  SampleScenes/singleTriangleNoLight.ray n\n"
"Sample Input:  SampleScenes/singleSphereLighted.ray n\n"
"Sample Input:  SampleScenes/reflectionTest.ray n\n"
"Sample Input:  SampleScenes/refractionTest.ray n\n"
"Sample Input:  SampleScenes/reflection&refraction.ray n\n"
"Sample Input:  SampleScenes/textureTriTest.ray n\n"
"Sample Input:  SampleScenes/textureSphereTest.ray n\n"
"Sample Input:  SampleScenes/reflectiveSpheres&Tris.ray n\n"
"Sample Input:  SampleScenes/testEverything.ray n\n"
"Sample Input:  SampleScenes/refractPrism.ray n\n"
"Also, asks whether or not you want to configure for use with clusters (does not display results while running) \n"
"If you want to run without being asked for an input\n"
"'./Project_3/Project_3  SampleScenes/reflectionTest.ray y'\n\n";


glm::vec3 getDirection(float fovx, float fovy, glm::vec3 screenRight, glm::vec3 screenDown, glm::vec3 dir, int x, int y) {
	float dx = 2.0f * glm::sin(fovx / 2.0f) / SCR_WIDTH;
	float offsetx = ((float)x - ((float)SCR_WIDTH / 2.0f) + 0.5) * dx;

	float dy = 2.0f * glm::sin(fovy / 2.0f) / SCR_HEIGHT;
	float offsety = ((float)y - ((float)SCR_HEIGHT / 2.0f) + 0.5) * dy;

	return (dir + offsetx * screenRight + offsety * screenDown);

}

int main(int argc, char **argv)
{
	std::printf(preamble.c_str());
	std::string fn, ans_str;
	std::string ans_str2;

	// This turns off the window and OpenGL functions so that you can run it on the clusters.    
		//  This will also make it run slightly faster since it doesn't need to display in every 10 rows. 
	bool cluster=false;
	if (argc < 2)// if not specified, prompt for filename and if running on cluster
	{
		char input[999];
		std::printf("Input .ray file: ");
		std::cin >> input;
		fn = input;
		std::printf("Running on cluster or no display (y/n): ");
		std::cin >> input;
		ans_str = input;
		cluster= (ans_str.compare("y")==0) || (ans_str.compare("Y")==0);
	}
	else //otherwise, use the name provided
	{
		/* Open jpeg (reads into memory) */
		char* filename = argv[1];
		fn = filename;

		if (argc < 3)
		{
			char input[999];
			std::printf("Running on cluster or no display (y/n): ");
			std::cin >> input;
			ans_str = input;
			cluster= (ans_str.compare("y")==0) || (ans_str.compare("Y")==0);
		}
		else if (argc < 4)
		{
			char* ans = argv[2];
			ans_str = ans;
			cluster= (ans_str.compare("y")==0) || (ans_str.compare("Y")==0);
		}
		else
		{
			char* ans = argv[2];
			ans_str = ans;
			cluster = (ans_str.compare("y") == 0) || (ans_str.compare("Y") == 0);
			char* ans2 = argv[3];
			ans_str2 = ans2;
		}

	}


	if (cluster)
		std::printf("Configured for Clusters\n");
	else
		std::printf("Not configured for Clusters\n");
	fn = "../Project_3/Media/" + fn;
	std::printf("Opening File %s\n", fn.c_str());
	myScene = new scene(fn.c_str());
	GLFWwindow* window  = NULL;

	Shader screenShader("", "");
	if (!cluster)
	{
	    // glfw: initialize and configure
	    // ------------------------------

	    glfwInit();
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	    glfwWindowHint(GLFW_SAMPLES, 4);

	#ifdef __APPLE__
	    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif
	    // glfw window creation
	    // --------------------
	    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project 3", NULL, NULL);
	    if (window == NULL)
	    {
		    std::cout << "Failed to create GLFW window" << std::endl;
		    glfwTerminate();
		    return -1;
	    }
	    glfwMakeContextCurrent(window);
	    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	    // glad: load all OpenGL function pointers
	    // ---------------------------------------
	    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	    {
		    std::cout << "Failed to initialize GLAD" << std::endl;
		    return -1;
	    }


	    // configure global opengl state
	    // -----------------------------
	    glEnable(GL_MULTISAMPLE); // Enabled by default on some drivers, but not all so always enable to make sure

	    // build and compile shaders
	    // -------------------------


	    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
							     // positions   // texCoords
		    -1.0f,  1.0f,  0.0f, 1.0f,
		    -1.0f, -1.0f,  0.0f, 0.0f,
		    1.0f, -1.0f,  1.0f, 0.0f,

		    -1.0f,  1.0f,  0.0f, 1.0f,
		    1.0f, -1.0f,  1.0f, 0.0f,
		    1.0f,  1.0f,  1.0f, 1.0f
	    };

	    glGenVertexArrays(1, &quadVAO);
	    glGenBuffers(1, &quadVBO);
	    glBindVertexArray(quadVAO);
	    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	    glEnableVertexAttribArray(1);
	    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	    // shader configuration
	    // --------------------

	    screenShader = Shader("../Project_3/Shaders/screenShader.vert", "../Project_3/Shaders/screenShader.frag");
	    screenShader.use();
	    screenShader.setInt("screenTexture", 0);
	    glActiveTexture(GL_TEXTURE0);
	    // Create initial image texture
	    // Starts from the top left (0,0) to the bottom right
	    // X is the column number and Y is the row number
	    glGenTextures(1, &textureColorbuffer);
	}
	for (unsigned int y = 0; y < SCR_HEIGHT; y++)
		for (unsigned int x = 0; x < SCR_WIDTH; x++)
			image.push_back(glm::u8vec3(0));

	if (!cluster)
		update_image_texture();




	bool raytracing = true;

	//get camera parameters from scene
	glm::vec3 eye = myScene->getEye();
	glm::vec3 lookAt = myScene->getLookAt();
	glm::vec3 up = myScene->getUp();
	float fovy = myScene->getFovy();

	fovy = fovy * 3.1416f / (float)180;
	//printf("the fovy is %f\n", fovy);

	//get the direction we are looking
	glm::vec3 dir = glm::normalize(lookAt - eye);
	//cross up and dir to get a vector to our left
	glm::vec3 left = glm::normalize(glm::cross(up, dir));

	float fovx = (fovy*(float)SCR_WIDTH) / (float)SCR_HEIGHT;

	glm::vec3 gaze = glm::normalize(lookAt - eye); // center ray
	//float gazeLength = glm::length(gaze);
	//float height = tan(fovy) * gazeLength * 2;
	//float width = tan(fovx) * gazeLength * 2;

	glm::vec3 screenRight = glm::normalize(glm::cross(gaze, up));
	glm::vec3 screenDown = glm::normalize(glm::cross(gaze, screenRight));
	//glm::vec3 currentDir = gaze - screenRight * tan(fovy / 2) * gazeLength - screenDown * tan(fovx / 2) * gazeLength;
	//int halfHeight = (int)(SCR_HEIGHT / 2);
	//int halfWidth = (int)(SCR_WIDTH / 2);
	
	
	// Check the angle 
	/*
	// Middle top
	currentDir = gaze - screenDown * tan(fovy / 2) * gazeLength;
	float angle = glm::acos(glm::length(gaze) / glm::length(currentDir));
	printf(" The fovy angle %f should match %f\n", angle, fovy / 2);
	// Middle left
	currentDir = gaze - screenRight * tan(fovx / 2) * gazeLength;
	angle = glm::acos(glm::length(gaze) / glm::length(currentDir));
	printf(" The fovx angle %f should match %f\n", angle, fovx / 2);
	// Middle right
	currentDir = gaze + screenRight * tan(fovx / 2) * gazeLength;
	angle = glm::acos(glm::length(gaze) / glm::length(currentDir));
	printf(" The fovx angle %f should match %f\n", angle, fovx/2);
	*/
	glm::vec3 currentColor;
	// render loop
	// -----------
    bool running = true;
    if (!cluster) 
        running=!glfwWindowShouldClose(window);
	while (running)
	{
		// Start the ray tracing
		if (raytracing)
		{

			/*glm::vec3 dir = glm::vec3(1, 1, 1);
			glm::vec3 eye = glm::vec3(1, 1, 1);
			glm::vec3 center = glm::vec3(0, 0, 0);
			float radius = 1;
			float a = glm::dot(dir, dir);
			printf("a=%f", a);
			float b = glm::dot(dir, (eye - center));
			printf("[%f, %f, %f]", (eye - center).x, (eye - center).y, (eye - center).z);
			printf("b=%f", b);
			float c = glm::dot((eye - center), (eye - center)) - radius * radius;
			printf("c=%f", c);

			float firstintersection = (-b - sqrt(b * b - a * c)) / a;
			printf("firstintersection=%f", firstintersection);
			float seccondintersection = (-b + sqrt(b * b - a * c)) / a;
			printf("seccondintersection=%f", seccondintersection);*/

			// Go through 10 rows in parallel
			for (unsigned int y = 0; y < SCR_HEIGHT; y++)
			{
				// This runs the following loop in parallel -- You can comment the next line out for debugging purposes 
				//#pragma omp parallel for schedule(dynamic)
				for (unsigned int x = 0; x < SCR_WIDTH; x++)
				{
					glm::vec3 currentColor;
					// Some code that just makes a green/red pattern
					currentColor = glm::vec3(x % 255 / 255.0, y % 255 / 255.0, 0.0f);

					// You will have to write this function in the scene class, using recursive raytracing to determine color
					// Right here, you will need to determine the current direction "currentDir" based off what you know about projective geometry (Section 4.3)
					// You will need to compute the direction from the eye, to the place on the camera plane derived from the x and y coordinates and the lookat direction (Section 4.3.3) 
					// The eye is defined by the camera coordinates (above)
					// The ray is defined (using the line equation y=mx+b ... again) where m=the current direction and b= the eye. 
					// eye = vec3 position
					//currentDir = currentDir + screenRight * (float)SCR_WIDTH + screenDown * (float)SCR_HEIGHT;
					//fovy = h/halfHeight*fovy must go from -1 to positive 1
					//int newx = x - (int)(SCR_WIDTH / 2);
					//int newy = y - (int)(SCR_HEIGHT / 2);
					//if (x == 0) {
					//	printf("float right: %f", (float)tan(newx / floor(SCR_WIDTH / 2) * fovx / 2)* gazeLength);
					//}

					//glm::vec3 currentDir = gaze + screenRight * (float)tan(newx/floor(SCR_WIDTH /2)*fovx / 2) * gazeLength + screenDown * (float)tan(newy / floor(SCR_HEIGHT / 2)*fovx / 2) * gazeLength;
					glm::vec3 currentDir = getDirection(fovx, fovy, screenRight, screenDown, dir, x, y);
					currentColor = myScene->rayTrace(eye,glm::normalize(currentDir),0);
					
					//Put the color into our image buffer.  
					//  This first clamps the "currentColor" variable within a range of 0,1 which means min(max(x,0),1) 
					//	so if all white or black, your colors are outside this range.
					//  Then, this takes the float colors between 0 and 1 and makes them between [0,255], then coverts to uint8 through rounding.  
					image[x + y* SCR_WIDTH] = glm::u8vec3(glm::clamp(currentColor,0.0f,1.0f)*255.0f);
				}

				// Draw and process input for every 10 completed rows
				if (!cluster && (y % 10 == 0 || y == SCR_HEIGHT-1))
				{
					// input
					// -----
					processInput(window);
					if (glfwWindowShouldClose(window))
						break;

					//  Update texture for for drawing
					update_image_texture();


					// Draw the textrue
					glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
					glClear(GL_COLOR_BUFFER_BIT);
					// Bind our texture we are creating
					glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
					glBindVertexArray(quadVAO);
					glDrawArrays(GL_TRIANGLES, 0, 6);

					glfwSwapBuffers(window);
					glfwPollEvents();
				}

			}
			// Write the final output image
			//printf("output: %s", ans_str);
			char buf[9999];
			if (ans_str2.length() > 0 && argc >= 4) {
				sprintf(buf, "../Project_3/%s.png", ans_str2);
			}
			else {
				sprintf(buf, "../Project_3/final_out.png");
			}
			stbi_write_png(buf, SCR_WIDTH, SCR_HEIGHT, 3, &image[0], sizeof(glm::u8vec3)*SCR_WIDTH);
		}
		// Done rendering, just draw the image now
		raytracing = false;



        if (!cluster) 
        {
		    // input
		    // -----
		    processInput(window);

		    // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
		    glClear(GL_COLOR_BUFFER_BIT);
		    // Bind our texture we are creating
		    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		    glBindVertexArray(quadVAO);
		    glDrawArrays(GL_TRIANGLES, 0, 6);

		    glfwSwapBuffers(window);
		    glfwPollEvents();

            running=!glfwWindowShouldClose(window);

        }
        else
            running=false;

	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
    if (!cluster) 
    {
        glDeleteBuffers(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);

        glfwTerminate();
    }
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//   The movement of the boxes is still here.  Feel free to use it or take it out
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	// Escape Key quits
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	// P Key saves the image
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)

		stbi_write_png("../Project_3/out.png", SCR_WIDTH, SCR_HEIGHT, 3, &image[0], sizeof(glm::u8vec3)*SCR_WIDTH);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);

}

// Update the current texture in image and sent the data to the textureColorbuffer
void update_image_texture()
{
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, &image[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}



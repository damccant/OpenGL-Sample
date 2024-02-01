#include "glad/glad.h"
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <math.h>

/**
 * Callback for GLFW when the window is resized.
 * Updates the OpenGL viewport to fill the whole window
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool wireframe = false;
void toggleWireframe(void)
{
	wireframe = !wireframe;
	if(wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool allowChangeWireframe = true;

/**
 * Processes input for the window
 */
void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		// prevent wireframe mode toggling on/off instantly
		if(allowChangeWireframe)
			toggleWireframe();
		allowChangeWireframe = false;
	}
	else
		allowChangeWireframe = true;
}

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// Create the window
	GLFWwindow* window = glfwCreateWindow(800, 600, "C++ OpenGL Demo", NULL, NULL);
	if(!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// load GLAD
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// setup OpenGL viewport
	glViewport(0, 0, 800, 600);

	// setup the GLFW window callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// create the shader program (contains all shaders, etc.)
	Shader theShader;

	float vertices[] = {
		// positions       // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // top center
	};
	unsigned int indices[] = {
		0, 1, 2,
//		1, 2, 3,
	};

	unsigned int VAO, VBO, EBO;
	// create the vertex array object (points to the VBO)
	glGenVertexArrays(1, &VAO);
	// create the vertex buffer object
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the vertex array object
	glBindVertexArray(VAO);

	// fill the vertex buffer object with the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// fill the element buffer object with the indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// link vertex attributes
	// layout location 0, size of vertex attribute, data type, don't normalize, stride, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// layout location 1, size of vertex attribute, data type, don't normalize, stride, offset
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind the VBO so we don't accidently modify it
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// unbind the VAO so we don't accidently modify it
	glBindVertexArray(0);

	// main render loop
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		// clear the frame
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(theShader.id, "ourColor");
		// use the shader program
		theShader.use();
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		
		// bind to the vertex array object
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// delete the vertex array object
	glDeleteVertexArrays(1, &VAO);
	// delete the vertex buffer object
	glDeleteBuffers(1, &VBO);
	// delete the element buffer object
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

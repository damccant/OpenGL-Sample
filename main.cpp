const unsigned int MAZE_X = 25;
const unsigned int MAZE_Y = 20;

#include "glad/glad.h"

#include "shader.h"
#include "texture.h"
#include <iostream>
#include <iomanip>
#include <math.h>

/*#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"*/

#include <GLFW/glfw3.h>

#include "maze.h"
#include "path.h"
#include "game.h"

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

bool paused = true;
bool lost = false;
bool won = false;

bool allowChangeWireframe = true;
bool allowChangeMaze = true;
bool allowChangeShowPath = true;
bool allowChangePaused = true;
bool allowChangeNoclip = true;

/**
 * Resets the game to its original state
 */
void reset(Maze& maze, GameObject& cat, AIObject& mouse, AIObject& cheese, GameObject& cheeseTgt)
{
	maze.clearMaze();
	maze.generate();
	won = false;
	lost = false;

	// reset the cheese's target
	cheeseTgt.x = rand() % MAZE_X + 0.5f;
	cheeseTgt.y = rand() % MAZE_Y + 0.5f;

	// reset the cheese
	cheese.x = 0.5f;
	cheese.y = 0.5f;
	cheese.setTarget(&cheeseTgt);

	// reset the mouse
	mouse.x = 0.5f;
	mouse.y = MAZE_Y - 0.5f;
	mouse.setTarget(&cheese);

	// reset the cat
	cat.x = MAZE_X - 0.5f;
	cat.y = MAZE_Y - 0.5f;
}

/**
 * Processes input for the window
 */
void processInput(GLFWwindow *window, Maze& maze, GameObject& cat, AIObject& mouse, AIObject& cheese, GameObject& cheeseTgt)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		// prevent holding space
		if(allowChangePaused)
			if(lost || won)
				reset(maze, cat, mouse, cheese, cheeseTgt);
			else
				paused = !paused;
		allowChangePaused = false;
	}
	else
		allowChangePaused = true;
	if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		// prevent holding F1
		if(allowChangeMaze)
			reset(maze, cat, mouse, cheese, cheeseTgt);
		allowChangeMaze = false;
	}
	else
		allowChangeMaze = true;
	if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		// prevent holding F2
		if(allowChangeShowPath)
			AIObject::showPath = !AIObject::showPath;
		allowChangeShowPath = false;
	}
	else
		allowChangeShowPath = true;
	if(glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		// prevent holding F3
		if(allowChangeNoclip)
			PlayerObject::noclip = !PlayerObject::noclip;
		allowChangeNoclip = false;
	}
	else
		allowChangeNoclip = true;
	if(glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS)
	{
		// prevent wireframe mode toggling on/off instantly
		if(allowChangeWireframe)
			toggleWireframe();
		allowChangeWireframe = false;
	}
	else
		allowChangeWireframe = true;
}

void drawMenu(Shader shader, unsigned int texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	// use the shader program
	shader.use();

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::scale(trans, glm::vec3(0.75f, 0.75f, 1.0f));
	shader.setValue("transform", trans);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

	// enable texture blending (needed to make transparent textures work)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// create the shader program (contains all shaders, etc.)
	Shader theShader;

	float vertices[] = {
		// positions        // colors          // texture coords
		1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
		-1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f // top left
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// layout location 1, size of vertex attribute, data type, don't normalize, stride, offset
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// layout location 2, size of vertex attribute, data type, don't normalize, stride, offset
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind the VBO so we don't accidently modify it
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// unbind the VAO so we don't accidently modify it
	glBindVertexArray(0);

	Maze maze(MAZE_X, MAZE_Y);
	//Path path(maze, Coord(0, 0), Coord(24, 19));
	GameObject cheeseTgt(maze, rand() % MAZE_X + 0.5f, rand() % MAZE_Y + 0.5f);
	Cheese cheese(maze, 0.5f, 0.5f);
	cheese.setTarget(&cheeseTgt);
	Mouse mouse(maze, 0.5f, MAZE_Y - 0.5f);
	mouse.setTarget(&cheese);
	Cat cat(maze, MAZE_X - 0.5f, MAZE_Y - 0.5f);
	//cat.setTarget(&mouse);

	// TEXTURE
	unsigned int tex_pause = createTexture("asset/pause.png", GL_RGBA);
	unsigned int tex_lost = createTexture("asset/lost.png", GL_RGBA);
	unsigned int tex_won = createTexture("asset/won.png", GL_RGBA);
	Maze::loadMazeTextures("asset/maze/");
	Path::loadPathTextures();
	Cat::loadTextures("asset/cat.png");
	Cheese::loadTextures("asset/cheese.png");
	Mouse::loadTextures("asset/mouse.png");

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// main render loop
	while(!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		deltaTime = std::min(deltaTime, 1.0f); // don't let more than 1s elapse between frames
		processInput(window, maze, cat, mouse, cheese, cheeseTgt);
		if(!paused && !won && !lost)
		{
			cat.processInput(window, deltaTime);
			mouse.step();
			cheese.step();
			
			
			mouse.updatePosition(deltaTime);
			cheese.updatePosition(deltaTime);
			cat.updatePosition(deltaTime);

			if(cheese.checkCollision(cheeseTgt))
			{
				cheeseTgt.x = rand() % MAZE_X + 0.5f;
				cheeseTgt.y = rand() % MAZE_Y + 0.5f;
			}
			if(cat.checkCollision(mouse))
			{
				won = true;
			}
			if(mouse.checkCollision(cheese))
			{
				lost = true;
			}
		}

		// clear the frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// bind to the vertex array object (defines the shape)
		glBindVertexArray(VAO);
		// set the first texture as active (required for some drivers)
		glActiveTexture(GL_TEXTURE0);

		// render the maze
		maze.render(theShader);
		
		mouse.render(theShader, 1);
		cheese.render(theShader, 2);
		cat.render(theShader);
		
		if(paused)
			drawMenu(theShader, tex_pause);
		else if(won)
			drawMenu(theShader, tex_won);
		else if(lost)
			drawMenu(theShader, tex_lost);


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

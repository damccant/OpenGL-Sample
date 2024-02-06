#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "path.h"
#include "shader.h"
#include "texture.h"

/**
 * Represents a game object
 */
class GameObject
{
protected:
	static unsigned int texture;
	const Maze *maze;
public:
	/**
	 * Represents the objects game coordinates
	 * (Converted to screen coords by the shader)
	 */
	float x, y;
	GameObject(Maze& maze, float x, float y);
	void render(Shader shader);
	bool checkCollision(const GameObject &other) const;
	virtual unsigned int getTexture();
	virtual void loadTextures();
};

/**
 * Represents a game object that moves
 */
class MoveableGameObject : public GameObject
{
public:
	void updatePosition(float deltaTime);
	float xVel, yVel;
	MoveableGameObject(Maze& maze, float x, float y);
	MoveableGameObject(Maze& maze, float x, float y, float xVel, float yVel);
};

/**
 * Represents a player controlled object
 */
class PlayerObject : public MoveableGameObject
{
public:
	static bool noclip;
	PlayerObject(Maze& maze, float x, float y);
	static float moveSpeed;
	void processInput(GLFWwindow *window, float deltaTime);
};

/**
 * Represents a computer controlled object
 */
class AIObject : public MoveableGameObject
{
private:
	Path *path = NULL;
public:
	AIObject(Maze& maze, float x, float y);
	AIObject(Maze& maze, float x, float y, float xVel, float yVel);
	static bool showPath;
	static float moveSpeed;
	GameObject *target = NULL;
	void setTarget(GameObject *target);
	void render(Shader shader, int pathColor);
	void step();
};

#endif /* _OBJECT_H_ */
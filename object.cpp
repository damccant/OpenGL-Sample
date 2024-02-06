#include "object.h"

unsigned int GameObject::texture;

void GameObject::loadTextures()
{
	texture = createUndefinedTexture();
}

unsigned int GameObject::getTexture()
{
	return texture;
}

GameObject::GameObject(Maze& maze, float x, float y)
{
	this->maze = &maze;
	this->x = x;
	this->y = y;
}
MoveableGameObject::MoveableGameObject(Maze& maze, float x, float y) : GameObject(maze, x, y), xVel(0.0f), yVel(0.0f) {}
MoveableGameObject::MoveableGameObject(Maze& maze, float x, float y, float xVel, float yVel) : GameObject(maze, x, y), xVel(xVel), yVel(yVel) {}
PlayerObject::PlayerObject(Maze& maze, float x, float y) : MoveableGameObject(maze, x, y) {}
AIObject::AIObject(Maze& maze, float x, float y) : MoveableGameObject(maze, x, y) {}
AIObject::AIObject(Maze& maze, float x, float y, float xVel, float yVel) : MoveableGameObject(maze, x, y, xVel, yVel) {}

/**
 * Checks if two objects have collided using AABB - AABB collision
 */
bool GameObject::checkCollision(const GameObject &other) const
{
	float size = 0.3f;
	bool collisionX = x + size >= other.x - size && other.x + size >= x - size;
	bool collisionY = y + size >= other.y - size && other.y + size >= y - size;
	return collisionX && collisionY;
}

void GameObject::render(Shader shader)
{
	glBindTexture(GL_TEXTURE_2D, getTexture());
	// use the shader program
	shader.use();
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(
		2.0f * x / maze->width - 1.0f,
		2.0f * y / maze->height - 1.0f,
		0.0f));

	trans = glm::scale(trans, glm::vec3(1.0f / maze->width, 1.0f / maze->height, 1.0f));
	shader.setValue("transform", trans);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void MoveableGameObject::updatePosition(float deltaTime)
{
	x += xVel * deltaTime;
	y += yVel * deltaTime;
}

/**
 * Whether or not to show the path
 */
bool AIObject::showPath = false;

/**
 * Override GameObject to show the path we are walking along
 */
void AIObject::render(Shader shader, int pathColor)
{
	if(showPath && path != NULL)
		path->render(shader, pathColor);
	GameObject::render(shader);
}

float PlayerObject::moveSpeed = 2.5f;
bool PlayerObject::noclip = false;

/**
 * Process the keyboard input on the player's character
 * 
 * We need deltaTime to calculate the next coordinates we are about to move to and make sure they are not in a wall
 */
void PlayerObject::processInput(GLFWwindow *window, float deltaTime)
{
	xVel = yVel = 0;

	// first, get the user's intended direction
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		yVel += moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		yVel -= moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		xVel += moveSpeed;
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		xVel -= moveSpeed;

	// prevent the player from moving off the screen
	if(x + 0.5f >= maze->width)
		xVel = std::min(xVel, 0.0f);
	if(x - 0.5f <= 0)
		xVel = std::max(xVel, 0.0f);
	if(y + 0.5f >= maze->height)
		yVel = std::min(yVel, 0.0f);
	if(y - 0.5f <= 0)
		yVel = std::max(yVel, 0.0f);

	// prevent the player from going thru walls
	if(!noclip)
	{
		if(!maze->areNodesAdjacent(x, y, x+(xVel*deltaTime), y))
			xVel = 0.0f;
		if(!maze->areNodesAdjacent(x, y, x, y+(yVel*deltaTime)))
			yVel = 0.0f;
	}
}

void AIObject::setTarget(GameObject* object)
{
	target = object;
}

float AIObject::moveSpeed = 2.0f;

/**
 * Called on each frame to update the AI's goals
 */
void AIObject::step()
{
	// if there is no target, do nothing and stop here
	if(target == NULL)
		return;

	// if there is a target but no path, create the path
	if(path == NULL)
		path = new Path(*maze, Coord(x, y), Coord(target->x, target->y));
	else // there is a target, so update the path
		path->findRoute(Coord(x, y), Coord(target->x, target->y));


	if(path != NULL)
	{
		// go to the next waypoint
		xVel = yVel = 0.0f;
		bool r = false;
		float tolerance = 0.5f;
		while(true)
		{
			// go directly to the target
			float tx = target->x, ty = target->y;

			// if there is a path, follow that instead
			if(!path->path.empty())
			{
				Coord c = path->path.front();
				tx = c.first + 0.5f, ty = c.second + 0.5f;
			}

			if(x - tx > tolerance) // my x is too high, go left
				xVel = -moveSpeed;
			if(tx - x > tolerance) // my x is too low, go right
				xVel = moveSpeed;
			if(y - ty > tolerance) // my y is too high, go down
				yVel = -moveSpeed;
			if(ty - y > tolerance) // my y is too low, go up
				yVel = moveSpeed;
			// if the next waypoint is too close, remove it and go to the one after it
			if(xVel == 0.0f && yVel == 0.0f && !path->path.empty() && !r)
			{
				r = true;
				tolerance = 0.2f;
				path->path.erase(path->path.begin());
			}
			else
				break; // we are en route to the next waypoint
		}
	}
}


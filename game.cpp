#include "game.h"

Cheese::Cheese(Maze& maze, float x, float y) : AIObject(maze, x, y) {}
Mouse::Mouse(Maze& maze, float x, float y) : AIObject(maze, x, y) {}
Cat::Cat(Maze& maze, float x, float y) : PlayerObject(maze, x, y) {}

unsigned int Cheese::cheese_tex;

unsigned int Cheese::getTexture()
{
	return cheese_tex;
}

void Cheese::loadTextures(const std::string& path)
{
	cheese_tex = createTexture(path, GL_RGBA);
}

unsigned int Mouse::mouse_tex;

unsigned int Mouse::getTexture()
{
	return mouse_tex;
}

void Mouse::loadTextures(const std::string& path)
{
	mouse_tex = createTexture(path, GL_RGBA);
}

unsigned int Cat::cat_tex;

unsigned int Cat::getTexture()
{
	return cat_tex;
}

void Cat::loadTextures(const std::string& path)
{
	cat_tex = createTexture(path, GL_RGBA);
}

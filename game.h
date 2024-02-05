#ifndef _GAME_H_
#define _GAME_H_

#include "object.h"

class Cheese : public PlayerObject
{
private:
	static unsigned int cheese_tex;
public:
	Cheese(Maze& maze, float x, float y);
	unsigned int getTexture();
	static void loadTextures(const std::string& path);
};

class Mouse : public AIObject
{
private:
	static unsigned int mouse_tex;
public:
	Mouse(Maze& maze, float x, float y);
	unsigned int getTexture();
	static void loadTextures(const std::string& path);
};

class Cat : public AIObject
{
private:
	static unsigned int cat_tex;
public:
	Cat(Maze& maze, float x, float y);
	unsigned int getTexture();
	static void loadTextures(const std::string& path);
};

#endif /* _GAME_H_ */
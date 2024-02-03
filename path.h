#ifndef _PATH_H_
#define _PATH_H_

#include "maze.h"
#include <math.h>
#include <queue>
#include <unordered_map>
#include <initializer_list>
#include <algorithm>

#include "shader.h"
#include "texture.h"

class Path
{
private:
	//Maze const& maze;
	const Maze* maze;
	Coord from;
	Coord to;
	static std::vector<unsigned int> textures;
public:
	Path(const Maze& maze, const Coord from, const Coord to);
	std::deque<Coord> path;
	std::deque<Coord> findRoute(const Coord from, const Coord to);
	void render(Shader shader, int texture) const;
	static void loadPathTextures();
};

#endif /* _PATH_H_ */
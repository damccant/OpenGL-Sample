#ifndef _PATH_H_
#define _PATH_H_

#include "maze.h"

class Path
{
private:
	Maze theMaze;
	Coord src;
	Coord dest;
public:
	Path(const Maze& maze, const Coord src, const Coord dest);
	std::vector<Coord> path;
	Path& findRoute(const Coord src, const Coord dest);
}

#endif /* _PATH_H_ */
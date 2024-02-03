#ifndef _MAZE_H_
#define _MAZE_H_

#include <cstddef>
#include <unordered_set>
#include <stack>
#include <iostream>

typedef unsigned int scalar;
//typedef std::pair<scalar, scalar> Coord;
class Coord
{
public:
	scalar first, second;
	Coord(scalar f, scalar s) : first(f), second(s) {};
	Coord() : first(-1), second(-1) {};
	friend bool operator==(const Coord& lhs, const Coord& rhs)
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}
	friend std::ostream& operator<<(std::ostream& output, const Coord& rhs)
	{
		output << "(" << rhs.first << ", " << rhs.second << ")";
		return output;
	}
};

struct CoordHashFunction
{
	size_t operator() (const Coord &x) const
	{
		return x.first ^ x.second;
	}
};

typedef std::unordered_set<Coord, CoordHashFunction> SetOfCoord;
typedef std::stack<Coord> StackOfCoord;

// used by the maze generation
#define VISIT 0b00000100

#define UP    0b00000010
#define RIGHT 0b00000001

#include "shader.h"
#include "texture.h"

class Maze
{
public:
	// adjacency graph
	// 0 0 0 0 left up down right
	unsigned char **nodes;
	scalar width;
	scalar height;
	bool areCoordsValid(const Coord c) const;
	bool areCoordsValid(const scalar x, const scalar y) const;
	SetOfCoord getAdjacentNodes(const Coord c) const;
	SetOfCoord getAdjacentNodes(const scalar x, const scalar y) const;
	bool areNodesAdjacent(const Coord c1, const Coord c2) const;
	bool areNodesAdjacent(const scalar x1, const scalar y1, const scalar x2, const scalar y2) const;
	bool removeWallBetween(const Coord c1, const Coord c2);
	bool removeWallBetween(const scalar x1, const scalar y1, const scalar x2, const scalar y2);
	bool coordHasUnvisitedNeighbor(const Coord c) const;
	bool coordHasUnvisitedNeighbor(const scalar x, const scalar y) const;
	Coord randomUnvisitedNeighbor(const Coord c) const;
	Coord randomUnvisitedNeighbor(const scalar x, const scalar y) const;
	void unvisitAll(void);

	Maze(const scalar width, const scalar height);

	// the rule of three
	Maze(const Maze& m);
	Maze& operator=(const Maze& that);
	~Maze();

	void render(Shader shader) const;
	static void loadMazeTextures(const std::string &path);
	void generate(void);
	void clearMaze(void);
private:
	static unsigned int textures[4];
	void renderTile(Shader shader, const scalar x, const scalar y) const;
};

#endif /* _MAZE_H_ */
#ifndef _MAZE_H_
#define _MAZE_H_

#include <cstddef>
#include <unordered_set>
#include <stack>

typedef unsigned int scalar;
typedef std::pair<scalar, scalar> Coord;

struct hashFunction
{
	size_t operator() (const Coord &x) const
	{
		return x.first ^ x.second;
	}
};

typedef std::unordered_set<Coord, hashFunction> SetOfCoord;
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
	bool areCoordsValid(const Coord c);
	bool areCoordsValid(const scalar x, const scalar y);
	SetOfCoord getAdjacentNodes(const Coord c);
	SetOfCoord getAdjacentNodes(const scalar x, const scalar y);
	bool areNodesAdjacent(const Coord c1, const Coord c2);
	bool areNodesAdjacent(const scalar x1, const scalar y1, const scalar x2, const scalar y2);
	bool removeWallBetween(const Coord c1, const Coord c2);
	bool removeWallBetween(const scalar x1, const scalar y1, const scalar x2, const scalar y2);
	bool coordHasUnvisitedNeighbor(const Coord c);
	bool coordHasUnvisitedNeighbor(const scalar x, const scalar y);
	Coord randomUnvisitedNeighbor(const Coord c);
	Coord randomUnvisitedNeighbor(const scalar x, const scalar y);
	void unvisitAll(void);

	Maze(const scalar width, const scalar height);
	Maze(const Maze& m);
	~Maze();

	void render(Shader shader);
	static void loadMazeTextures(const std::string &path);
	void generate(void);
	void clearMaze(void);
private:
	static unsigned int textures[4];
	void renderTile(Shader shader, const scalar x, const scalar y);
};

#endif /* _MAZE_H_ */
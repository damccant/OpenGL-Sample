#include "maze.h"

inline static const Coord mkCoord(const scalar x, const scalar y)
{
	return Coord(x, y);
}

bool Maze::areCoordsValid(const Coord c) const
{
	return areCoordsValid(c.first, c.second);
}

bool Maze::areCoordsValid(const scalar x, const scalar y) const
{
	return x >= 0 && x < width && y >= 0 && y < height;
}

SetOfCoord Maze::getAdjacentNodes(const Coord c) const
{
	return getAdjacentNodes(c.first, c.second);
}

SetOfCoord Maze::getAdjacentNodes(const scalar x, const scalar y) const
{
	SetOfCoord adjacent;
	if(!areCoordsValid(x, y))
		return adjacent;
	if(x > 0 && nodes[x-1][y] & RIGHT)
		adjacent.insert(mkCoord(x-1, y));
	if(x < width - 1 && nodes[x][y] & RIGHT)
		adjacent.insert(mkCoord(x+1, y));
	if(y > 0 && nodes[x][y - 1] & UP)
		adjacent.insert(mkCoord(x, y-1));
	if(y < height - 1 && nodes[x][y] & UP)
		adjacent.insert(mkCoord(x, y+1));
	return adjacent;
}

bool Maze::areNodesAdjacent(const Coord c1, const Coord c2) const
{
	return areNodesAdjacent(c1.first, c1.second, c2.first, c2.second);
}

bool Maze::areNodesAdjacent(const scalar x1, const scalar y1, const scalar x2, const scalar y2) const
{
	if(!areCoordsValid(x1, y1) || !areCoordsValid(x2, y2))
		return false;
	if(x1 == x2)
		if(y1 == y2)
			return true; // coords are the same(?), therefore adjacent
		else if(y2 - y1 == 1 && nodes[x1][y1] & UP)
			return true;
		else if(y1 - y2 == 1 && nodes[x1][y2] & UP)
			return true;
		else
			return false;
	else if(y1 == y2)
		if(x1 == x2)
			return true; // this should never happen
		else if(x2 - x1 == 1 && nodes[x1][y1] & RIGHT)
			return true;
		else if(x1 - x2 == 1 && nodes[x2][y1] & RIGHT)
			return true;
		else
			return false;
	return false;
}

bool Maze::removeWallBetween(const Coord c1, const Coord c2)
{
	return removeWallBetween(c1.first, c1.second, c2.first, c2.second);
}

bool Maze::removeWallBetween(const scalar x1, const scalar y1, const scalar x2, const scalar y2)
{
	if(!areCoordsValid(x1, y1) || !areCoordsValid(x2, y2))
		return false;
	if(x1 == x2)
		if(y1 == y2)
			return false; // coords are the same(?), how do we remove the wall?
		else if(y2 - y1 == 1)
		{
			nodes[x1][y1] |= UP;
			return true;
		}
		else if(y1 - y2 == 1)
		{
			nodes[x1][y2] |= UP;
			return true;
		}
		else
			return false;
	else if(y1 == y2)
		if(x1 == x2)
			return false; // this should never happen
		else if(x2 - x1 == 1)
		{
			nodes[x1][y1] |= RIGHT;
			return true;
		}
		else if(x1 - x2 == 1)
		{
			nodes[x2][y1] |= RIGHT;
			return true;
		}
		else
			return false;
	return false;
}

void Maze::clearMaze(void)
{
	for(scalar x = 0; x < width; x++)
		for(scalar y = 0; y < height; y++)
			nodes[x][y] = 0;
}

Maze::Maze(const scalar width, const scalar height)
{
	this->width = width;
	this->height = height;
	nodes = new unsigned char*[width];
	for(scalar x = 0; x < width; x++)
	{
		nodes[x] = new unsigned char[height];
		for(scalar y = 0; y < height; y++)
			nodes[x][y] = 0;
	}
	generate();
}

Maze::Maze(const Maze& other)
{
	this->width = other.width;
	this->height = other.height;
	nodes = new unsigned char*[width];
	for(scalar x = 0; x < width; x++)
	{
		nodes[x] = new unsigned char[height];
		for(scalar y = 0; y < height; y++)
			nodes[x][y] = other.nodes[x][y];
	}
}

Maze& Maze::operator=(const Maze& that)
{
	unsigned char **local_nodes = new unsigned char*[width];
	for(scalar x = 0; x < width; x++)
	{
		local_nodes[x] = new unsigned char[height];
		for(scalar y = 0; y < height; y++)
			local_nodes[x][y] = that.nodes[x][y];
	}
	for(scalar x = 0; x < width; x++)
		delete[] nodes[x];
	delete[] nodes;
	nodes = local_nodes;
	return *this;
}


Maze::~Maze()
{
	for(scalar x = 0; x < width; x++)
		delete[] nodes[x];
	delete[] nodes;
}

bool Maze::coordHasUnvisitedNeighbor(const Coord c) const
{
	return coordHasUnvisitedNeighbor(c.first, c.second);
}

bool Maze::coordHasUnvisitedNeighbor(const scalar x, const scalar y) const
{
	if(!areCoordsValid(x, y))
		return false;
	if(x > 0 && ~nodes[x-1][y] & VISIT)
		return true;
	if(x < width - 1 && ~nodes[x+1][y] & VISIT)
		return true;
	if(y > 0 && ~nodes[x][y-1] & VISIT)
		return true;
	if(y < height - 1 && ~nodes[x][y+1] & VISIT)
		return true;
	return false;
}

Coord Maze::randomUnvisitedNeighbor(const Coord c) const
{
	return randomUnvisitedNeighbor(c.first, c.second);
}

Coord Maze::randomUnvisitedNeighbor(const scalar x, const scalar y) const
{
	if(!areCoordsValid(x, y))
		return mkCoord(-1, -1);
	
	int neighbors = 0;
	if(x > 0 && ~nodes[x-1][y] & VISIT)
		neighbors++;
	if(x < width - 1 && ~nodes[x+1][y] & VISIT)
		neighbors++;
	if(y > 0 && ~nodes[x][y-1] & VISIT)
		neighbors++;
	if(y < height - 1 && ~nodes[x][y+1] & VISIT)
		neighbors++;
	neighbors -= rand() % neighbors;
	if(x > 0 && ~nodes[x-1][y] & VISIT && --neighbors == 0)
		return mkCoord(x-1, y);
	if(x < width - 1 && ~nodes[x+1][y] & VISIT && --neighbors == 0)
		return mkCoord(x+1, y);
	if(y > 0 && ~nodes[x][y-1] & VISIT && --neighbors == 0)
		return mkCoord(x, y-1);
	if(y < height - 1 && ~nodes[x][y+1] & VISIT && --neighbors == 0)
		return mkCoord(x, y+1);
	// this should never happen
	std::cerr << "If you are seeing this message, there is a bug in Maze::randomUnvisitedNeighbor()!" << std::endl;
	return mkCoord(-1, -1);
}

void Maze::unvisitAll(void)
{
	for(scalar x = 0; x < width; x++)
		for(scalar y = 0; y < height; y++)
			nodes[x][y] &= ~VISIT;
}

/**
 * Generates the maze using iterative randomized depth-first search
 */
void Maze::generate(void)
{
	srand(time(0));
	nodes[0][0] |= VISIT;
	StackOfCoord stack;
	stack.push(mkCoord(0,0));
	while(!stack.empty())
	{
		Coord curr = stack.top();
		stack.pop();
		if(coordHasUnvisitedNeighbor(curr))
		{
			stack.push(curr);
			Coord chosen = randomUnvisitedNeighbor(curr);
			removeWallBetween(curr, chosen);
			nodes[chosen.first][chosen.second] |= VISIT;
			stack.push(chosen);
		}
	}
	unvisitAll();
}

unsigned int Maze::textures[4] = {0, 0, 0, 0};

void Maze::loadMazeTextures(const std::string &path)
{
	for(int x = 0; x <= 3; x++)
		textures[x] = createTexture(path + "maze" + std::to_string(x) + ".png");
}

void Maze::render(Shader shader) const
{
	for(scalar x = 0; x < width; x++)
		for(scalar y = 0; y < height; y++)
			renderTile(shader, x, y);
}

void Maze::renderTile(Shader shader, const scalar x, const scalar y) const
{
	shader.use();
	glBindTexture(GL_TEXTURE_2D, textures[nodes[x][y] % 4]);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(2.0f * x / width - (1.0f - 1.0f / width), 2.0f * y / height - (1.0f - 1.0f / height), -1.0f));
	trans = glm::scale(trans, glm::vec3(1.0f / width, 1.0f / height, 1.0f));
	shader.setValue("transform", trans);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
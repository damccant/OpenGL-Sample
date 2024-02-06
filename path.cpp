#include "path.h"

Path::Path(const Maze& maze, const Coord from, const Coord to)
{
	this->maze = &maze;
	findRoute(from, to);
}

inline static const float estimateDistance(const Coord from, const Coord to)
{
	float x1 = from.first, y1 = from.second, x2 = to.first, y2 = to.second;
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

struct CoordAndEstDist
{
	Coord coord;
	float distance;

	/**
	 * The priority queue returns the item with the highest priority,
	 * but for us the "highest priority" item is actually the "smallest distance",
	 * so we need to swap the direction of the comparison.
	 */
	friend bool operator<(CoordAndEstDist const& lhs, CoordAndEstDist const& rhs)
	{
		return lhs.distance > rhs.distance;
	}
	CoordAndEstDist(float distFromStart, const Coord coord, const Coord to)
	{
		this->coord = coord;
		distance = distFromStart + estimateDistance(coord, to);
	}
};

std::deque<Coord> reconstruct_path(std::unordered_map<Coord, Coord, CoordHashFunction> cameFrom, const Coord from, const Coord to)
{
	std::deque<Coord> path;
	Coord current = to;
	while(! (current == from))
	{
		path.push_front(current);
		current = cameFrom[current];
	}
	path.push_front(from);
	return path;
}

std::deque<Coord> Path::findRoute(const Coord from, const Coord to)
{
	this->from = from;
	this->to = to;

	std::vector<CoordAndEstDist> openSet;
	std::make_heap(openSet.begin(), openSet.end());

	openSet.push_back(CoordAndEstDist(0.0, from, to));
	std::push_heap(openSet.begin(), openSet.end());

	std::unordered_map<Coord, Coord, CoordHashFunction> cameFrom;

	std::unordered_map<Coord, float, CoordHashFunction> gScore;
	gScore[from] = 0.0f;

	while(!openSet.empty())
	{
		std::pop_heap(openSet.begin(), openSet.end());
		Coord current = openSet.back().coord;
		if(current == to)
			return path = reconstruct_path(cameFrom, from, to);
		openSet.pop_back();

		SetOfCoord neighbors = maze->getAdjacentNodes(current);
		for(const Coord neighbor : neighbors)
		{
			float tentative_gScore = gScore[current] + 1;
			gScore.emplace(neighbor, 999999999.0f);
			if(tentative_gScore < gScore[neighbor])
			{
				cameFrom[neighbor] = current;
				gScore[neighbor] = tentative_gScore;
				//fScore[neighbor] = tentative_gScore + estimateDistance(neighbor, to);
				auto it = openSet.begin();
				for(; it != openSet.end(); it++)
					if(it->coord == neighbor)
						break;
				if(it == openSet.end())
				{
					openSet.push_back(CoordAndEstDist(tentative_gScore, neighbor, to));
					std::push_heap(openSet.begin(), openSet.end());
				}
				else
				{
					it->distance = tentative_gScore + estimateDistance(neighbor, to);
					std::make_heap(openSet.begin(), openSet.end());
				}
			}
		}
	}
	std::cerr << "Warning: Could not find route between " << from << " and " << to << "!" << std::endl;
	std::deque<Coord> empty;
	return empty;
}

std::vector<unsigned int> Path::textures;

void Path::loadPathTextures()
{
	textures.push_back(createTextureOfColor(0, 0, 255));
	textures.push_back(createTextureOfColor(255, 0, 0));
	textures.push_back(createTextureOfColor(0, 255, 0));
}

void Path::render(Shader shader, int texture) const
{
	glBindTexture(GL_TEXTURE_2D, textures[texture]);
	shader.use();
	for(auto it = path.cbegin(); it != path.cend(); it++)
	{
		const unsigned int x = it->first, y = it->second;
		auto next = it + 1;
		const unsigned int nx = next->first, ny = next->second;
		glm::vec3 trans_base = glm::vec3(
			2.0f * x / maze->width - (1.0f - 1.0f / maze->width),
			2.0f * y / maze->height - (1.0f - 1.0f / maze->height),
			-0.9f);
		if(x == nx)
		{
			glm::vec3 scale = glm::vec3(0.1f / maze->width, 1.0f / maze->height, 1.0f);
			if(y == ny)
				; // ?
			else if(y - ny == 1) // down
			{
				trans_base -= glm::vec3(0.0f, 1.0f / maze->height, 0.0f);
				glm::mat4 trans = glm::mat4(1.0f);
				trans = glm::translate(trans, trans_base);
				trans = glm::scale(trans, scale);
				shader.setValue("transform", trans);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			else if(ny - y == 1) // up
			{
				trans_base += glm::vec3(0.0f, 1.0f / maze->height, 0.0f);
				glm::mat4 trans = glm::mat4(1.0f);
				trans = glm::translate(trans, trans_base);
				trans = glm::scale(trans, scale);
				shader.setValue("transform", trans);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			else
				; // ?
		}
		else if(y == ny)
		{
			glm::vec3 scale = glm::vec3(1.0f / maze->width, 0.1f / maze->height, 1.0f);
			if(x == nx)
				; // ?
			else if(x - nx == 1) // left
			{
				trans_base -= glm::vec3(1.0f / maze->width, 0.0f, 0.0f);
				glm::mat4 trans = glm::mat4(1.0f);
				trans = glm::translate(trans, trans_base);
				trans = glm::scale(trans, scale);
				shader.setValue("transform", trans);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
			else if(nx - x == 1) // right
			{
				trans_base += glm::vec3(1.0f / maze->width, 0.0f, 0.0f);
				glm::mat4 trans = glm::mat4(1.0f);
				trans = glm::translate(trans, trans_base);
				trans = glm::scale(trans, scale);
				shader.setValue("transform", trans);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}
		
		
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(
			2.0f * x / maze->width - (1.0f - 1.0f / maze->width),
			2.0f * y / maze->height - (1.0f - 1.0f / maze->height),
			-0.9f));
		trans = glm::scale(trans, glm::vec3(0.1f / maze->width, 0.1f / maze->height, 1.0f));
		shader.setValue("transform", trans);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}
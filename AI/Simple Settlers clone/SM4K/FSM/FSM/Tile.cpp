#include "Tile.h"

Tile::Tile(const glm::vec4 &color) : Object(color), building(false)
{
	treeAmount = 0;
	discovered = false;
}

Tile::~Tile()
{
}

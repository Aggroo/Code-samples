#include "TileHandler.h"
#include "shapeGenerators.hpp"
#include "AISystem.h"
#include "SettingsManager.h"
#include <algorithm>
#include "TreeEntity.h"
#include "GameHandler.h"



TileHandler* TileHandler::Instance()
{
	static TileHandler instance;
	return &instance;
}

void TileHandler::LoadMap(const char* filename, int h, int w, glm::vec2 tileSize, GLuint programID)
{
	Model waterTile = shapeGenerator::square(tileSize.x, tileSize.y, programID, "textures/water.png");
	Model swampTile = shapeGenerator::square(tileSize.x, tileSize.y, programID, "textures/swamp.png");
	Model cliffTile = shapeGenerator::square(tileSize.x, tileSize.y, programID, "textures/cliff.png");
	Model grassTile = shapeGenerator::square(tileSize.x, tileSize.y, programID, "textures/grass.png");
	Model buildingTile = shapeGenerator::square(tileSize.x, tileSize.y, programID, "textures/buildingConstruction.png");
	Model coalKilnConstuctTile = shapeGenerator::square(tileSize.x, tileSize.y, programID, "textures/CoalKilnConstruction.png");
	Model coalKilnTile = shapeGenerator::square(tileSize.x, tileSize.y, programID, "textures/CoalKilnProduction.png");
	Model treeTile = shapeGenerator::square(9, 10, programID, "textures/tree.png");
	Model stumpTile = shapeGenerator::square(9, 10, programID, "textures/treeStump.png");
	

	waterTile.init();
	swampTile.init();
	cliffTile.init();
	grassTile.init();
	buildingTile.init();
	coalKilnConstuctTile.init();
	coalKilnTile.init();
	treeTile.init();
	stumpTile.init();


	tiles.push_back(waterTile);
	tiles.push_back(swampTile);
	tiles.push_back(cliffTile);
	tiles.push_back(grassTile);
	tiles.push_back(treeTile);
	tiles.push_back(stumpTile);
	tiles.push_back(buildingTile);
	tiles.push_back(coalKilnConstuctTile);
	tiles.push_back(coalKilnTile);

	std::string cliff = "B";
	std::string water = "V";
	std::string trees = "T";
	std::string swamp = "G";
	std::string grass = "M";
	std::string start = "S";
	std::string goal = "D";

	mapSize = glm::vec2(w, h);
	int height = h;
	int width = w;

	std::string* mapArray = new std::string[height*width];

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			mapArray[y*width + x] = "";
		}
	}

	this->tileSize = tileSize;
	int squareSizeX = tileSize.x;
	int squareSizeY = tileSize.y;
	int midTileX = tileSize.x / 2;
	int midTileY = tileSize.y / 2;

	FILE* file;
	file = fopen(filename, "r");
	char c;
	int i = 0;
	while ((c = fgetc(file)) != EOF) {

		if (c != '\n')
		{
			mapArray[i] = c;
			i++;
		}

	}
	fclose(file);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Tile square = Tile(glm::vec4(1.0f, 1.0f, 1.0f, 0.1f));
			if (mapArray[y*width + x] == cliff)
			{
				square.setModel(&tiles[2]);
				square.SetType(tileType::cliff);
			}
			else if (mapArray[y*width + x] == water)
			{
				square.setModel(&tiles[0]);
				square.SetType(tileType::water);
			}
			else if (mapArray[y*width + x] == trees)
			{
				square.setModel(&tiles[3]);
				square.SetType(tileType::trees);
				for (int i = 0; i < 5; i++)
				{
					TreeEntity* trees = new TreeEntity(glm::vec4(0.5f, 0.5f, 0.5f, 0.2f));
					trees->setModel(&tiles[4]);
					trees->setPosition(AISystem::Instance()->randomizer((squareSizeX*x), (squareSizeX*x + tileSize.x)), AISystem::Instance()->randomizer((squareSizeX*y), (squareSizeX*y + tileSize.y)-5));
					GameHandler::Instance()->GetTreeList()->push_back(trees);
					square.GetTreeList()->push_back(trees);
				}
				square.setPosition(squareSizeX*x, squareSizeY*y);	
			}
			else if (mapArray[y*width + x] == swamp)
			{
				square.setModel(&tiles[1]);
				square.SetType(tileType::swamp);
			}
			else if (mapArray[y*width + x] == grass)
			{
				square.setModel(&tiles[3]);
				square.SetType(tileType::grass);
			}

			square.setPosition(squareSizeX*x, squareSizeY*y);
			map.push_back(square);
		}
	}
}

std::vector<Tile>* TileHandler::GetMap()
{
	return &map;
}


glm::vec2 TileHandler::GetMapSize()
{
	return mapSize;
}

Tile* TileHandler::GetTileAtPos(glm::vec2 pos)
{
	int nodeIndex;
	AISystem::Instance()->PointToIndex(pos, nodeIndex);
	return &map[nodeIndex];
}

Tile* TileHandler::GetTileAtPos(int index)
{
	return &map[index];
}

Model* TileHandler::GetTileModel(int index)
{
	if (index < tiles.size())
	{
		return &tiles[index];
	}
	else
	{
		return nullptr;
	}
	
}

NavType TileHandler::GetNavTypeFromTile(int index)
{
	if (map[index].GetType() == tileType::cliff)
		return NavType::cliff;
	else if (map[index].GetType() == tileType::water)
		return NavType::water;
	else if (map[index].GetType() == tileType::grass)
		return NavType::grass;
	else if (map[index].GetType() == tileType::trees)
		return NavType::trees;
	else if (map[index].GetType() == tileType::swamp)
		return NavType::swamp;
}

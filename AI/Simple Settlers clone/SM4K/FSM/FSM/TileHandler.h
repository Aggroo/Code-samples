#pragma once
//Glew
#include <GL\glew.h>

//GLFW
#include <GLFW\glfw3.h>

#include <glm.hpp>
#include <vector>
#include "Object.h"
#include "GraphNodeTypes.h"
#include "Tile.h"

/*****************************************/
/*			TileHandle - Singleton		 */
/*		Handles all tiles in a map		 */
/*****************************************/
class TileHandler
{
public:
	static TileHandler* Instance();

	//Loads a map
	void LoadMap(const char* filename, int h, int w, glm::vec2 tileSize, GLuint programID);

	//Gets a vector of the entire map
	std::vector<Tile>* GetMap();

	//Get a vector of the map size
	glm::vec2 GetMapSize();

	//Gets a Tile at a certain position
	Tile* GetTileAtPos(glm::vec2 pos);

	//Gets a Tile at a certain index
	Tile* GetTileAtPos(int index);

	//Gets a Model of a certain Tile
	Model* GetTileModel(int index);

	//Gets the type of a Tile
	NavType GetNavTypeFromTile(int index);

private:
	TileHandler(){}
	TileHandler(const TileHandler&) = delete;
	TileHandler& operator=(const TileHandler&) = delete;

	glm::vec2 tileSize;
	glm::vec2 mapSize;

	std::vector<Tile> map;
	std::vector<Model> tiles;
	
};


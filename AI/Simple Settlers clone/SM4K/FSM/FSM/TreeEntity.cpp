#include "TreeEntity.h"
#include "TileHandler.h"


TreeEntity::TreeEntity(const glm::vec4 &color) : Object(color)
{
	harvested = false;
}

TreeEntity::~TreeEntity()
{
}

bool TreeEntity::isHarvested()
{
	return harvested;
}

void TreeEntity::Harvested()
{
	harvested = true;
	SetModel(TileHandler::Instance()->GetTileModel(5));
}

#pragma once
//Glew
#include <GL\glew.h>

//GLFW
#include <GLFW\glfw3.h>

#include <glm.hpp>
#include "Unit.h"
#include <map>
class UnitHandler
{
public:
	static UnitHandler* Instance();

	void CreateUnit(Model* model, glm::vec2 position, glm::vec4 color, profession prof);

	void CreateDummy(Model* model, glm::vec2 position, glm::vec4 color, profession prof);
	std::vector<Unit*>* GetUnits();
	Unit* GetUnit(int ID);

	void SetProgramID(GLuint programID);

	void SetupUnitModels();
	Model* GetUnitModel(profession prof);

	glm::vec2 GetHomePosition() const { return homePosition; }
	void SetHomePosition(glm::vec2 val) { homePosition = val; }

private:
	UnitHandler();
	~UnitHandler();
	UnitHandler(const UnitHandler&) = delete;
	UnitHandler& operator=(const UnitHandler&) = delete;

	std::vector<Unit*> objects;
	std::map<profession, Model> unitModels;

	int id;
	GLuint programID;
	glm::vec2 homePosition;

};


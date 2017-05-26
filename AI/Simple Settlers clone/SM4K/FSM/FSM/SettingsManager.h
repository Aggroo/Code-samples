#pragma once
#include "SettingsReader.h"
#include <map>
class SettingsReader;

/***********************************************/
/*	 Gives easy access to the SettingsReader   */
/***********************************************/
class SettingsManager
{
public:
	//Singleton class
	static SettingsManager* Instance();

	//Adds a SettingsReader to the readerMap
	void CreateReader(std::string filename, std::string readerName);
	//Gets a SettingsReader from the readerMap
	SettingsReader GetReader(std::string readerName);
private:
	SettingsManager(){}
	SettingsManager(const SettingsManager&) = delete;
	SettingsManager& operator=(const SettingsManager&) = delete;

	std::map<std::string, SettingsReader> readerMap;
};


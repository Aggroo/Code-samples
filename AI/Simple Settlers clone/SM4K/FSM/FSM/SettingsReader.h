#pragma once
#include <map>
/**************************************/
/* Class used for reading a .ini file */
/**************************************/
class SettingsReader
{
public:
	SettingsReader();
	~SettingsReader();

	//Reads a file and saves the values
	void ReadFile(std::string filename);

	//Used for the SettingsManager
	static SettingsReader Read(std::string filename);

	//Used when saving a value to check which section the value is in
	std::string CreateSectionKey(std::string section, std::string key);

	//Gets values
	std::string GetString(std::string defaultValue, std::string section, std::string key);
	int GetInt(int defaultValue, std::string section, std::string key);
	float GetFloat(float defaultValue, std::string section, std::string key);
	bool GetBool(bool defaultValue, std::string section, std::string key);
	
private:
	std::map<std::string, std::string> stringMap;
	std::map<std::string, float> floatMap;
	std::map<std::string, int> intMap;
	std::map<std::string, bool> boolMap;


};


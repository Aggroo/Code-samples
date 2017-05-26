#include "SettingsReader.h"
#include <iostream>
#include <fstream>
#include <string>


SettingsReader::SettingsReader()
{
}


SettingsReader::~SettingsReader()
{
}

std::string SettingsReader::CreateSectionKey(std::string section, std::string key)
{
	return (section + "_" + key);
}

void SettingsReader::ReadFile(std::string filename)
{
	try
	{
		std::string currentSection = "";
		std::ifstream file(filename);
		for (std::string line; std::getline(file, line);)
		{
			if (line == "")
				continue;
			else if (line.find_first_of(";") != std::string::npos)
				continue;
			else if (line.find_first_of("@") != std::string::npos)
			{
				currentSection = line.substr(1);
			}
			else
			{
				std::string key = line.substr(0, line.find("="));
				std::string value = line.substr(line.find("=")+1);
				
				if (value.find_first_of("\"") != std::string::npos && value.find_last_of("\"") != std::string::npos)
				{
					std::size_t length = value.find_last_of("\"") - value.find_first_of("\"");
					std::string mapKey = CreateSectionKey(currentSection, key);
					stringMap[mapKey] = value.substr(1, length - 1);
				}
				else if (value.find_first_of(".") != std::string::npos)
				{
					std::string mapKey = CreateSectionKey(currentSection, key);
					floatMap[mapKey] = std::stof(value);
				}
				else if (value == "true" || value == "false")
				{
					std::string mapKey = CreateSectionKey(currentSection, key);
					if (value == "true")
					{
						boolMap[mapKey] = true;
					}
					else if (value == "false")
					{
						boolMap[mapKey] = false;
					}
				}
				else
				{
					std::string mapKey = CreateSectionKey(currentSection, key);
					intMap[mapKey] = std::stoi(value);
				}
			}
		}
		file.close();
	}
	catch (std::exception e)
	{
		printf("Error: %ld", e);
	}

}

SettingsReader SettingsReader::Read(std::string filename)
{
	SettingsReader temp;
	temp.ReadFile(filename);
	return temp;
}
std::string SettingsReader::GetString(std::string defaultValue, std::string section, std::string key)
{
	if (stringMap[CreateSectionKey(section, key)] != "")
	{
		return stringMap[CreateSectionKey(section, key)];
	}
	else
		return defaultValue;
}

int SettingsReader::GetInt(int defaultValue, std::string section, std::string key)
{
	if (intMap[CreateSectionKey(section, key)] != NULL)
	{
		return intMap[CreateSectionKey(section, key)];
	}
	else
		return defaultValue;
}

bool SettingsReader::GetBool(bool defaultValue, std::string section, std::string key)
{
	if (boolMap[CreateSectionKey(section, key)] != NULL)
	{
		return boolMap[CreateSectionKey(section, key)];
	}
	else
		return defaultValue;
}

float SettingsReader::GetFloat(float defaultValue, std::string section, std::string key)
{
	if (floatMap[CreateSectionKey(section, key)] != NULL)
	{
		return floatMap[CreateSectionKey(section, key)];
	}
	else
		return defaultValue;
}


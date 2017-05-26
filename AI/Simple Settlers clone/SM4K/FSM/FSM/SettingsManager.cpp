#include "SettingsManager.h"


SettingsManager* SettingsManager::Instance()
{
	static SettingsManager instance;
	return &instance;
}

void SettingsManager::CreateReader(std::string filename, std::string readerName)
{
	readerMap[readerName] = SettingsReader::Read(filename);
}

SettingsReader SettingsManager::GetReader(std::string readerName)
{
	return readerMap[readerName];
}


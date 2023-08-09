#include "stdafx.h"
#include "LevelExtractor.h"

std::vector<std::wstring> LevelExtractor::m_AllLevelsPath(20);
LevelExtractor::LevelExtractor()
{
}

void LevelExtractor::ReadAndUpdateLevelsFromFile()
{
	const std::string path{ "Resources/Levels/Gameplay/" };
	m_AllLevelsPath.clear();
	try
	{
		for (const auto& entry : fs::directory_iterator(path))
		{
			if (entry.is_regular_file() && entry.path().extension() == ".bmp")
			{
				m_AllLevelsPath.push_back(entry.path().c_str());
			}
		}
	}
	catch (const fs::filesystem_error& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

void LevelExtractor::BuildAvailableLevels()
{
}

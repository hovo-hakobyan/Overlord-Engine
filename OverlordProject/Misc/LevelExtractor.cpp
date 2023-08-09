#include "stdafx.h"
#include "LevelExtractor.h"


std::vector<std::wstring> LevelExtractor::m_AllLevelsPath(20);
int LevelExtractor::SELECTED_LEVEL_IDX{ 0 };
int LevelExtractor::m_MaxLevelNum{ 28 };
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
				std::wstring newPath = entry.path().c_str();
				std::wstring searchTerm = L"Resources/";

				size_t pos = newPath.find(searchTerm);
				if (pos != std::wstring::npos)
				{
					newPath.erase(pos, searchTerm.length());
				}

				m_AllLevelsPath.push_back(newPath);
			}

			if (m_AllLevelsPath.size() >= m_MaxLevelNum)
			{
				break;
			}
		}
	}
	catch (const fs::filesystem_error& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

std::vector<std::wstring> LevelExtractor::GetAllLevelsPath()
{
	return m_AllLevelsPath;
}


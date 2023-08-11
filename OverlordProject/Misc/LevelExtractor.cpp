#include "stdafx.h"
#include "LevelExtractor.h"


std::vector<std::string> LevelExtractor::m_AllLevelsPath(20);

int LevelExtractor::m_MaxLevelNum{ 28 };
int LevelExtractor::m_SelectedLevelIdx{ -1 };
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
				std::string newPath = entry.path().string();
				std::string searchTerm = "Resources/";

				size_t pos = newPath.find(searchTerm);
				if (pos != std::string::npos)
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

std::vector<std::string> LevelExtractor::GetAllLevelsPath()
{
	return m_AllLevelsPath;
}

std::string LevelExtractor::GetLevelPath(int levelIdx)
{
	if (m_AllLevelsPath.size() ==0 || levelIdx > m_AllLevelsPath.size() -1)
	{
		return "";
	}

	
	
	return m_AllLevelsPath[levelIdx];
}

int LevelExtractor::GetSelectedLevelIdx()
{
	return m_SelectedLevelIdx;
}

void LevelExtractor::SetSelectedLevelIdx(int idx)
{
	m_SelectedLevelIdx = idx;
}


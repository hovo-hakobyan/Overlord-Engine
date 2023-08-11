#pragma once
class LevelExtractor final
{
public:
	LevelExtractor();
	~LevelExtractor() = default;
	LevelExtractor(const LevelExtractor& other) = delete;
	LevelExtractor(LevelExtractor&& other) noexcept = delete;
	LevelExtractor& operator=(const LevelExtractor& other) = delete;
	LevelExtractor& operator=(LevelExtractor&& other) noexcept = delete;

	static void ReadAndUpdateLevelsFromFile();
	static std::vector<std::string> GetAllLevelsPath();
	static std::string GetLevelPath(int levelIdx);
	static int GetSelectedLevelIdx();
	static void SetSelectedLevelIdx(int idx);
private:
	static std::vector<std::string> m_AllLevelsPath;
	static int m_MaxLevelNum;
	static int m_SelectedLevelIdx;
};


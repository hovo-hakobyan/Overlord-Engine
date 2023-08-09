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
	static std::vector<std::wstring> GetAllLevelsPath();

	static int SELECTED_LEVEL_IDX;
private:
	static std::vector<std::wstring> m_AllLevelsPath;
	static int m_MaxLevelNum;
};


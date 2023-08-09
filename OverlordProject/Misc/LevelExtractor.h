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
	static void BuildAvailableLevels();

private:
	static std::vector<std::wstring> m_AllLevelsPath;
};


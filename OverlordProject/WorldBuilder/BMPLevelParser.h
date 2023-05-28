#pragma once

struct Tiles
{
	//Corresponds with red channel of BMP
	const int ground = 0;
	const int solidWall = 253;
	const int brickWall = 226;
	const int grass = 21;
	const int water = 38;
};

enum class TileTypes
{
	Ground,SolidWall,BrickWall,Grass,Water
};
typedef std::vector<TileTypes*> TileCollection;
class BMPLevelParser final
{
public:
	BMPLevelParser();
	~BMPLevelParser() = default;
	BMPLevelParser(const BMPLevelParser& other) = delete;
	BMPLevelParser(BMPLevelParser&& other) noexcept = delete;
	BMPLevelParser& operator=(const BMPLevelParser& other) = delete;
	BMPLevelParser& operator=(BMPLevelParser&& other) noexcept = delete;

	TileCollection GetLevelData(const std::string& filePath, uint8_t rows, uint8_t cols);
private:

};


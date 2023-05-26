#include "stdafx.h"
#include "BMPLevelParser.h"
#include "External/CImg/CImg.h"

using namespace cimg_library;


BMPLevelParser::BMPLevelParser() 
{

}

const std::vector<TileTypes> BMPLevelParser::GetLevelData(const std::string& filePath, uint8_t rows, uint8_t cols)
{
	CImg<unsigned char> img(filePath.c_str());

	const int imgHeight = img.height();
	const int imgWidth = img.width();

	const float tileWidth = static_cast<float>(imgWidth) / cols;
	const float tileHeight = static_cast<float>(imgHeight) / rows;

	std::vector<TileTypes> levelData(rows * cols);
	Tiles tiles;
	for (int y = 0; y < rows; ++y) 
	{
		for (int x = 0; x < cols; ++x)
		{
			int idx = y * cols + x;
			float xData = x * tileWidth + tileHeight / 2.0f;
			float yData = y * tileHeight + tileHeight / 2.0f;
			const unsigned char* color = img.data(static_cast<int>(xData), static_cast<int>(yData));

			//we just use Red value to compare 
			const int r = static_cast<int>(color[0]);


			if (r == tiles.ground)
			{
				levelData[idx] = TileTypes::Ground;
			}
			else if (r == tiles.brickWall)
			{
				levelData[idx] = TileTypes::BrickWall;
			}
			else if (r == tiles.solidWall)
			{
				levelData[idx] = TileTypes::SolidWall;
			}
			else if (r == tiles.water)
			{
				levelData[idx] = TileTypes::Water;
			}
			else if (r == tiles.grass)
			{
				levelData[idx] = TileTypes::Grass;
			}
		}
	}

	return levelData;
}

#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	TODO_W7(L"Implement SpriteFontLoader >> Parse .fnt file")
		//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	byte b = pReader->Read<byte>();
	byte m = pReader->Read<byte>();
	byte f = pReader->Read<byte>();

	//If Identification bytes doesn't match B|M|F,
	if (b != 66 || m != 77 || f !=70)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font");
		return nullptr;
	}

	//Parse the version (version 3 required)
	byte version = pReader->Read<byte>();

	//If version is < 3,
	if (version < 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}


	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	
	//Retrieve the blockId and blockSize
	byte blockId = pReader->Read<byte>();
	uint32_t blockSize = pReader->Read<uint32_t>();

	//Retrieve the FontSize [fontDesc.fontSize]
	//Short is 2 bytes
	fontDesc.fontSize = pReader->Read<USHORT>();

	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pReader->MoveBufferPosition(12);

	//Retrieve the FontName [fontDesc.fontName]
	fontDesc.fontName = pReader->ReadNullString();

	//**********
	// BLOCK 1 *
	//**********
	

	//Retrieve the blockId and blockSize
	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<uint32_t>();

	uint32_t blockStartPos = static_cast<uint32_t>(pReader->GetBufferPosition());

	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	pReader->MoveBufferPosition(4);
	fontDesc.textureWidth = pReader->Read<USHORT>();
	fontDesc.textureHeight = pReader->Read<USHORT>();

	//Retrieve PageCount
	USHORT pageCount = pReader->Read<USHORT>();

	if (pageCount > 1)
	{
		Logger::LogError(L"Only one texture per font is allowed!");
	}

	//Advance to Block2 (Move Reader)
	pReader->SetBufferPosition(blockStartPos + blockSize);

	//**********
	// BLOCK 2 *
	//**********
	// 
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<uint32_t>();
	blockStartPos = static_cast<uint32_t>(pReader->GetBufferPosition());

	//Retrieve the PageName (BinaryReader::ReadNullString)
	std::wstring pageName = pReader->ReadNullString();
	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	std::wstring fullTexturePath = loadInfo.assetFullPath.parent_path().append(pageName).wstring();
	
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]
	fontDesc.pTexture = ContentManager::Load<TextureData>(fullTexturePath);

	//**********
	// BLOCK 3 *
	//**********
	pReader->SetBufferPosition(blockStartPos + blockSize);

	//Retrieve the blockId and blockSize
	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<uint32_t>();

	//Retrieve Character Count (see documentation)
	uint32_t characterCount = blockSize / 20;

	//Create loop for Character Count, and:
	for (uint32_t i = 0; i < characterCount; ++i)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		wchar_t characterId =static_cast<wchar_t>( pReader->Read<uint32_t>());

		//> Create instance of FontMetric (struct)
		FontMetric fontMetric;
		
		//	> Set Character (CharacterId) [FontMetric::character]
		fontMetric.character = characterId;

		//	> Retrieve Xposition (store Local)
		USHORT xPos = pReader->Read<USHORT>();

		//	> Retrieve Yposition (store Local)
		USHORT yPos = pReader->Read<USHORT>();

		//	> Retrieve & Set Width [FontMetric::width]
		fontMetric.width = pReader->Read<USHORT>();

		//	> Retrieve & Set Height [FontMetric::height]
		fontMetric.height = pReader->Read<USHORT>();

		//	> Retrieve & Set OffsetX [FontMetric::offsetX]
		fontMetric.offsetX = pReader->Read<short>();

		//	> Retrieve & Set OffsetY [FontMetric::offsetY]
		fontMetric.offsetY = pReader->Read<short>();

		//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
		fontMetric.advanceX = pReader->Read<short>();

		//	> Retrieve & Set Page [FontMetric::page]
		fontMetric.page = pReader->Read<unsigned char>();

		//	> Retrieve Channel (BITFIELD!!!) 
		//		> See documentation for BitField meaning [FontMetrix::channel]
		unsigned char channel = pReader->Read<unsigned char>();
		switch (channel)
		{
			//Blue
		case 1:
			channel = 2;
			break;
			//Green
		case 2:
			channel = 1;
			break;
			//Red
		case 4:
			channel = 0;
			break;
			//Alpha
		case 8:
			channel = 3;
			break;
		default:
			channel = 0;
			break;
		}

		fontMetric.channel = channel;

		//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
		fontMetric.texCoord.x = xPos / static_cast<float>(fontDesc.textureWidth);
		fontMetric.texCoord.y = yPos / static_cast<float>(fontDesc.textureHeight);

		//> Insert new FontMetric to the metrics [font.metrics] map
		//	> key = (wchar_t) charId
		//	> value = new FontMetric
		fontDesc.metrics.insert(std::pair{ characterId,fontMetric });

		//(loop restarts till all metrics are parsed)
	}

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}

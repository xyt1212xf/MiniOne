#include "pch.h"
#include "Font.h"
#include "ResourceManager.h"
namespace mini
{
	CFont::CFont()
	{
		mResourceType = Font;
	}


	CFont::~CFont()
	{
		SafeRelease(mpTexture);
	}

	const mini::CTexture* const CFont::getFontTexture() const
	{
		return mpTexture;
	}

	bool CFont::initialise()
	{
		loadFontData(mResourceName + ".txt");
		loadTexture(mResourceName + ".dds");
		return true;
	}

	bool CFont::loadFontData(const std::string& fileName)
	{
		std::ifstream fin;

		// Read in the font size and spacing between chars.
		fin.open(fileName);
		if (fin.fail())
		{
			return false;
		}
		int cellCount = 0;
		fin >> cellCount;
		mFontCells.resize(cellCount);
		
		char temp = 0;
		// Read in the 95 used ascii characters for text.
		for (auto& item : mFontCells)
		{
			fin.get(temp);
			while (temp != ' ')
			{
				fin.get(temp);
			}
			fin.get(temp);
			while (temp != ' ')
			{
				fin.get(temp);
			}

			fin >> item.left;
			fin >> item.right;
			fin >> item.size;
		}

		// Close the file.
		fin.close();
		return true;
	}

	bool CFont::loadTexture(const std::string& textureFileName)
	{
		CResourceBase* pBase = CResourceManager::getSinglePtr()->createResource(textureFileName, Texture);
		mpTexture = dynamic_cast<CTexture*>(pBase);
		if (nullptr != mpTexture)
		{
			mpTexture->AddRef();
		}
		return true;
	}
}

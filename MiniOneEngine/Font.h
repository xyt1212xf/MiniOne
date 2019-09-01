#pragma once
#include "EngineHead.h"
#include "ResourceBase.h"
#include "GPUBuffer.h"
#include "FoundationStruct.h"
#include "Texture.h"

namespace mini
{
	enum ShoweMode
	{
		Left = 1 << 1,
		Right = 1 << 2,
		Top = 1 << 3,
		Bottom = 1 << 4,
		H_Center = 1 << 5,
		U_Center = 1 << 6,
		Center = 1 << 7
	};

	class CFont : public CResourceBase
	{
		friend class CFontUnit;
		friend class CResourceManager;
		struct FontCell
		{
			float left, right;
			int size;
		};

	public:
		CFont();
		virtual ~CFont();
		const CTexture* const getFontTexture() const;
	private:
		virtual bool initialise() override;
		bool loadFontData(const std::string& fileName);
		bool loadTexture(const std::string& textureFileName);

	private:
		std::vector<FontCell>	mFontCells;
		CTexture*	mpTexture = nullptr;
	};
}
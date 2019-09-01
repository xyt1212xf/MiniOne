#pragma once
#include "EngineHead.h"
#include "Shader.h"
#include "ResourceBase.h"
#include "Texture.h"
#include "ScriptPlugin.h"
namespace mini
{
	class CMaterial : public CResourceBase
	{
		friend class CMaterialUnit;
		friend class CResourceManager;
	public:
		CMaterial();
		virtual ~CMaterial();
		void	render(CDevice* pDevice);

	private:
		CShader*					mpShader;
		std::list<CTexture*>		mTextures;
		union _constBuffer
		{
			struct
			{
				float mDiffuseColor[4];
				float mAmbientColor[4];
				float mSpecularColor[3];
				float mSpecularPower;
			};
			float pConstBuffer;
		};
		_constBuffer mConstBuffer;
	};
}
//template<typename T>
//T CMaterial::getTableItem(lua_State* pLua, const std::string& tableName, const std::string& itemName);

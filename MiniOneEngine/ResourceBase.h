#pragma once
#include "EngineHead.h"

namespace mini
{
	enum ResType
	{
		ResourceBase,
		Texture,
		Shader,
		Mesh,
		Material,
		Wnd,
		Font,
	};


	class CResourceBase : public CRefcount
	{
	public:
		CResourceBase();
		virtual ~CResourceBase();
		UINT32	getResourceID()const;
		std::string& getResourceName();
		void setResourceName(std::string strName);
		virtual bool initialise();
		ResType	getResourceType();

	private:
		UINT32		mResourceUUID;

	protected:
		std::string	mResourceName;
		ResType		mResourceType;
	};
}

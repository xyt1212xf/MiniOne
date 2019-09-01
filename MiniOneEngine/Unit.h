#pragma once
#include "EngineHead.h"
#include <functional>

namespace mini
{
	enum PropertyFlag
	{
		Editor = 1 << 0,
		Logic = 1 << 1,
		GroundName = 1 << 2,
	};

	enum PropertyDataType
	{
		_ADDRESS = 1 << 0,
		_FLOAT = 1 << 1,
		_FLOAT2 = 1 << 2,
		_FLOAT3 = 1 << 3,
		_FLOAT4 = 1 << 4,
		_COLOR32 = 1 << 5,
		_COLOR24 = 1 << 6,
		_TEXTURE = 1 << 7,
	};

	struct Property
	{
		UINT32										nIndex;
		UINT32										nFlag;
		std::string									strName;
		void*										pMemory;
		PropertyDataType							dataType;
		std::function<void()>						callback;
		Property()
		{
		}
		Property(UINT32 index, const std::string& str, void* pData, PropertyDataType datatype, UINT32 flag, std::function<void()> func)
		{
			nIndex = index;
			strName = str;
			pMemory = pData;
			nFlag = flag;
			dataType = datatype;
			callback = func;
		}

	};

	class CUnit : public CRefcount
	{
	public:
		CUnit();
		virtual ~CUnit();
		virtual void onInitialise();
		virtual void update();
		virtual void onUninitialise();
		virtual void setUnitFile(const std::string& strFileName);
		virtual Property& getProperty(UINT32 uIndex);
		virtual Property& getProperty(const std::string& strName);
		virtual void exportPropertyToEditor();
		virtual void updateUnitProperty(Property* pProperty);

		Property* getPropertyFirst();
		Property* getPropertyNext();

	protected:
		Property* RegisterProperty(std::string strName, void* pData, PropertyDataType datatype, UINT32 flag, std::function<void()> func);
	
	protected:
		bool	mbDirty;
		std::string mstrFileName;
		std::vector<Property*>	mProperty;
		std::map<std::string, Property>	mstrProperty;
		std::vector<Property*>::iterator mstartItem;
	};
}

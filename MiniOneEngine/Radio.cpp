#include "pch.h"
#include "Radio.h"

namespace mini
{
	namespace ui
	{
		static std::map<UINT32, std::list<CRadio*> > GroupMap;
		CRadio::CRadio()
		{
			mWndType = ui_radio;
		}

		CRadio::~CRadio()
		{
			removeGroup(mGroupId);
		}

		void CRadio::setGroupId(UINT32 groupId)
		{
			removeGroup(mGroupId);
			GroupMap[groupId].push_back(this);
			mGroupId = groupId;
		}

		UINT32 CRadio::getGroupId()const
		{
			return mGroupId;
		}

		void CRadio::removeGroup(UINT32 groupId)
		{
			auto item = GroupMap.find(groupId);
			if (item != GroupMap.end())
			{
				auto radio = std::find((item->second).begin(), (item->second).end(), this);
				if (radio != (item->second).end())
				{
					(item->second).erase(radio);
				}
			}
		}

		void CRadio::select()
		{
			for (auto& item : GroupMap[mGroupId])
			{
				item->setState(UI_Normal);
			}
			setState(UI_Push);
		}
	}

}
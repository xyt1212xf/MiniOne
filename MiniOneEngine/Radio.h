#pragma once
#include "Wnd.h"
namespace mini
{
	namespace ui
	{
		class CRadio : public CWnd
		{
			friend class CTableWnd;
		public:
			CRadio();
			virtual ~CRadio();
			void setGroupId(UINT32 groupId);
			UINT32 getGroupId() const;

		protected:
			void select();

		private:
			void removeGroup(UINT32 groupId);

		private:
			UINT32 mGroupId = 0;
		};
	}
}
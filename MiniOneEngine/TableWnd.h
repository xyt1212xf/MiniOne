#pragma once
#include "Wnd.h"
#include "Radio.h"
namespace mini
{
	namespace ui
	{
		class CTableWnd : public CWnd
		{
		public:
			CTableWnd();
			virtual ~CTableWnd();
			void addToTable(CWnd* pWnd, const std::string& itemName);
			void removeFromTable(INT32 index);
			void select(INT32 index );

		private:
			virtual bool insideMsgProcess(event& e) override;

		private:
			struct subWnd 
			{
				CRadio* pItemWnd = nullptr;
				CWnd* pWnd = nullptr;
			};
			std::vector<subWnd>		mSubItemWnds;
			INT32					mCurrentIndex = -1;
		};
	}
}
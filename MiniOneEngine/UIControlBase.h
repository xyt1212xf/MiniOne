#pragma once
#include "FoundationStruct.h"
namespace mini
{
#define	WHITE		0xffffffff					//白色
#define BLACK		0xff000000					//黑色
#define RED			0xffff0000					//红色
#define GREEN		0xff00ff00					//绿色
#define BLUE		0xff0000ff					//蓝色
#define YELLOW		0xffffff00					//黄色
#define CYAN		0xff00ffff					//蓝绿色
#define MAGENTA		0xffff00ff					//品红
	namespace ui
	{
		class UIControlBase //: public ObjectBase
		{
		public:
			friend class UIMain;

			UIControlBase();
			virtual ~UIControlBase(void);

		protected:
			//控件的位置             
			CPoint2F				mPoint;

			//在D3D中的位置
			CPoint2F				mD3DPoint;

			//控件的大小
			CPoint2F				mSize;

			CPoint2F				mScale;

			CRectF					mAnswerArea;

			CRectF					mOldAnswerArea;

			//是否可以使用
			bool                    mActive;

			//是否有焦点
			bool					mfocus;

			//是否显示
			bool					mbVisible;
		};
		typedef UIControlBase* LCPUIControlBase;
	}
}

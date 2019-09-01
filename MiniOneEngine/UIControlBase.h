#pragma once
#include "FoundationStruct.h"
namespace mini
{
#define	WHITE		0xffffffff					//��ɫ
#define BLACK		0xff000000					//��ɫ
#define RED			0xffff0000					//��ɫ
#define GREEN		0xff00ff00					//��ɫ
#define BLUE		0xff0000ff					//��ɫ
#define YELLOW		0xffffff00					//��ɫ
#define CYAN		0xff00ffff					//����ɫ
#define MAGENTA		0xffff00ff					//Ʒ��
	namespace ui
	{
		class UIControlBase //: public ObjectBase
		{
		public:
			friend class UIMain;

			UIControlBase();
			virtual ~UIControlBase(void);

		protected:
			//�ؼ���λ��             
			CPoint2F				mPoint;

			//��D3D�е�λ��
			CPoint2F				mD3DPoint;

			//�ؼ��Ĵ�С
			CPoint2F				mSize;

			CPoint2F				mScale;

			CRectF					mAnswerArea;

			CRectF					mOldAnswerArea;

			//�Ƿ����ʹ��
			bool                    mActive;

			//�Ƿ��н���
			bool					mfocus;

			//�Ƿ���ʾ
			bool					mbVisible;
		};
		typedef UIControlBase* LCPUIControlBase;
	}
}

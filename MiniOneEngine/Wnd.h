#pragma once
#include "ResourceBase.h"
#include "ScriptPlugin.h"
#include "FoundationStruct.h"
#include "GUIStyle.h"
#include "Font.h"
namespace mini
{
	class CTexture;

	namespace ui
	{
		enum
		{
			UI_Normal = 0,
			UI_Push,
			UI_Hover,
			UI_Disable,
			UI_Leave,
			UI_MaxState,
		};

		enum
		{
			UI_Move = 0x00000001,			//�����ƶ�
			UI_Bind = 0x00000010,			//���԰����
			UI_ProcessMsg = 0x00000100,			//��Ϣ��Ӧ
			UI_Mode = 0x00001000,			//ģ̬
			UI_UVLR = 0x00010000,			//�������Ҿ���
			UI_UVTB = 0x00100000,			//�������¾���
			UI_ISTOP = 0x01000000,			//�������Ƿ񸡶������ϲ�
			UI_SCISSOR = 0x10000000,			//�����Ƿ��вü�����
			UI_Edit = 0x00000000,			//�༭״̬	 
		};

		class CWnd;
		typedef std::list< CWnd* > Childlist;
		typedef std::function<bool()> ItemFunction;
		typedef bool(CWnd::*Function)();

		class CWnd : public CToScriptClass, public CResourceBase
		{
			typedef std::tuple<MSG_TYPE, std::function<bool()>> MsgWndCallback;
			friend class CGuiPlugin;
			friend class CGuiRenderer;
			friend class CUIMain;

		public:
			CWnd();
			virtual ~CWnd();
			virtual void bindItemFunction(MSG_TYPE msgType, const std::string& name, Function function);
			virtual void bindItemFunction(MSG_TYPE msgType, const std::string& name, const std::string& childName, Function function);

			void setPosition(float x, float y);
			void positionOffset(float x, float y);
			void positionOffset(const CPoint2F& pt);

			CPoint2F getPosition();
			void setSize(float w, float h);
			CPoint2F getSize();

			void setWidth(float width);
			void setHeight(float height);

			void setScale(float scale);
			CPoint2F getScale();
			void setScaleX(float scaleX);
			void setScaleY(float scaleY);
			void enable();
			void disable();
			void setVisual(bool bVisible);
			float getWidth();
			float getHeight();
			bool isVisible();
			bool isActive();
			void setWndName(const std::string& strName);
			const std::string& getWndName();
			CWnd* findChildWnd(CWnd* pWnd);
			CWnd* findChildWnd(const std::string& name);
			CWnd* findChildWnd(INT32 id);

			CWnd* getParent();
			void setParentWnd(CWnd* pWnd);
			void addChildWnd(CWnd* pWnd);
			void removeChildWnd(CWnd* pWnd);
			void removeChildWnd(UINT32 id);
			const std::string& getWindowText();
			void setWindowText(const std::string& text);
			float getX();
			float getY();
			CPoint2F getLocalPosition();
			void setX(float x);
			void setY(float y);
			WndType getWndType();

			void setGuiStyle(GuiStyle* pGuiStyle);
			void setGuiStyle(const std::string& styleName);

			GuiStyle* getGuiStyle();

			// �õ���ת
			float					getRotation();

			// ������ת
			void					setRotation(float angle);

			// �õ�������
			CWnd*					getParentUIWnd();
			// ���ô����ı�
			virtual void			SetTextContent(const std::string& text);

			bool	isFocus();

			void	setFocus(bool bFocus);

			UINT32 getId();

			void setFont(CFont* pFont, CPoint4F color, CPoint2F size, ShoweMode mode);
			void setFont(CFontUnit* pFontUnit);
			void setFontShowMode(ShoweMode mode);
			void setFontSize(CPoint2F size);
			void setFontColor(CPoint4F color);
			//void setDrawText(const std::string& text, float x, float y, float w, float h);

			CFont*		getFont();
			ShoweMode	getFontShowMode();
			CPoint2F	getFontSize();
			CPoint4F	getFontColor();

		private:
			UINT8  getTextureAlpha(const POINT &mMouse);

		protected:
			virtual void setState(INT32 state);
			virtual bool insideMsgProcess(event& e);
			virtual void bindMsg();

		protected:
			WndType			mWndType;
			std::list<CWnd*> mpChildren;
			INT32			mState;						//��ǰ״̬
			CPoint2F		mPosition;
			CPoint2F		mSize;
			CPoint2F		mScale;
			GuiStyle*		mpGuiStyle = nullptr;
			CTexture*		mpUserTexture = nullptr;
			CFontUnit*		mpFontUnit = nullptr;
			bool			mScissor = false;

		private:
			std::map<CWnd*, std::list<MsgWndCallback> >	mItemCallback;
			UINT32			mId;
			UINT32			mUnProcessMsg;				//���ᴦ�����Ϣ
			CWnd*			mpParentWnd = nullptr;

			std::string mWndName;
			std::string mWindowText;
			std::string	mContent;						//���������

			DWORD			mExterFlag = 0;
			UINT32			mTransparencyMsg = 0;
			DWORD			mflag;						//���ڱ��
			float			mRotation;					//��ת
			DWORD			mUIColor;					//������ɫ

			bool mbVisible = true;
			bool mActive = true;
			bool mfocus = false;
			bool mbDrawSelf = true;

			DeclareScriptClass(CWnd);
		};
		extern 	bool IsSpace(const POINT &mMouse, CWnd* pWnd);
	}
}

#define BindMessage(msg, name, function) \
	bindItemFunction(msg, name, (mini::ui::Function)function)
#define BindChildMessage(msg, name, childname, function) \
	bindItemFunction(msg, name, childname, (mini::ui::Function)function)

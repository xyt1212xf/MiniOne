#include "pch.h"
#include "Wnd.h"
#include "GuiPlugin.h"
#include "Texture.h"
#include "GUIStyle.h"

namespace mini
{
	namespace ui
	{
		bool IsSpace(const POINT &mMouse, CWnd* pWnd)
		{
			bool bResult = false;
			if (nullptr != pWnd)
			{
				CPoint2F& pt = pWnd->getPosition();
				CPoint2F& size = pWnd->getSize();
				if ((mMouse.x >= pt.x && mMouse.x <= pt.x + size.x) &&
					(mMouse.y >= pt.y && mMouse.y <= pt.y + size.y))
				{
					bResult = true;
				}
			}
			return bResult;
		}

#define MYSELFID 0
		DefineScriptClass(CWnd)
			BeginRegisterMember(CWnd)
			REGISTER_CLASS_MEMFUN2(CWnd, void, float, float, "setPosition", setPosition);
		REGISTER_CLASS_MEMFUN2(CWnd, void, float, float, "setSize", setSize);
		REGISTER_CLASS_MEMFUN0(CWnd, UINT32, "getId", getId);
		EndRegisterMember()


			CWnd::CWnd()
			: mpParentWnd(nullptr)
		{
			mWindowText = "";
			mResourceType = Wnd;
			mWndType = ui_wnd;
			mWndName = "";
			mfocus = false;
			mflag = UI_ProcessMsg | UI_Move | UI_ISTOP;
			mUnProcessMsg = 0;
			mUIColor = 0xffffffff;

			static UINT32 ID = 0;
			mId = ++ID;
		}

		CWnd::~CWnd()
		{
			if (mpParentWnd != nullptr)
			{
				SafeRelease(mpParentWnd);
			}
			SafeRelease(mpFontUnit);
		}

		void CWnd::bindItemFunction(MSG_TYPE msgType, const std::string& name, Function function)
		{
			CWnd* pWnd = findChildWnd(name);
			if (nullptr != pWnd)
			{
				mItemCallback[pWnd].push_back(MsgWndCallback(msgType, std::bind(function, this)));
			}
		}

		void CWnd::bindItemFunction(MSG_TYPE msgType, const std::string& name, const std::string& childName, Function function)
		{
			CWnd* pWnd = findChildWnd(name);
			if (nullptr != pWnd)
			{
				pWnd->bindItemFunction(msgType, childName, function);
			}
		}

		void CWnd::setPosition(float x, float y)
		{
			float offsetX = mPosition.x;
			float offsetY = mPosition.y;
			if (nullptr != mpParentWnd)
			{
				mPosition.x += mpParentWnd->mPosition.x + x;
				mPosition.y += mpParentWnd->mPosition.y + y;
			}
			else
			{
				mPosition.x = x;
				mPosition.y = y;
			}
			if (mpChildren.size() > 0)
			{
				for (auto& item : mpChildren)
				{
					item->positionOffset(mPosition.x + offsetX, mPosition.y + offsetY);
				}
			}
			setWindowText(mWindowText);
		}

		void CWnd::positionOffset(float x, float y)
		{
			mPosition.x += x;
			mPosition.y += y;
			setWindowText(mWindowText);
		}

		void CWnd::positionOffset(const CPoint2F& pt)
		{
			mPosition.x += pt.x;
			mPosition.y += pt.y;
			setWindowText(mWindowText);
		}

		void CWnd::setFont(CFont* pFont, CPoint4F color, CPoint2F size, ShoweMode mode)
		{
			if (nullptr != pFont)
			{
				SafeDelete(mpFontUnit);
			}
			mpFontUnit = new CFontUnit(pFont);
			mpFontUnit->setFontColor(color);
			mpFontUnit->setFontSize(size);
			mpFontUnit->setFontShowMode(mode);
			setWindowText(mWindowText);
		}

		void CWnd::setFont(CFontUnit* pFontUnit)
		{
			if (mpFontUnit != pFontUnit && nullptr != pFontUnit)
			{
				setFont(pFontUnit->getFont(), pFontUnit->getFontColor(), pFontUnit->getFontSize(), pFontUnit->getFontShowMode());
			}
		}

		void CWnd::setFontShowMode(ShoweMode mode)
		{
			if (nullptr != mpFontUnit)
			{
				mpFontUnit->setFontShowMode(mode);
			}
		}

		void CWnd::setFontSize(CPoint2F size)
		{
			if (nullptr != mpFontUnit)
			{
				mpFontUnit->setFontSize(size);
			}
		}

		void CWnd::setFontColor(CPoint4F color)
		{
			if (nullptr != mpFontUnit)
			{
				mpFontUnit->setFontColor(color);
			}
		}

		/*
		void CWnd::setDrawText(const std::string& text, float x, float y, float w, float h)
		{
			if (nullptr != mpFontUnit)
			{
				mpFontUnit->setDrawText(text, x, y );
			}
		}
		*/

		CFont* CWnd::getFont()
		{
			return  nullptr != mpFontUnit ? mpFontUnit->getFont() : nullptr;
		}

		ShoweMode CWnd::getFontShowMode()
		{
			return nullptr != mpFontUnit ? mpFontUnit->getFontShowMode() : ShoweMode::Center;
		}

		CPoint2F CWnd::getFontSize()
		{
			return nullptr != mpFontUnit ?  mpFontUnit->getFontSize() : CPoint2F(0, 0);
		}

		CPoint4F CWnd::getFontColor()
		{
			return nullptr != mpFontUnit ?  mpFontUnit->getFontColor() : CPoint4F(1,1,1,1);
		}

		CPoint2F CWnd::getPosition()
		{
			return mPosition;
		}

		void CWnd::setSize(float w, float h)
		{
			mSize.x = w;
			mSize.y = h;
			setWindowText(mWindowText);
		}

		CPoint2F CWnd::getSize()
		{
			return mSize;
		}

		void CWnd::setWidth(float width)
		{
			mSize.x = width;
		}

		void CWnd::setHeight(float height)
		{
			mSize.y = height;
		}

		void CWnd::setScale(float scale)
		{
			mScale.x = scale;
			mScale.y = scale;
		}

		CPoint2F CWnd::getScale()
		{
			return mScale;
		}

		void CWnd::setScaleX(float scaleX)
		{
			mScale.x = scaleX;
		}

		void CWnd::setScaleY(float scaleY)
		{
			mScale.y = scaleY;
		}

		void CWnd::enable()
		{
			mActive = true;
		}

		void CWnd::disable()
		{
			mActive = false;
		}

		void CWnd::setVisual(bool bVisible)
		{
			mbVisible = bVisible;
		}

		float CWnd::getWidth()
		{
			return mSize.x;
		}

		float CWnd::getHeight()
		{
			return mSize.y;
		}

		bool CWnd::isVisible()
		{
			return mbVisible;
		}

		bool CWnd::isActive()
		{
			return mActive;
		}

		void CWnd::setWndName(const std::string& strName)
		{
			mWndName = strName;
		}

		const std::string& CWnd::getWndName()
		{
			return mWndName;
		}

		CWnd* CWnd::findChildWnd(CWnd* pWnd)
		{
			CWnd* pFindWnd = nullptr;
			for (auto item : mpChildren)
			{
				if (item == pWnd)
				{
					pFindWnd = item;
				}
			}
			return pFindWnd;
		}

		CWnd* CWnd::findChildWnd(const std::string& name)
		{
			CWnd* pFindWnd = nullptr;
			for (auto item : mpChildren)
			{
				if (item->getWndName() == name)
				{
					pFindWnd = item;
				}
			}
			return pFindWnd;
		}

		CWnd* CWnd::findChildWnd(INT32 id)
		{
			CWnd* pFindWnd = nullptr;
			for (auto item : mpChildren)
			{
				if (item->getId() == id)
				{
					pFindWnd = item;
				}
			}
			return pFindWnd;
		}

		CWnd* CWnd::getParent()
		{
			return mpParentWnd;
		}

		void CWnd::setParentWnd(CWnd* pWnd)
		{
			if (mpParentWnd != pWnd)
			{
				SafeRelease(mpParentWnd);
			}
			mpParentWnd = pWnd;
			mpParentWnd->AddRef();
		}

		bool CWnd::insideMsgProcess(event &e)
		{
			return false;
		}

		void CWnd::bindMsg()
		{

		}

		void CWnd::addChildWnd(CWnd* pWnd)
		{
			auto item = std::find(mpChildren.begin(), mpChildren.end(), pWnd);
			if (item == mpChildren.end())
			{
				mpChildren.push_back(pWnd);
				pWnd->AddRef();
				SafeRelease(pWnd->mpParentWnd);
				if (ui_wnd == pWnd->mWndType)
				{
					CGuiPlugin* pGuiPlugin = GETPLUGIN(CGuiPlugin, PluginType::GUI);
					pGuiPlugin->removeWnd(pWnd);
				}
				pWnd->mpParentWnd = this;
				pWnd->mpParentWnd->AddRef();
			}
		}

		void CWnd::removeChildWnd(CWnd* pWnd)
		{
			auto item = std::find(mpChildren.begin(), mpChildren.end(), pWnd);
			if (item != mpChildren.end())
			{
				SafeRelease(pWnd->mpParentWnd);
				SafeRelease((*item));
				mpChildren.erase(item);
			}
		}

		void CWnd::removeChildWnd(UINT32 id)
		{
			auto item = mpChildren.begin();
			auto end = mpChildren.end();
			while (item != end)
			{
				if ((*item)->getId() == id)
				{
					mpChildren.erase(item);
					break;
				}
			}
		}

		const std::string& CWnd::getWindowText()
		{
			return mWindowText;
		}

		void CWnd::setWindowText(const std::string& text)
		{
			mWindowText = text;
			if (nullptr != mpFontUnit)
			{
				// Calculate the screen coordinates of the bottom of the bitmap.
				mpFontUnit->setDrawText(mWindowText, mPosition.x, mPosition.y, mSize.x, mSize.y);
			}
		}

		float CWnd::getX()
		{
			return mPosition.x;
		}


		float CWnd::getY()
		{
			return mPosition.y;
		}


		mini::CPoint2F CWnd::getLocalPosition()
		{
			if (nullptr != mpParentWnd)
			{
				return mPosition - mpParentWnd->mPosition;
			}
			else
			{
				return mPosition;
			}
		}

		void CWnd::setX(float x)
		{
			mPosition.x = x;
		}

		void CWnd::setY(float y)
		{
			mPosition.y = y;
		}

		ui::WndType CWnd::getWndType()
		{
			return mWndType;
		}

		void CWnd::setGuiStyle(GuiStyle* pGuiStyle)
		{
			if (mpGuiStyle != pGuiStyle)
			{
				SafeRelease(mpGuiStyle);
				mpGuiStyle = pGuiStyle;
				if (mpGuiStyle != nullptr)
				{
					mpGuiStyle->AddRef();
					if (mpGuiStyle->mTextures.size() > 0)
					{
						mpUserTexture = mpGuiStyle->mTextures[0];
					}
				}
			}
		}


		void CWnd::setGuiStyle(const std::string& styleName)
		{
			if (nullptr != gGuiStyleManager)
			{
				setGuiStyle(gGuiStyleManager->loadGuiStyle(styleName, mWndType));
			}
		}

		ui::GuiStyle* CWnd::getGuiStyle()
		{
			return mpGuiStyle;
		}

		float CWnd::getRotation()
		{
			return mRotation;
		}

		void CWnd::setRotation(float angle)
		{
			mRotation = angle;
		}

		ui::CWnd* CWnd::getParentUIWnd()
		{
			return mpParentWnd;
		}

		void CWnd::SetTextContent(const std::string& text)
		{
			mContent = text;
		}

		bool CWnd::isFocus()
		{
			return mfocus;
		}

		void CWnd::setFocus(bool bFocus)
		{
			mfocus = bFocus;
		}

		UINT32 CWnd::getId()
		{
			return mId;
		}

		UINT8 CWnd::getTextureAlpha(const POINT &mMouse)
		{
			UINT8 Alpha = 255;
			if (nullptr != mpGuiStyle)
			{
				auto size = (INT32)mpGuiStyle->mTextures.size();
				if (size > 0 && mState < size)
				{
					float x = (float)(mMouse.x - mPosition.x) / mSize.x;
					float y = (float)(mMouse.y - mPosition.y) / mSize.y;
					Alpha = GetTextureAlpha(mpGuiStyle->mTexturesAlpha[mState], x, y, mpGuiStyle->mTextures[mState]->getImageWidth());
				}
			}
			return Alpha;
		}

		void CWnd::setState(INT32 state)
		{
			if (nullptr != mpGuiStyle && state < (INT32)mpGuiStyle->mTextures.size())
			{
				mState = state;
				mpUserTexture = mpGuiStyle->mTextures[mState];
			}
		}
	}
}

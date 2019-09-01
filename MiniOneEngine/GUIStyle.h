#pragma once
#include "resourcebase.h"
#include "Texture.h"
#include <functional>
namespace mini
{
	namespace ui
	{
		enum WndType
		{
			ui_wnd = 0,				//窗口
			ui_button = 1,			//按钮
			ui_check = 2,			//单选
			ui_progress = 3,		//进度条
			ui_list = 4,			//列表
			ui_staticimage = 5,		//图片
			ui_dynamicimage = 6,	//动态图片
			ui_edit = 7,			//文本
			ui_staticedit = 8,		//静态文本
			ui_minwnd = 9,			//小窗口
			ui_mouse = 10,			//鼠标
			ui_radio = 11,			//单选
			ui_checkbox = 12,		//复选
			ui_slide = 13,			//滑动条
			ui_actionimage = 14,    //动画控件
			ui_verslide = 15,		//竖滑条
			ui_statictext = 16,		//静态文本控件
			ui_skill = 17,          //技能CD控件
			ui_progressBar = 18,    //进度条控件
			ui_dropList = 19,		//下拉菜单
			ui_horSlide = 20,		//水平滚动条
			ui_comboBox = 21,		//下拉菜单
			ui_menu = 22,			//菜单
			ui_font = 23,
			ui_tableWnd = 24,
			ui_all,
			selfwnd = 0x0FFFFFFE,
		};

		struct GuiStyle : public CResourceBase
		{
			bool	initialise = false;
			std::vector<CRectF>					mUVlist;
			std::vector<CTexture*>				mTextures;
			std::vector<std::vector<UINT8>>		mTexturesAlpha;
			std::vector<std::string> mTexturesName;
			std::string				mName;
			WndType					mWndType = ui_wnd;
			POINT					mPt;
			POINT					mSize;
		};
#define ControlStyle(structName, Type) \
	struct structName : public GuiStyle \
	{\
		structName()\
		{\
			mWndType = Type;\
		}\
	};

		ControlStyle(ButtonStyle, ui_button);
		ControlStyle(MenuStyle, ui_menu);
		ControlStyle(RadioStyle, ui_radio);

		struct TableWndStyle : public GuiStyle
		{
			TableWndStyle()
			{
				mWndType = ui_tableWnd;
				pRadioStyle = nullptr;
			}
			~TableWndStyle()
			{
				SafeRelease(pRadioStyle);
			}
			std::string radioStyleName;
			GuiStyle* pRadioStyle;
		};

		class CGuiStyleManager
		{
		public:
			CGuiStyleManager();
			~CGuiStyleManager();
			bool loadGuiStyleFile(const std::string& strFileName);
			GuiStyle* loadGuiStyle(const std::string& styleName, WndType type);
			bool initGuiStyle(GuiStyle* pGuiStyle);
			GuiStyle* getGuiStyle(const std::string& styleName, WndType type);

		private:
			bool initWndStyle(GuiStyle* pGuiStyle);
			bool initButtonStyle(GuiStyle* pGuiStyle);
			bool initRadioStyle(GuiStyle* pGuiStyle);
			bool initTableWndStyle(GuiStyle* pGuiStyle);
			void initTexture(GuiStyle* pGuiStyle);
			void initUVListAlpha(GuiStyle* pGuiStyle, INT32 width);

			bool initFontStyle(GuiStyle* pGuiStyle);
			bool parseWndStyle(GuiStyle* pGuiStyle, TiXmlElement* pElement);
			bool parseButtonStyle(GuiStyle* pGuiStyle, TiXmlElement* pElement);
			bool parseTableWndStyle(GuiStyle* pGuiStyle, TiXmlElement* pElement);
			std::map<std::string, std::string> parseCommonStyle(GuiStyle* pGuiStyle, TiXmlElement* pElement);

		private:
			std::map<WndType, std::list<GuiStyle*> >	mGuiStyles;
			std::function<bool(GuiStyle*) > minitGuiStyleFunctiona[ui_all];
		};
		extern CGuiStyleManager* gGuiStyleManager;
		extern UINT8 GetTextureAlpha(std::vector<UINT8>& alphaArray, float x, float y, UINT32 imageWidth);
	}
}

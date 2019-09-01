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
			ui_wnd = 0,				//����
			ui_button = 1,			//��ť
			ui_check = 2,			//��ѡ
			ui_progress = 3,		//������
			ui_list = 4,			//�б�
			ui_staticimage = 5,		//ͼƬ
			ui_dynamicimage = 6,	//��̬ͼƬ
			ui_edit = 7,			//�ı�
			ui_staticedit = 8,		//��̬�ı�
			ui_minwnd = 9,			//С����
			ui_mouse = 10,			//���
			ui_radio = 11,			//��ѡ
			ui_checkbox = 12,		//��ѡ
			ui_slide = 13,			//������
			ui_actionimage = 14,    //�����ؼ�
			ui_verslide = 15,		//������
			ui_statictext = 16,		//��̬�ı��ؼ�
			ui_skill = 17,          //����CD�ؼ�
			ui_progressBar = 18,    //�������ؼ�
			ui_dropList = 19,		//�����˵�
			ui_horSlide = 20,		//ˮƽ������
			ui_comboBox = 21,		//�����˵�
			ui_menu = 22,			//�˵�
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

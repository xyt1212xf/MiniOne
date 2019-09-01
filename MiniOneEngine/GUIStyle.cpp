#include "pch.h"
#include "GUIStyle.h"
#include "ResourceManager.h"
#include "RendererPlugin.h"
namespace mini
{
	namespace ui
	{
#pragma warning( push )
#pragma warning( disable:4244 ) 
#pragma warning( disable:4018 ) 
		UINT8 GetTextureAlpha(std::vector<UINT8>& alphaArray, float x, float y, UINT32 imageWidth)
		{
			UINT8 alpha = 255;
			INT32 index = x * imageWidth + y;
			if (index < alphaArray.size())
			{
				alpha = alphaArray[index];
			}
			return alpha;
		}
#pragma warning( pop )
		CGuiStyleManager* gGuiStyleManager = nullptr;
		//////////////////////////////////////////////////////////////////////////
		CGuiStyleManager::CGuiStyleManager()
		{
			gGuiStyleManager = this;
			minitGuiStyleFunctiona[ui_wnd] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_button] = std::bind(&CGuiStyleManager::initButtonStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_check] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_progress] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_staticimage] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_dynamicimage] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_edit] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_staticedit] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_minwnd] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_mouse] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_radio] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_checkbox] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_slide] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_actionimage] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_verslide] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_statictext] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_skill] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_progressBar] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_dropList] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_horSlide] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_comboBox] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_menu] = std::bind(&CGuiStyleManager::initWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_font] = std::bind(&CGuiStyleManager::initFontStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_tableWnd] = std::bind(&CGuiStyleManager::initTableWndStyle, this, std::placeholders::_1);
			minitGuiStyleFunctiona[ui_radio] = std::bind(&CGuiStyleManager::initRadioStyle, this, std::placeholders::_1);
		}

		CGuiStyleManager::~CGuiStyleManager()
		{
			for (auto i : mGuiStyles)
			{
				for (auto item : (i.second))
				{
					SafeRelease(item);
				}
			}
		}

		bool CGuiStyleManager::loadGuiStyleFile(const std::string& strFileName)
		{
#define parse(wndtype, Class, parseStyle ) \
		TiXmlElement* pChildElement = pWndType->FirstChildElement(); \
		while (pChildElement) \
		{\
			GuiStyle* pStyle = new Class;\
			WndType type = wndtype;\
			if (parseStyle(pStyle, pChildElement))\
			{\
				mGuiStyles[type].push_back(pStyle);\
			}\
			else\
			{\
				SafeDelete(pStyle);\
				break;\
			}\
			pChildElement = pChildElement->NextSiblingElement();\
		}

			TiXmlDocument doc;
			if (doc.LoadFile(strFileName))
			{
				const char *errorStr = doc.ErrorDesc();
				TiXmlElement *root = doc.FirstChildElement();           //获取根节点元素
				const char* ElementName = root->Value();                   //获取元素名
				TiXmlElement *pWndType = root->FirstChildElement();        //获取root元素下的第一个子元素
				while (pWndType != nullptr)
				{
					const char* wndtype = pWndType->Value();
					if (strcmp("ui_wnd", wndtype) == 0)
					{
						parse(ui_wnd, GuiStyle, parseWndStyle);
					}
					else if (strcmp("ui_button", wndtype) == 0)
					{
						parse(ui_button, ButtonStyle, parseButtonStyle);
					}
					else if (strcmp("ui_menu", wndtype) == 0)
					{
						parse(ui_menu, MenuStyle, parseButtonStyle);
					}
					else if (strcmp("ui_tableWnd", wndtype) == 0)
					{
						parse(ui_tableWnd, TableWndStyle, parseTableWndStyle);
					}
					else if (strcmp("ui_radio", wndtype) == 0)
					{
						parse(ui_radio, RadioStyle, parseWndStyle);
					}
					pWndType = pWndType->NextSiblingElement();
				}
			}
			return true;
		}

		GuiStyle* CGuiStyleManager::loadGuiStyle(const std::string& styleName, WndType type)
		{
			auto item = mGuiStyles[type];
			for (auto i : item)
			{
				if (styleName == i->mName)
				{
					if (i->initialise)
					{
						return i;
					}
					else
					{
						return initGuiStyle(i) ? i : nullptr;
					}
				}
			}
			return nullptr;
		}

		bool CGuiStyleManager::initGuiStyle(GuiStyle* pGuiStyle)
		{
			CRendererPlugin::getSinglePtr()->stopRender();
#ifdef DEBUG
			if (pGuiStyle != nullptr && pGuiStyle->initialise)
			{
				return true;
			}
#endif // DEBUG	
			minitGuiStyleFunctiona[pGuiStyle->mWndType](pGuiStyle);
			pGuiStyle->initialise = true;
			CRendererPlugin::getSinglePtr()->recoverRender();
			return true;
		}

		ui::GuiStyle* CGuiStyleManager::getGuiStyle(const std::string& styleName, WndType type)
		{
			GuiStyle* pGuiStyle = nullptr;
			auto item = mGuiStyles[type];
			for (auto i : item)
			{
				if (styleName == i->mName)
				{
					pGuiStyle = i;
				}
			}
			return pGuiStyle;
		}

		bool CGuiStyleManager::initWndStyle(GuiStyle* pGuiStyle)
		{
			initTexture(pGuiStyle);
			initUVListAlpha(pGuiStyle, pGuiStyle->mSize.x);
			return true;
		}

		bool CGuiStyleManager::initButtonStyle(GuiStyle* pGuiStyle)
		{
			int size = pGuiStyle->mSize.x;
			if (pGuiStyle->mTexturesName.size() == 1)
			{
				size = pGuiStyle->mSize.x / 4;
			}
			if (pGuiStyle->mTexturesName.size() == 1)
			{
				for (int i = 1; i < 4; ++i)
				{
					pGuiStyle->mTexturesName.push_back(pGuiStyle->mTexturesName[0]);
				}
			}
			initTexture(pGuiStyle);
			initUVListAlpha(pGuiStyle, size);
			return true;
		}

		bool CGuiStyleManager::initRadioStyle(GuiStyle* pGuiStyle)
		{
			int size = pGuiStyle->mSize.x;
			if (pGuiStyle->mTexturesName.size() == 1)
			{
				size = pGuiStyle->mSize.x / 2;
			}
			if (pGuiStyle->mTexturesName.size() == 1)
			{
				for (int i = 1; i < 2; ++i)
				{
					pGuiStyle->mTexturesName.push_back(pGuiStyle->mTexturesName[0]);
				}
			}
			initTexture(pGuiStyle);
			initUVListAlpha(pGuiStyle, size);
			return true;
		}

		bool CGuiStyleManager::initTableWndStyle(GuiStyle* pGuiStyle)
		{
			TableWndStyle* pTableWndStyle = dynamic_cast<TableWndStyle*>(pGuiStyle);
			initTexture(pGuiStyle);
			initUVListAlpha(pGuiStyle, pGuiStyle->mSize.x);
			pTableWndStyle->pRadioStyle = loadGuiStyle(pTableWndStyle->radioStyleName, ui_radio);
			pTableWndStyle->pRadioStyle->AddRef();
			return true;
		}

		void CGuiStyleManager::initTexture(GuiStyle* pGuiStyle)
		{
			for (auto i : pGuiStyle->mTexturesName)
			{
				CResourceBase* pResource = CResourceManager::getSinglePtr()->findResource(i);
				CTexture* pTexture = nullptr;
				if (pResource == nullptr)
				{
					pTexture = dynamic_cast<CTexture*>(CResourceManager::getSinglePtr()->createResource(i, Texture));
				}
				else
				{
					pTexture = dynamic_cast<CTexture*>(pResource);
				}
				if (pTexture != nullptr)
				{
					pGuiStyle->mTextures.push_back(pTexture);
				}
			}
		}

		void CGuiStyleManager::initUVListAlpha(GuiStyle* pGuiStyle, INT32 width)
		{
			for (size_t i = 0; i < pGuiStyle->mTextures.size(); ++i)
			{
				CTexture* pTexture = pGuiStyle->mTextures[i];
				pGuiStyle->mUVlist.push_back(CRectF());
				auto& item = pGuiStyle->mUVlist.back();
				item.left = ((float)pGuiStyle->mPt.x + i * width) / pTexture->getImageWidth();
				item.right = (float)(pGuiStyle->mPt.x + i * width + width) / pTexture->getImageWidth();
				item.top = (float)pGuiStyle->mPt.y / pTexture->getImageHeight();
				item.bottom = (float)(pGuiStyle->mPt.y + pGuiStyle->mSize.y) / pTexture->getImageHeight();
				pGuiStyle->mTexturesAlpha.push_back(pTexture->saveAlpha(pGuiStyle->mPt.x + i * width,
					pGuiStyle->mPt.y,
					pGuiStyle->mPt.x + i * width + width,
					pGuiStyle->mPt.y + pGuiStyle->mSize.y));
			}
		}

		bool CGuiStyleManager::initFontStyle(GuiStyle* pGuiStyle)
		{
			return true;
		}

		bool CGuiStyleManager::parseWndStyle(GuiStyle* pGuiStyle, TiXmlElement* pElement)
		{
			auto parseMap = parseCommonStyle(pGuiStyle, pElement);

			return true;
		}

		bool CGuiStyleManager::parseButtonStyle(GuiStyle* pGuiStyle, TiXmlElement* pElement)
		{
			auto parseMap = parseCommonStyle(pGuiStyle, pElement);
			if (parseMap.find("n") != parseMap.end())
			{
				pGuiStyle->mTexturesName.resize(4);
				pGuiStyle->mTexturesName[0] = parseMap["n"];
				if (parseMap.find("h") != parseMap.end())
				{
					pGuiStyle->mTexturesName[1] = parseMap["h"];
				}
				if (parseMap.find("p") != parseMap.end())
				{
					pGuiStyle->mTexturesName[2] = parseMap["p"];
				}
				if (parseMap.find("d") != parseMap.end())
				{
					pGuiStyle->mTexturesName[3] = parseMap["d"];
				}
			}

			return true;
		}

		bool CGuiStyleManager::parseTableWndStyle(GuiStyle* pGuiStyle, TiXmlElement* pElement)
		{
			auto parseMap = parseCommonStyle(pGuiStyle, pElement);
			TableWndStyle* pTableWndStyle = dynamic_cast<TableWndStyle*>(pGuiStyle);
			pTableWndStyle->radioStyleName = parseMap["item"];
			return true;
		}

		std::map<std::string, std::string> CGuiStyleManager::parseCommonStyle(GuiStyle* pGuiStyle, TiXmlElement* pElement)
		{
			std::map<std::string, std::string> parse;
			TiXmlAttribute *Attribute = pElement->FirstAttribute();  //获取brother元素的第一个属性
			while (Attribute != nullptr)
			{
				const char*  AttributeName = Attribute->Name();              //获取Attribute属性的名字
				const char*  AttributeValue = Attribute->Value();            //获取Attribute属性的值
				parse[AttributeName] = AttributeValue;
				Attribute = Attribute->Next();
			}
			if (parse.find("name") != parse.end())
			{
				pGuiStyle->mName = parse["name"];
			}
			if (parse.find("x") != parse.end())
			{
				pGuiStyle->mPt.x = atoi(parse["x"].c_str());
			}
			if (parse.find("y") != parse.end())
			{
				pGuiStyle->mPt.y = atoi(parse["y"].c_str());
			}
			if (parse.find("width") != parse.end())
			{
				pGuiStyle->mSize.x = atoi(parse["width"].c_str());
			}
			if (parse.find("height") != parse.end())
			{
				pGuiStyle->mSize.y = atoi(parse["height"].c_str());
			}
			if (parse.find("image") != parse.end())
			{
				pGuiStyle->mTexturesName.push_back(parse["image"]);
			}
			return parse;
		}
	}
}

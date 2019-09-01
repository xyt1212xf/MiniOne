#include "pch.h"
#include "GuiPlugin.h"
#include "Camera.h"
#include "RendererPlugin.h"
#include "Font.h"
namespace mini
{
	namespace ui
	{
		CGuiPlugin* CGuiPlugin::gMySelf = nullptr;

		CGuiPlugin::CGuiPlugin(CScriptPlugin* pPlugin)
			: mpPlugin(pPlugin)
			, mpUICamear(nullptr)
		{
			mpUIMain = new CUIMain;
		}

		CGuiPlugin::~CGuiPlugin()
		{
			SafeRelease(mpUICamear);
			SafeDelete(mpUIMain);
		}

		bool CGuiPlugin::onStart()
		{
			gMySelf = this;
			CRenderer* pRenderer = CRendererPlugin::getSinglePtr()->addRenderer(RendererType::GuiRenderer);
			mpGuiRenderer = dynamic_cast<CGuiRenderer*>(pRenderer);
			/*
			CWnd* pWnd = new CWnd();
			addWnd(pWnd);
			SafeRelease(pWnd);
			*/
			//mpUICamear = GEngine->
			/*
			const char* content = "function newindex( t, k, v )\n"
				"	 cc = {}\n"
				"function newindex( t, k, v )\n"
				"end\n"
				""
				"function SetValue( t, k, v )\n"
				"  t[k] = v\n"
				"end\n"
				"cc = {}\n";
			CLuaBind::doString(content);
			*/
			return true;
		}

		void CGuiPlugin::onRun()
		{

		}

		void CGuiPlugin::onShutDown()
		{
			gMySelf = nullptr;
		}

		bool CGuiPlugin::callScript(UINT32 scriptHandle, UINT32 id)
		{
			static lua_State* l = CScriptPlugin::getSinglePtr()->getLuaState();
			INT32 t = lua_gettop(l);
			lua_getglobal(l, "_LUAC");
			lua_pushnumber(l, scriptHandle);
			lua_gettable(l, -2);

			lua_pushstring(l, "WndProcess");
			lua_gettable(l, -2);
			/*
				这里需要特别注意,如果写法是table:function(),
				需要多传递一个参数，就是table自身,而且一定要放在第一个参数上
			*/
			lua_pushvalue(l, -2);
			lua_pcall(l, 1, 0, 0);
			lua_settop(l, t);
			return true;
		}

		bool CGuiPlugin::callCPlusPlus(CWnd* pWnd, CWnd* pchildWnd)
		{
			return true;
		}

		void CGuiPlugin::addWnd(CWnd* pWnd)
		{
			mpUIMain->addWnd(pWnd);
		}

		void CGuiPlugin::removeWnd(CWnd* pWnd)
		{
			mpUIMain->removeWnd(pWnd);
		}

		CWnd* CGuiPlugin::findWnd(const std::string& strName)
		{
			return mpUIMain->findWnd(strName);
		}

		CWnd* CGuiPlugin::findWnd(CWnd* pWnd)
		{
			return mpUIMain->findWnd(pWnd);
		}

		bool CGuiPlugin::initWnd(CWnd* pWnd, std::string guiFile)
		{
			if (pWnd != nullptr)
			{
				TiXmlDocument lconfigXML;
				if (!lconfigXML.LoadFile(guiFile.c_str()))
				{
					return false;
				}
				TiXmlElement *root = lconfigXML.FirstChildElement();           //获取根节点元素
				const char* ElementName = root->Value();                   //获取元素名
				TiXmlElement *pWndType = root->FirstChildElement();        //获取root元素下的第一个子元素
				CFont*	pFont = nullptr;
				CPoint2F size;
				CPoint4F color;
				std::map<UINT32, CFont*> FontMaps;
				while (pWndType != nullptr)
				{
					const char* wndtype = pWndType->Value();
					if (strcmp("Window", wndtype) == 0)
					{
						mGuiParse.parseWnd(pWnd, pWndType);
					}
					else if (strcmp("Font", wndtype) == 0)
					{
						auto& attributeMap = mGuiParse.parseFont(pWnd, pWndType);
						auto parse = [&attributeMap](const std::string& key)->std::vector<float>
						{
							std::vector<float> result;
							auto&& list = foundation::split(attributeMap[key], ",");
							result.reserve(list.size());
							for (auto& item : list)
							{
								result.push_back((float)atof(item.c_str()));
							}
							return result;
						};

						if (attributeMap.find("color") != attributeMap.end())
						{
							std::vector<float>&& c = parse("color");
							color = { c[0], c[1], c[2], c[3] };
						}
						if (attributeMap.find("size") != attributeMap.end())
						{
							std::vector<float>&& s = parse("size");
							size = { s[0], s[1] };
						}

						CResourceBase* pResourceBase = CResourceManager::getSinglePtr()->createResource(attributeMap["name"], Font);
						pFont = dynamic_cast<CFont*>(pResourceBase);
						if (nullptr != pFont)
						{
							pWnd->setFont(pFont, color, size, ShoweMode::Center);
						}
						if (attributeMap.find("id") != attributeMap.end())
						{
							INT32 id = atoi(attributeMap["id"].c_str());
							FontMaps[id] = pFont;
						}
						else
						{
							FontMaps[0] = pFont;
						}
					}
					else if (strcmp("Button", wndtype) == 0)
					{
						CWnd* pChildWnd = new CButton();
						pWnd->addChildWnd(pChildWnd);
						mGuiParse.parseButton(pChildWnd, pWndType);
						auto pt = pWnd->getPosition();
						pChildWnd->positionOffset(pt.x, pt.y);
						pChildWnd->setFont(pFont, color, size, ShoweMode::Center);
						SafeRelease(pChildWnd);
					}
					else if (strcmp("Menu", wndtype) == 0)
					{
						CWnd* pChildWnd = new CMenu();
						pWnd->addChildWnd(pChildWnd);
						mGuiParse.parseMenu(pChildWnd, pWndType);
						SafeRelease(pChildWnd);
					}
					else if (strcmp("Table", wndtype) == 0)
					{
						CWnd* pChildWnd = new CTableWnd();
						pWnd->addChildWnd(pChildWnd);
						mGuiParse.parseTablwWnd(pChildWnd, pWndType);
						auto pt = pWnd->getPosition();
						pChildWnd->positionOffset(pt.x, pt.y);
						pChildWnd->setFont(pFont, color, size, ShoweMode::Center);
						SafeRelease(pChildWnd);
					}
					pWndType = pWndType->NextSiblingElement();
				}
				addWnd(pWnd);
				pWnd->bindMsg();
				return true;
			}
			return false;
		}

		void CGuiPlugin::Process(event& type)
		{
			mpUIMain->processMessage(type);
		}
	}
}
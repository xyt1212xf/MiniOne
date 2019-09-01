#include "pch.h"
#include "GuiParse.h"
namespace mini
{
	namespace ui
	{
		CGuiParse::CGuiParse()
		{
			mpGuiStyleManager = new CGuiStyleManager;
			mpGuiStyleManager->loadGuiStyleFile("resource/Editor/Gui/GuiStyle.xml");
		}

		CGuiParse::~CGuiParse()
		{
			SafeDelete(mpGuiStyleManager);
		}

		CWnd* CGuiParse::createControl(const std::string& wndType)
		{
			if (wndType == "wnd")
			{
				return new CWnd();
			}
			else if (wndType == "button")
			{
				return new CButton();
			}
			else if (wndType == "")
			{
				return new CMenu();
			}
			else if (wndType == "tableWnd")
			{
				return new CTableWnd();
			}
			else if (wndType == "radio")
			{
				return new CRadio();
			}
			else if (wndType == "")
			{
			}
			else if (wndType == "")
			{
			}
			else if (wndType == "")
			{
			}
			return nullptr;
		}

		std::map<std::string, std::string> CGuiParse::parseAttribute(TiXmlElement* pElement)
		{
			TiXmlAttribute *Attribute = pElement->FirstAttribute();  //获取brother元素的第一个属性
			std::map<std::string, std::string> attribute;
			while (Attribute != nullptr)
			{
				const char*  AttributeName = Attribute->Name();              //获取Attribute属性的名字
				const char*  AttributeValue = Attribute->Value();            //获取Attribute属性的值
				attribute[AttributeName] = AttributeValue;
				Attribute = Attribute->Next();
			}
			return attribute;
		}

		std::map<std::string, std::string>  CGuiParse::parseWnd(CWnd* pWnd, TiXmlElement* pElement)
		{
			auto attributeMap = parseAttribute(pElement);
			bindCommonAttribute(pWnd, attributeMap);
			return attributeMap;
		}

		std::map<std::string, std::string>  CGuiParse::parseFont(CWnd* pWnd, TiXmlElement* pElement)
		{
			auto attributeMap = parseAttribute(pElement);
			return attributeMap;
		}

		std::map<std::string, std::string>  CGuiParse::parseMenu(CWnd* pWnd, TiXmlElement* pElement)
		{
			CMenu* pMenuWnd = dynamic_cast<CMenu*>(pWnd);
			auto attributeMap = parseAttribute(pElement);
			bindCommonAttribute(pWnd, attributeMap);
			float x = pMenuWnd->getPosition().x;
			float y = pMenuWnd->getPosition().y;
			auto func = [&](TiXmlElement* pxmlElement, const std::string& parentName)->TiXmlElement*
			{
				TiXmlElement* pElement = nullptr;
				if (nullptr != pxmlElement)
				{
					attributeMap.clear();
					attributeMap = parseAttribute(pxmlElement);
					CWnd* pItemWnd = nullptr;
					if (attributeMap.find("type") != attributeMap.end())
					{
						pItemWnd = createControl(attributeMap["type"]);
					}
					else
					{
						pItemWnd = createControl("button");
					}
					bindCommonAttribute(pItemWnd, attributeMap);
					pItemWnd->setHeight(pMenuWnd->getHeight());
					pItemWnd->setPosition(x, y);
					x += ((int)pItemWnd->getSize().x + 1);
					if (pItemWnd != nullptr)
					{
						pMenuWnd->addItem(attributeMap["name"], pItemWnd, parentName);
					}
					pElement = pxmlElement->FirstChildElement();
				}
				return pElement;
			};
			std::string parentName = "";
			std::list<std::pair<std::string, TiXmlElement *>> pAllElement;
			TiXmlElement *pItemElement = pElement->FirstChildElement();
			if (pItemElement->FirstChildElement() != nullptr)
			{
				pAllElement.push_back(std::pair<std::string, TiXmlElement*>(parentName, pItemElement));
			}
			while (pItemElement != nullptr)
			{
				TiXmlElement* pNextElement = func(pItemElement, parentName);
				if (nullptr != pNextElement)
				{
					parentName = attributeMap["name"];
					if (pNextElement->FirstChildElement() != nullptr)
					{
						pAllElement.push_back(std::pair<std::string, TiXmlElement*>(parentName, pItemElement));
					}
					pItemElement = pNextElement;
				}
				else
				{
					pItemElement = pItemElement->NextSiblingElement();
					if (nullptr == pItemElement)
					{
						if (pAllElement.size() > 0)
						{
							auto item = pAllElement.back();
							parentName = item.first;
							pItemElement = item.second;
							pItemElement = pItemElement->NextSiblingElement();
							pAllElement.pop_back();
						}
					}
				}
			}
			pMenuWnd->parseFinished();
			return attributeMap;
		}

		std::map<std::string, std::string>  CGuiParse::parseButton(CWnd* pWnd, TiXmlElement* pElement)
		{
			auto attributeMap = parseAttribute(pElement);
			bindCommonAttribute(pWnd, attributeMap);
			return attributeMap;		
		}

		std::map<std::string, std::string> CGuiParse::parseTablwWnd(CWnd* pWnd, TiXmlElement* pElement)
		{
			auto attributeMap = parseAttribute(pElement);
			bindCommonAttribute(pWnd, attributeMap);
			return attributeMap;
		}

		std::map<std::string, std::string> CGuiParse::parseRadio(CWnd* pWnd, TiXmlElement* pElement)
		{
			auto attributeMap = parseAttribute(pElement);
			bindCommonAttribute(pWnd, attributeMap);		
			return attributeMap;
		}

		void CGuiParse::bindCommonAttribute(CWnd* pWnd, std::map<std::string, std::string>& attributeMap)
		{
			if (attributeMap.find("name") != attributeMap.end())
			{
				pWnd->setWndName(attributeMap["name"]);
			}
			if (attributeMap.find("text") != attributeMap.end())
			{
				pWnd->setWindowText(attributeMap["text"]);
			}
			if (attributeMap.find("x") != attributeMap.end())
			{
				pWnd->setX((float)atoi(attributeMap["x"].c_str()));
			}
			if (attributeMap.find("y") != attributeMap.end())
			{
				pWnd->setY((float)atoi(attributeMap["y"].c_str()));
			}
			if (attributeMap.find("width") != attributeMap.end())
			{
				pWnd->setWidth((float)atoi(attributeMap["width"].c_str()));
			}
			if (attributeMap.find("height") != attributeMap.end())
			{
				pWnd->setHeight((float)atoi(attributeMap["height"].c_str()));
			}
			if (attributeMap.find("style") != attributeMap.end())
			{
				GuiStyle* pGuiStyle = mpGuiStyleManager->loadGuiStyle(attributeMap["style"], pWnd->getWndType());
				pWnd->setGuiStyle(pGuiStyle);
			}
		}
	}
}
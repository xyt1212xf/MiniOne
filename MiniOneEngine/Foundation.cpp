#include "pch.h"
#include "Foundation.h"
namespace mini
{
	namespace foundation
	{
		const std::string getExterName(const std::string& strName)
		{
			if (std::string::npos == strName.rfind("."))
			{
				return "";
			}
			else
			{
				return strName.substr(strName.rfind(".") + 1);
			}
		}
	}
}


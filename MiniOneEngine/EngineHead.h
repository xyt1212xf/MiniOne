#pragma once

#include "PlugInEnum.h"
#include "plugin.h"
#include <vector>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <unordered_map>
#include <string>
#include <process.h>
#include <atomic>
#include <fstream>
#include <tuple>
#include <mutex>  
#include <initializer_list>
#include <algorithm>
#include "Foundation.h"
#include "TinyXML/tinyxml.h"


#ifdef WIN32
#include <windows.h>
#endif // WIN32

#pragma comment(lib, "Lua535.lib")
#pragma comment(lib, "TinyXML.lib")
#pragma comment(lib, "libfbxsdk.lib")

#define CastTo(pObject, type) dynamic_cast<type>(pObject);

#define SafeDeleteArray( pObject ) \
	if(pObject != nullptr)\
	{\
		delete[] pObject;\
		pObject = nullptr;\
	}

#define SafeDelete( pObject ) \
	if(pObject != nullptr)\
	{\
		delete pObject;\
		pObject = nullptr;\
	}

#define SafeRelease( pObject ) \
	if(pObject != nullptr)\
	{\
		pObject->Release();\
		pObject = nullptr;\
	}

#include "TSingle.h"
#include "Refcount.h"

namespace mini
{
	enum RendererType
	{
		DeferredRenderer = 1 << 1,
		FrontRenderer = 1 << 2,
		ShadowRenderer = 1 << 3,
		PostRenderer = 1 << 4,
		GuiRenderer = 1 << 5,
		UnknowType = 255,
	};
}
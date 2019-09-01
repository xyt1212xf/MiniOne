#pragma once
#include "DevicePlatform.h"
namespace mini
{
#ifdef WIN32
#define CVector D3DXVECTOR3
#else
class CVector
{
public:
	CVector();
	~CVector();
};
#endif
}

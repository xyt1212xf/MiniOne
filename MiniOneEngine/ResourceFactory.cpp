#include "pch.h"
#include "ResourceFactory.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Wnd.h"
#include "Font.h"
namespace mini
{
	CResourceFactory::CResourceFactory()
	{
	}


	CResourceFactory::~CResourceFactory()
	{
	}


	CResourceBase* CResourceFactory::createResource(ResType type)
	{
		CResourceBase* pResourceBase = nullptr;
		switch (type)
		{
		case Texture:
			pResourceBase = new CTexture();
			break;
		case Shader:
			pResourceBase = new CShader();
			break;
		case Mesh:
			pResourceBase = new CMesh();
			break;
		case Material:
			pResourceBase = new CMaterial();
			break;
		case Wnd:
			pResourceBase = new ui::CWnd();
			break;
		case Font:
			pResourceBase = new CFont();
			break;
		default:
			break;
		}
		return pResourceBase;
	}
}

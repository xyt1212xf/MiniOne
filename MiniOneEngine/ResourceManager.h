#pragma once
#include "EngineHead.h"
#include "ResourceBase.h"
#include "ResourceFactory.h"
#include "ScriptHead.h"
#include <fbxsdk.h>
namespace mini
{
	class CMeshUnit;
	class CMesh;
	class CShader;
	/*
	struct ShaderInfo
	{
		std::string vertexVersion;
		std::string pixelVersion;
		std::string vsFile;
		std::string psFile;
		std::string vsMain;
		std::string psMain;
	};
	*/
	class CResourceManager : public TSingle<CResourceManager>
	{
		template<typename T>
		T getTableItem(lua_State* pLua, const char* tableName, const char* itemName)
		{
			int t = lua_gettop(pLua);
			lua_getglobal(pLua, tableName);
			lua_pushstring(pLua, itemName);
			lua_gettable(pLua, -2);
			if (!lua_isnil(pLua, -1))
			{
				return (T)lua_tonumber(pLua, -1);
			}
			else
			{
				lua_settop(pLua, t);
				return -1;
			}
		}

		template<>
		const char* getTableItem(lua_State* pLua, const char* tableName, const char* itemName)
		{
			int t = lua_gettop(pLua);
			lua_getglobal(pLua, tableName);
			lua_pushstring(pLua, itemName);
			lua_gettable(pLua, -2);
			if (!lua_isnil(pLua, -1))
			{
				return lua_tostring(pLua, -1);
			}
			else
			{
				lua_settop(pLua, t);
				return "";
			}
		}
	public:
		CResourceManager();
		virtual ~CResourceManager();
		void addResource(CResourceBase* pBase);
		void releaseResource(CResourceBase* pBase);
		void clearResource(CResourceBase* pBase);
		CResourceBase*	createResource(ResType type)noexcept;
		CResourceBase*	createResource(std::string fileName, ResType type)noexcept;
		CResourceBase*	findResource(std::string fileName);

		template<typename T>
		CMeshUnit* createMeshUnit(void* pVertices, UINT32 verticeSize, UINT32* pIndices, UINT32 indicesSize);

	private:
		bool loadGui(CResourceBase* pResourceBase, const std::string& strFile);
		bool loadResource(CResourceBase* pResourceBase, const std::string& strFile);
		bool loadTexture(CResourceBase* pResourceBase, const std::string& strFile);
		bool loadShader(CResourceBase* pResourceBase, const std::string& strShaderName);
		bool loadMaterial(CResourceBase* pResourceBase, const std::string& strFile);
		bool loadMesh(CResourceBase* pMesh, const std::string& strFile);
		bool loadMesh_m(CMesh* pMesh, const std::string& strFile);
		bool loadMesh_mTwo(CMesh* pMesh, const std::string& strFile);
		bool loadMesh_fbx(CMesh* pMesh, const std::string& strFile);
		bool loadMesh_txt(CMesh* pMesh, const std::string& strFile);
		bool loadFont(CResourceBase* pResourceBase, const std::string& strFile);
		bool InitializeFbxSdkObjects();
		void DestroyFbxSdkObjects();
		void addVSConstBuffer(CShader* pShader);
		void addPSConstBuffer(CShader* pShader);

	private:
		std::map<UINT32, CResourceBase*>		mResourceMap;
		std::map<std::string, CResourceBase*>	mResourceMapByName;
		CResourceFactory*					mpFactory;
		FbxManager*							mpFbxSdkManager;
	};

	template<typename T>
	CMeshUnit* CResourceManager::createMeshUnit(void* pVertices, UINT32 verticeSize, UINT32* pIndices, UINT32 indicesSize)
	{
		CResourceBase* pResource = createResource(ResType::Mesh);
		CMesh* pMesh = dynamic_cast<CMesh*>(pResource);

		pMesh->fillVertexBuffer(pVertices, verticeSize, sizeof(T));
		pMesh->fillIndexBuffer(pIndices, indicesSize, sizeof(UINT32));

		CMeshUnit* pMeshUnit = new CMeshUnit();
		pMeshUnit->bindMesh(pMesh);
		SafeRelease(pMesh);
		return pMeshUnit;
	}

#define GResourceManager CResourceManager::getSinglePtr()	
}


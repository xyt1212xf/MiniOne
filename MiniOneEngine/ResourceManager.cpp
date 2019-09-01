#include "pch.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Font.h"
#include "RendererPlugin.h"
#include "Vector.h"
#include "Skeleton.h"
namespace mini
{
	std::vector<CPoint3F>			skinInfo;
	std::vector<Bone>				bones;
	std::vector<std::list<float>>	vertexboneWeight;
	std::vector<std::list<INT64>>	vertexBoneIndex;
	std::map<INT64, Bone>			boneLayer;

	UINT32 readFbxVertice(FbxMesh* pFbxMesh, INT32 index, CPoint3F* vertex);
	UINT32 readFbxNormal(FbxMesh* pFbxMesh, INT32 index, INT32 vertexCount, CPoint3F* normal);
	UINT32 readFbxUV(FbxMesh* pFbxMesh, INT32 index, INT32 textureUVIndex, CPoint2F* uv);
	UINT32 readFbxTangent(FbxMesh* pFbxMesh, INT32 index, CPoint3F* tangent);
	UINT32 readFbxBinormal(FbxMesh* pFbxMesh, INT32 index, CPoint3F* binormal);

	bool readFbxSkeleton(FbxNode* pFbxNode, INT64 parentId);
	bool readSkinInfo(FbxNode* pFbxNodeNode, VertexFormatVTNTBSkin* pVertices);
	void readFbxMeshFinished(CMesh* pMesh);

	INT32 calculateTableLength(lua_State* pLua)
	{
		INT32 nCount = 1;
		INT32 _t = lua_gettop(pLua);
		lua_pushnil(pLua);

		while (lua_next(pLua, -2))
		{
			int nkey = lua_type(pLua, -2);
			lua_pop(pLua, 1);
			++nCount;
		}
		lua_settop(pLua, _t);
		return nCount;
	}

	void CalculateTangentBinormal(VertexFormatVTNTB& vertex0,
		VertexFormatVTNTB& vertex1,
		VertexFormatVTNTB& vertex2,
		CPoint3F& tangent, CPoint3F& binormal)
	{
		float vector1[3], vector2[3];
		float tuVector[2], tvVector[2];

		// Calculate the two vectors for this face.
		vector1[0] = vertex1.position.x - vertex0.position.x;
		vector1[1] = vertex1.position.y - vertex0.position.y;
		vector1[2] = vertex1.position.z - vertex0.position.z;

		vector2[0] = vertex2.position.x - vertex0.position.x;
		vector2[1] = vertex2.position.y - vertex0.position.y;
		vector2[2] = vertex2.position.z - vertex0.position.z;

		// Calculate the tu and tv texture space vectors.
		tuVector[0] = vertex1.uv.x - vertex0.uv.x;
		tvVector[0] = vertex1.uv.y - vertex0.uv.y;

		tuVector[1] = vertex2.uv.x - vertex0.uv.x;
		tvVector[1] = vertex2.uv.y - vertex0.uv.y;

		// Calculate the denominator of the tangent/binormal equation.
		float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

		// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
		tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
		tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
		tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

		binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
		binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
		binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

		// Calculate the length of this normal.
		float length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

		// Normalize the normal and then store it
		tangent.x = tangent.x / length;
		tangent.y = tangent.y / length;
		tangent.z = tangent.z / length;

		// Calculate the length of this normal.
		length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

		// Normalize the normal and then store it
		binormal.x = binormal.x / length;
		binormal.y = binormal.y / length;
		binormal.z = binormal.z / length;
	}

	void CalculateNormal(CPoint3F& tangent, CPoint3F& binormal, CPoint3F& normal)
	{
		// Calculate the cross product of the tangent and binormal which will give the normal vector.
		normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
		normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
		normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

		// Calculate the length of the normal.
		float length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

		// Normalize the normal.
		normal.x = normal.x / length;
		normal.y = normal.y / length;
		normal.z = normal.z / length;
	}

	CResourceManager::CResourceManager()
	{
		mpFactory = CResourceFactory::initialize();
		InitializeFbxSdkObjects();
	}

	CResourceManager::~CResourceManager()
	{
		DestroyFbxSdkObjects();
		mpFactory = nullptr;
		CResourceFactory::destory();
	}

	void CResourceManager::addResource(CResourceBase* pBase)
	{
		if (pBase != nullptr)
		{
			pBase->AddRef();
			UINT32 key = pBase->getResourceID();
			mResourceMap[key] = pBase;
		}
	}

	void CResourceManager::releaseResource(CResourceBase* pBase)
	{
		if (pBase != nullptr)
		{
			UINT32 key = pBase->getResourceID();
			auto item = mResourceMap.find(key);
			if (item != mResourceMap.end())
			{
				SafeRelease(item->second);
			}
		}
	}

	void CResourceManager::clearResource(CResourceBase* pBase)
	{
		if (pBase != nullptr)
		{
			UINT32 key = pBase->getResourceID();
			auto item = mResourceMap.find(key);
			if (item != mResourceMap.end())
			{
				mResourceMap.erase(item);
			}
			std::string& strName = pBase->getResourceName();
			auto nameItem = mResourceMapByName.find(strName);
			if (nameItem != mResourceMapByName.end())
			{
				mResourceMapByName.erase(nameItem);
			}
		}
	}

	CResourceBase* CResourceManager::createResource(ResType type) noexcept
	{
		try
		{
			CResourceBase* pResource = mpFactory->createResource(type);
			if (pResource != nullptr)
			{
				UINT32 uuid = pResource->getResourceID();
				mResourceMap[uuid] = pResource;
				return pResource;
			}
			else
			{
				throw(0);
			}
		}
		catch (...)
		{
			return nullptr;
		}
	}


	CResourceBase* CResourceManager::createResource(std::string fileName, ResType type) noexcept
	{
		std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
		if (fileName.length() > 0)
		{
			auto item = mResourceMapByName.find(fileName);
			if (item == mResourceMapByName.end())
			{
				CResourceBase* pResource = createResource(type);
				pResource->setResourceName(fileName);
				mResourceMapByName[fileName] = pResource;
				UINT32 uuid = pResource->getResourceID();
				mResourceMap[uuid] = pResource;
				loadResource(pResource, fileName);
				return pResource;
			}
			else
			{
				return (item->second);
			}
		}
		else
		{
			return createResource(type);
		}
	}

	CResourceBase* CResourceManager::findResource(std::string fileName)
	{
		std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
		auto item = mResourceMapByName.find(fileName);
		if (item == mResourceMapByName.end())
		{
			return nullptr;
		}
		else
		{
			return item->second;
		}
	}

	bool CResourceManager::loadResource(CResourceBase* pResourceBase, const std::string& strFile)
	{
		bool bLoadResoult = false;
		if (pResourceBase != nullptr)
		{
			switch (pResourceBase->getResourceType())
			{
			case Texture:
				bLoadResoult = loadTexture(pResourceBase, strFile);
				break;
			case Shader:
				bLoadResoult = loadShader(pResourceBase, strFile);
				break;
			case Mesh:
				bLoadResoult = loadMesh(pResourceBase, strFile);
				break;
			case Material:
				bLoadResoult = loadMaterial(pResourceBase, strFile);
				break;
			case Wnd:
				bLoadResoult = loadGui(pResourceBase, strFile);
				break;
			case Font:
				bLoadResoult = loadFont(pResourceBase, strFile);
			}
		}
		return bLoadResoult;
	}

	bool CResourceManager::loadTexture(CResourceBase* pResourceBase, const std::string& strFile)
	{
		pResourceBase->setResourceName(strFile);
		return pResourceBase->initialise();
	}

	bool CResourceManager::loadShader(CResourceBase* pResourceBase, const std::string& strShaderName)
	{
		bool bResult = false;
		lua_State* pLua = CScriptPlugin::getSinglePtr()->getLuaState();
		if (pLua != nullptr)
		{
			CShader* pShader = dynamic_cast<CShader*>(pResourceBase);
			if (pShader != nullptr)
			{
				pShader->setDevice(CRendererPlugin::getSinglePtr()->getDevice());
				int iTop = lua_gettop(pLua);
				if (pShader != nullptr)
				{
					std::string vsFile = getTableItem<const char*>(pLua, strShaderName.c_str(), "vs");
					std::string vsMain = getTableItem<const char*>(pLua, strShaderName.c_str(), "vsMain");
					std::string vertexVersion = getTableItem<const char*>(pLua, strShaderName.c_str(), "vsVersion");
					std::string psFile = getTableItem<const char*>(pLua, strShaderName.c_str(), "ps");
					std::string psMain = getTableItem<const char*>(pLua, strShaderName.c_str(), "psMain");
					std::string pixelVersion = getTableItem<const char*>(pLua, strShaderName.c_str(), "psVersion");

					bResult = (pShader->createVextexShader(vsFile, vsMain, vertexVersion) &&
						pShader->createPixelShader(psFile, psMain, pixelVersion));
					lua_settop(pLua, iTop);
				}
			}
		}
		return bResult;
	}

	bool CResourceManager::loadGui(CResourceBase* pResourceBase, const std::string& strFile)
	{
		/*
		ui::CWnd* pWnd = dynamic_cast<ui::CWnd*>(pResourceBase);
		lua_State* pLua = CScriptPlugin::getSinglePtr()->getLuaState();

		CLuaBind::doFile(strFile.c_str());
		int iTop = lua_gettop(pLua);

		UINT32 wndType = getTableItem<UINT32>(pLua, "miniWnd", "wndType");
		std::string strContent = getTableItem<const char*>(pLua, "miniWnd", "position");
		auto split = foundation::splitString<float>(strContent, ",");
		pWnd->setPosition(split[0], split[1]);

		strContent = getTableItem<const char*>(pLua, "miniWnd", "size");
		split = foundation::splitString<float>(strContent, ",");
		pWnd->setSize(split[0], split[1]);

		strContent = getTableItem<const char*>(pLua, "miniWnd", "texture");
		CResourceBase* pResource = CResourceManager::getSinglePtr()->findResource(strContent);
		CTexture* pTexture = nullptr;
		if (pResource == nullptr)
		{
			pTexture = dynamic_cast<CTexture*>(CResourceManager::getSinglePtr()->createResource(strContent, Texture));
		}
		else
		{
			pTexture = dynamic_cast<CTexture*>(pResource);
		}
		pWnd->setTexture(pTexture);
		SafeRelease(pTexture);;
		*/
		return true;
	}

	bool CResourceManager::loadMaterial(CResourceBase* pResourceBase, const std::string& strFile)
	{
		try
		{
			CMaterial* pMaterial = dynamic_cast<CMaterial*>(pResourceBase);
			lua_State* pLua = CScriptPlugin::getSinglePtr()->getLuaState();

			CLuaBind::doFile(strFile.c_str());
			int iTop = lua_gettop(pLua);

			std::string strVS = getTableItem<const char*>(pLua, "material", "vs");
			std::string strPS = getTableItem<const char*>(pLua, "material", "ps");

			CResourceBase* pResource = CResourceManager::getSinglePtr()->findResource(strVS + strPS);
			bool bResult = true;
			if (pResource == nullptr)
			{
				std::string strVSVersion = getTableItem<const char*>(pLua, "material", "vsVersion");

				std::string strPSVersion = getTableItem<const char*>(pLua, "material", "psVersion");

				std::string strVSMain = getTableItem<const char*>(pLua, "material", "vsMain");

				std::string strPSMain = getTableItem<const char*>(pLua, "material", "psMain");

				UINT32 uNum = getTableItem<UINT32>(pLua, "material", "elementItemCount");
				if (uNum == -1)
				{
					throw 0;
				}
				std::vector<D3D11_INPUT_ELEMENT_DESC> input_element;
				input_element.resize(uNum);

				for (UINT32 i = 0; i < uNum; ++i)
				{
					lua_getglobal(pLua, "inputElementLayout");

					/* 把函数所需要的参数入栈 */
					lua_pushnumber(pLua, i+1);
					lua_call(pLua, 1, 7);
					input_element[i].SemanticName = lua_tostring(pLua, -7);
					input_element[i].SemanticIndex = (UINT)lua_tointeger(pLua, -6);
					input_element[i].Format = (DXGI_FORMAT)lua_tointeger(pLua, -5);
					input_element[i].InputSlot = (UINT)lua_tointeger(pLua, -4);
					input_element[i].AlignedByteOffset = (UINT)lua_tointeger(pLua, -3);
					input_element[i].InputSlotClass = (D3D11_INPUT_CLASSIFICATION)lua_tointeger(pLua, -2);
					input_element[i].InstanceDataStepRate = (UINT)lua_tointeger(pLua, -1);
				}

				lua_getglobal(pLua, "inputElementLayout");
				pMaterial->mpShader = dynamic_cast<CShader*>(CResourceManager::getSinglePtr()->createResource(Shader));
				pMaterial->mpShader->setDevice(CRendererPlugin::getSinglePtr()->getDevice());
				bResult = (	pMaterial->mpShader->createVextexShader(strVS, strVSMain, strVSVersion, &input_element[0], uNum) &&
							pMaterial->mpShader->createPixelShader(strPS, strPSMain, strPSVersion));

				
			}
			else
			{
				pMaterial->mpShader = dynamic_cast<CShader*>(pResource);
				pMaterial->mpShader->AddRef();
			}
		
			UINT32 nPixeConstSize = 0;
			std::string strContent = getTableItem<const char*>(pLua, "material", "DiffuseColor");	
			if (strContent.length() > 0)
			{
				auto split = foundation::splitString<float>(strContent, ",");
				nPixeConstSize += sizeof(CPoint4F);
				memcpy(&pMaterial->mConstBuffer.mDiffuseColor[0], &split[0], sizeof(pMaterial->mConstBuffer.mDiffuseColor));
			}
			strContent = getTableItem<const char*>(pLua, "material", "AmbientColor");
			if (strContent.length() > 0)
			{
				auto split = foundation::splitString<float>(strContent, ",");
				nPixeConstSize += sizeof(CPoint4F);
				memcpy(&pMaterial->mConstBuffer.mAmbientColor[0], &split[0], sizeof(pMaterial->mConstBuffer.mAmbientColor));
			}

			strContent = getTableItem<const char*>(pLua, "material", "SpecularColor");
			if (strContent.length() > 0)
			{
				auto split = foundation::splitString<float>(strContent, ",");
				nPixeConstSize += sizeof(CPoint4F);
				memcpy(&pMaterial->mConstBuffer.mSpecularColor[0], &split[0], sizeof(pMaterial->mConstBuffer.mSpecularColor));
				strContent = getTableItem<const char*>(pLua, "material", "SpecularColor");
				pMaterial->mConstBuffer.mSpecularPower = foundation::stringToType<float>(strContent);
				pMaterial->mpShader->createVertexConstBuffer(sizeof(CPoint4F), 1);
			}
			
			addPSConstBuffer(pMaterial->mpShader);
			auto matrices = CRendererPlugin::getSinglePtr()->getTransposeMatrices();
			pMaterial->mpShader->createVertexConstBuffer(matrices.size() * sizeof(CMatrix), 0);

			std::stringstream stream;
			std::string textureName = "";
			
			bool bCreteSmaplerDesc = false;
			for (INT32 i = 0; i < 10; ++i)
			{
				stream << i;
				textureName = "texture" + stream.str();
				stream.str("");
				strContent = getTableItem<const char*>(pLua, "material", textureName.c_str());
				if (strContent.length() > 0)
				{
					D3D11_SAMPLER_DESC samplerDesc;
					if (!bCreteSmaplerDesc)
					{
						bCreteSmaplerDesc = true;
						samplerDesc.Filter = (D3D11_FILTER)getTableItem<UINT32>(pLua, "textureSampler", "Filter");
						samplerDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)getTableItem<UINT32>(pLua, "textureSampler", "AddressU");
						samplerDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)getTableItem<UINT32>(pLua, "textureSampler", "AddressV");
						samplerDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)getTableItem<UINT32>(pLua, "textureSampler", "AddressW");
						samplerDesc.MipLODBias = getTableItem<float>(pLua, "textureSampler", "MipLODBias");
						samplerDesc.MaxAnisotropy = getTableItem<UINT32>(pLua, "textureSampler", "MaxAnisotropy");
						samplerDesc.ComparisonFunc = (D3D11_COMPARISON_FUNC)getTableItem<UINT32>(pLua, "textureSampler", "ComparisonFunc");
						samplerDesc.BorderColor[0] = 0;
						samplerDesc.BorderColor[1] = 0;
						samplerDesc.BorderColor[2] = 0;
						samplerDesc.BorderColor[3] = 0;
						samplerDesc.MinLOD = getTableItem<float>(pLua, "textureSampler", "MinLOD");
						samplerDesc.MaxLOD = 1 >= getTableItem<float>(pLua, "textureSampler", "MaxLOD") ? D3D11_FLOAT32_MAX : getTableItem<float>(pLua, "textureSampler", "MaxLOD");
					}
					CResourceBase* pResource = CResourceManager::getSinglePtr()->findResource(strContent);

					CTexture* pTexture = nullptr;
					if (pResource == nullptr)
					{
						pTexture = dynamic_cast<CTexture*>(CResourceManager::getSinglePtr()->createResource(strContent, Texture));
						// Create a texture sampler state description.
						pTexture->initialiseSampler(&samplerDesc);
						pMaterial->mTextures.push_back(pTexture);
					}
					else
					{
						pTexture = dynamic_cast<CTexture*>(pResource);
						pMaterial->mTextures.push_back(pTexture);
						pResource->AddRef();
					}
				}
				else
				{
					break;
				}
			}


			lua_settop(pLua, iTop);
			return bResult;
		}
		catch (...)
		{
			return false;
		}
		return true;
	}

	bool CResourceManager::loadMesh(CResourceBase* pResourceBase, const std::string& strFile)
	{
		auto pMesh = dynamic_cast<CMesh*>(pResourceBase);
		if (foundation::getExterName(strFile) == "m")
		{
			return loadMesh_m(pMesh, strFile);
		}
		if (foundation::getExterName(strFile) == "m2")
		{
			return loadMesh_mTwo(pMesh, strFile);
		}
		else if (foundation::getExterName(strFile) == "fbx")
		{
			return loadMesh_fbx(pMesh, strFile);
		}
		else if (foundation::getExterName(strFile) == "txt")
		{
			return loadMesh_txt(pMesh, strFile);
		}
		return true;
	}


	bool CResourceManager::loadMesh_m(CMesh* pMesh, const std::string& strFile)
	{
		std::ifstream fin;
		char input = 0;

		// Open the model file.
		fin.open(strFile.c_str());

		// If it could not open the file then exit.
		if (fin.fail())
		{
			return false;
		}

		// Read up to the value of vertex count.
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}

		// Read in the vertex count.
		INT32 vertexCount = 0;
		fin >> vertexCount;

		// Set the number of indices to be the same as the vertex count.
		INT32 indexCount = vertexCount;
		std::vector<VertexFormatVTN> vertices;
		std::vector<UINT32> indices;
		vertices.resize(vertexCount);
		indices.resize(vertexCount);
		// Read up to the beginning of the data.
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		// Read in the vertex data.
		for (INT32 i = 0; i < vertexCount; ++i)
		{
			fin >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;
			fin >> vertices[i].uv.x >> vertices[i].uv.y;
			fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
			indices[i] = i;
		}

		// Close the model file.
		fin.close();

		pMesh->fillVertexBuffer(&vertices[0], sizeof(VertexFormatVTN)* vertices.size(), sizeof(VertexFormatVTN));
		pMesh->fillIndexBuffer(&indices[0], sizeof(UINT32) * indices.size(), sizeof(UINT32));
		return true;
	}

	bool CResourceManager::loadMesh_mTwo(CMesh* pMesh, const std::string& strFile)
	{
		std::ifstream fin;
		char input = 0;

		// Open the model file.
		fin.open(strFile.c_str());

		// If it could not open the file then exit.
		if (fin.fail())
		{
			return false;
		}

		// Read up to the value of vertex count.
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}

		// Read in the vertex count.
		INT32 vertexCount = 0;
		fin >> vertexCount;

		// Set the number of indices to be the same as the vertex count.
		INT32 indexCount = vertexCount;
		std::vector<VertexFormatVTNTB> vertices;
		std::vector<UINT32> indices;
		vertices.resize(vertexCount);
		indices.resize(vertexCount);
		// Read up to the beginning of the data.
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		// Read in the vertex data.
		for (INT32 i = 0; i < vertexCount; ++i)
		{
			fin >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;
			fin >> vertices[i].uv.x >> vertices[i].uv.y;
			fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
			indices[i] = i;
		}
		INT32 faceNumber = indices.size() / 3;
		INT32 index = 0;
		CPoint3F tangent, binormal, normal;
		for (INT32 i = 0; i < faceNumber; ++i)
		{
			index = i * 3;
			CalculateTangentBinormal(vertices[index + 0], vertices[index + 1], vertices[index + 2], tangent, binormal);

			// Calculate the new normal using the tangent and binormal.
			CalculateNormal(tangent, binormal, normal);

			vertices[index + 0].normal = normal;
			vertices[index + 0].tangent = tangent;
			vertices[index + 0].binormal = binormal;

			vertices[index + 1].normal = normal;
			vertices[index + 1].tangent = tangent;
			vertices[index + 1].binormal = binormal;

			vertices[index + 2].normal = normal;
			vertices[index + 2].tangent = tangent;
			vertices[index + 2].binormal = binormal;
		}
		// Close the model file.
		fin.close();

		pMesh->fillVertexBuffer(&vertices[0], sizeof(VertexFormatVTNTB)* vertices.size(), sizeof(VertexFormatVTNTB));
		pMesh->fillIndexBuffer(&indices[0], sizeof(UINT32) * indices.size(), sizeof(UINT32));
		return true;
	}


	bool readFbxSkeleton(FbxNode* pFbxNode, INT64 parentId)
	{
		bool haveSkeleton = false;
		for (int i = 0; i < pFbxNode->GetChildCount(); i++)
		{
			FbxNode* pNode = pFbxNode->GetChild(i);
			for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
			{
				const FbxNodeAttribute* pAttribute = pNode->GetNodeAttributeByIndex(i);
				pAttribute->GetAttributeType();
				switch (pAttribute->GetAttributeType())
				{
				case FbxNodeAttribute::eSkeleton:
				{
					haveSkeleton = true;
					Bone bone;
					bone.nParentIndex = parentId;
					bone.boneName = pNode->GetName();
					INT64 id = pNode->GetUniqueID();
					bone.uniqueID = id;
					boneLayer[id] = bone;
					readFbxSkeleton(pNode, id);
					haveSkeleton = true;
				}
				}
			}
		}
		return haveSkeleton;
	}

	bool readSkinInfo(FbxNode* pNode, VertexFormatVTNTBSkin* pVertices)
	{
		if (nullptr == pNode || nullptr == pVertices)
		{
			return false;
		}
		do
		{
			const FbxNodeAttribute* pNodeAttr = pNode->GetNodeAttribute();
			if (pNodeAttr == 0)
			{
				break;
			}
			FbxMesh* pMesh = pNode->GetMesh();
			if (pMesh == 0)
			{
				break;
			}
			std::map<UINT32, INT32> recordBoneIndex;
			const int nDeformerCount = pMesh->GetDeformerCount();
			for (int deformerIndex = 0; deformerIndex < nDeformerCount; ++deformerIndex)
			{
				const int nDeformerCount = pMesh->GetDeformerCount();
				for (int deformerIndex = 0; deformerIndex < nDeformerCount; ++deformerIndex)
				{
					FbxSkin* pSkin = reinterpret_cast<FbxSkin*>(pMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
					if (pSkin == 0)
					{
						continue;
					}
					const int nClusterCount = pSkin->GetClusterCount();
					for (int clusterIndex = 0; clusterIndex < nClusterCount; ++clusterIndex)
					{
						FbxCluster* pCluster = pSkin->GetCluster(clusterIndex);
						INT64 boneId = pCluster->GetLink()->GetUniqueID();
						auto item = boneLayer.find(boneId);
						if (item == boneLayer.end())
						{
							continue;
						}

						// Associate each joint with the control points it affects
						const int nControlPointCount = pCluster->GetControlPointIndicesCount();
						const int* pCPIndexList = pCluster->GetControlPointIndices();
						const double* pWeightList = pCluster->GetControlPointWeights();
						for (int cpIndex = 0; cpIndex < nControlPointCount; ++cpIndex)
						{
							INT32 index = recordBoneIndex[cpIndex]++;
							pVertices[cpIndex].skinWeight.m[index] = (float)pWeightList[cpIndex];
							pVertices[cpIndex].BoneIndex.m[index] = (INT32)boneId;
						}
					}
				}
			}

		} while (0);
		return true;
	}

	void readFbxMeshFinished(CMesh* pMesh)
	{
		//	pMesh->mVertices.resize(size);
		//	pMesh->mIndices.resize(triangleCount * 3);				
	}

	bool CResourceManager::loadMesh_fbx(CMesh* pMesh, const std::string& strFile)
	{
		/*
		std::ifstream stream;
		stream.open(strFile.c_str(), std::ios_base::in | std::ios::binary);
		if (!stream.bad())
		{
			std::stringstream buffer;
			buffer << stream.rdbuf();
			std::string contents(buffer.str());
			stream.close();
		}
		*/
		FbxScene* pSDKScene = FbxScene::Create(mpFbxSdkManager, "");
		FbxImporter* pSDKImporter = FbxImporter::Create(mpFbxSdkManager, "");

		if (!pSDKImporter->Initialize(strFile.c_str(), -1, mpFbxSdkManager->GetIOSettings()))
		{
			return false;
		}
		if (!pSDKImporter->Import(pSDKScene))
		{
			return false;
		}

		FbxAxisSystem OurAxisSystem(FbxAxisSystem::eDirectX);
		FbxAxisSystem SceneAxisSystem = pSDKScene->GetGlobalSettings().GetAxisSystem();
		if (SceneAxisSystem != OurAxisSystem)
		{
			OurAxisSystem.ConvertScene(pSDKScene);
		}

		FbxGeometryConverter lGeomConverter(mpFbxSdkManager);
		lGeomConverter.Triangulate(pSDKScene, true);

		FbxNode* pRootNode = pSDKScene->GetRootNode();
		bool existSkeleton = false;
		if (pRootNode)
		{
			existSkeleton = readFbxSkeleton(pRootNode, 0);

			float* pVertices = nullptr;
			if (existSkeleton)
			{
				pVertices = (float*)&pMesh->mVerticesSkin[0];
			}
			else
			{
				pVertices = (float*)&pMesh->mVertices[0];
			}
			for (int i = 0; i < pRootNode->GetChildCount(); i++)
			{
				FbxNode* pNode = pRootNode->GetChild(i);
				const char* nodeName = pNode->GetName();
				FbxDouble3 translation = pNode->LclTranslation.Get();
				FbxDouble3 rotation = pNode->LclRotation.Get();
				FbxDouble3 scaling = pNode->LclScaling.Get();

				for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
				{
					const FbxNodeAttribute* pAttribute = pNode->GetNodeAttributeByIndex(i);
					pAttribute->GetAttributeType();
					switch (pAttribute->GetAttributeType())
					{
					case FbxNodeAttribute::eUnknown:
					case FbxNodeAttribute::eNull:
						break;
					case FbxNodeAttribute::eMesh:
					{
						FbxMesh* pFbxMesh = pNode->GetMesh();
						int vertexCount = pFbxMesh->GetControlPointsCount();
						int triangleCount = pFbxMesh->GetPolygonCount();
						vertexboneWeight.resize(vertexCount);
						vertexBoneIndex.resize(vertexCount);
						int index = 0;
						int ctrlPointIndex = 0;
						byte* pData = nullptr;
						if (existSkeleton)
						{
							pMesh->mVerticesSkin.resize(vertexCount);
							pData = (byte*)&pMesh->mVerticesSkin[0];
							pMesh->mVertexSize = sizeof(VertexFormatVTNTBSkin);
						}
						else
						{
							pMesh->mVertices.resize(vertexCount);
							pData = (byte*)&pMesh->mVertices[0];
							pMesh->mVertexSize = sizeof(VertexFormatVTNTB);
						}

						pMesh->mIndices.resize(triangleCount * 3);
						for (INT32 i = 0; i < triangleCount; ++i)
						{
							for (INT32 j = 0; j < 3; ++j)
							{
								ctrlPointIndex = pFbxMesh->GetPolygonVertex(i, j);
								pMesh->mIndices[index] = ctrlPointIndex;
								byte* pTempData = pData + pMesh->mVertexSize * ctrlPointIndex;
								pTempData += readFbxVertice(pFbxMesh, ctrlPointIndex, (CPoint3F*)pTempData);
								pTempData += readFbxNormal(pFbxMesh, ctrlPointIndex, index++, (CPoint3F*)pTempData);
								pTempData += readFbxUV(pFbxMesh, ctrlPointIndex, pFbxMesh->GetTextureUVIndex(i, j), (CPoint2F*)pTempData);
								pTempData += readFbxTangent(pFbxMesh, ctrlPointIndex, (CPoint3F*)pTempData);
								pTempData += readFbxBinormal(pFbxMesh, ctrlPointIndex, (CPoint3F*)pTempData);
							}
						}
						if (existSkeleton)
						{
							readSkinInfo(pNode, &pMesh->mVerticesSkin[0]);
						}
						else
						{

						}
						pFbxMesh->Destroy();
					}

					break;
					}
				}
			}
		}
		pSDKImporter->Destroy();
		pSDKScene->Destroy();
		if (existSkeleton)
		{
			pMesh->fillVertexBuffer(&pMesh->mVerticesSkin[0], sizeof(VertexFormatVTNTBSkin)* pMesh->mVerticesSkin.size(), sizeof(VertexFormatVTNTBSkin));
			pMesh->fillIndexBuffer(&pMesh->mIndices[0], sizeof(UINT32) * pMesh->mIndices.size(), sizeof(UINT32));
		}
		else
		{
			pMesh->fillVertexBuffer(&pMesh->mVertices[0], sizeof(VertexFormatVTNTB)* pMesh->mVertices.size(), sizeof(VertexFormatVTNTB));
			pMesh->fillIndexBuffer(&pMesh->mIndices[0], sizeof(UINT32) * pMesh->mIndices.size(), sizeof(UINT32));
		}
		return true;
	}

	bool CResourceManager::loadMesh_txt(CMesh* pMesh, const std::string& strFile)
	{
		std::ifstream fin;
		char input;


		// Open the model file.
		fin.open(strFile.c_str());

		// If it could not open the file then exit.
		if (fin.fail())
		{
			return false;
		}

		// Read up to the value of vertex count.
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		INT32 vertexCount = 0;
		// Read in the vertex count.
		fin >> vertexCount;

		// Set the number of indices to be the same as the vertex count.
	//	m_indexCount = vertexCoun;
		pMesh->mVertexCount = vertexCount;
		pMesh->mVertices.resize(vertexCount);
		pMesh->mIndices.resize(vertexCount);

		auto& vertices = pMesh->mVertices;

		// Read up to the beginning of the data.
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		// Read in the vertex data.
		for (INT32 i = 0; i < vertexCount; i++)
		{
			fin >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;
			fin >> vertices[i].uv.x >> vertices[i].uv.y;
			fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
			pMesh->mIndices[i] = i;
		}

		// Close the model file.
		fin.close();
		pMesh->fillVertexBuffer(&vertices[0], sizeof(VertexFormatVTNTB)*vertexCount, sizeof(VertexFormatVTNTB));
		pMesh->fillIndexBuffer(&pMesh->mIndices[0], sizeof(UINT32)* vertexCount, sizeof(UINT32));
		return true;
	}

	bool CResourceManager::loadFont(CResourceBase* pResourceBase, const std::string& strFile)
	{
		CFont* pFont = dynamic_cast<CFont*>(pResourceBase);
		if (nullptr != pFont)
		{
			pFont->initialise();
		}
		return true;
	}

	bool CResourceManager::InitializeFbxSdkObjects()
	{
		mpFbxSdkManager = FbxManager::Create();
		if (!mpFbxSdkManager)
		{
			FBXSDK_printf("Error: Unable to create FBX Manager!\n");
			return false;
		}

		//Create an IOSettings object. This object holds all import/export settings.
		FbxIOSettings* ios = FbxIOSettings::Create(mpFbxSdkManager, IOSROOT);
		mpFbxSdkManager->SetIOSettings(ios);

		//Load plugins from the executable directory (optional)
		FbxString lPath = FbxGetApplicationDirectory();
		mpFbxSdkManager->LoadPluginsDirectory(lPath.Buffer());

		return true;
	}

	void CResourceManager::DestroyFbxSdkObjects()
	{
		if (mpFbxSdkManager != nullptr)
		{
			mpFbxSdkManager->Destroy();
		}
	}

	void CResourceManager::addVSConstBuffer(CShader* pShader)
	{
		lua_State* pLua = CScriptPlugin::getSinglePtr()->getLuaState();
		int t = lua_gettop(pLua);
		lua_getglobal(pLua, "material");
		lua_pushstring(pLua, "vsConstBuffer");
		lua_gettable(pLua, -2);


		if (!lua_isnil(pLua, -1))
		{
			if (lua_istable(pLua, -1))
			{	
				INT32 nCount = calculateTableLength(pLua);
				for (INT32 i = 1; i < nCount; ++i)
				{
					lua_pushnumber(pLua, i);
					lua_gettable(pLua, -2);
					if (!lua_isnil(pLua, -1))
					{

					}
					else
					{
						break;
					}
				}
			}
		}
		lua_settop(pLua, t);
	}

	void CResourceManager::addPSConstBuffer(CShader* pShader)
	{
		lua_State* pLua = CScriptPlugin::getSinglePtr()->getLuaState();
		int t = lua_gettop(pLua);
		lua_getglobal(pLua, "material");
		lua_pushstring(pLua, "psConstBuffer");
		lua_gettable(pLua, -2);
		if (!lua_isnil(pLua, -1))
		{
			if (lua_istable(pLua, -1))
			{
				INT32 nCount = calculateTableLength(pLua);
				for (INT32 i = 1; i < nCount; ++i)
				{
					lua_pushnumber(pLua, i);
					lua_gettable(pLua, -2);
					if (!lua_isnil(pLua, -1))
					{
						lua_pushstring(pLua, "name");
						lua_gettable(pLua, -2);
						std::string name = lua_tostring(pLua, -1);
						lua_pop(pLua, 1);

						lua_pushstring(pLua, "size");
						lua_gettable(pLua, -2);
						INT32 size = static_cast<INT32>(lua_tonumber(pLua, -1));
						lua_pop(pLua, 1);

						lua_pushstring(pLua, "registerIndex");
						lua_gettable(pLua, -2);
						INT32 registerIndex = static_cast<INT32>(lua_tonumber(pLua, -1));
						lua_pop(pLua, 2);
						pShader->addPSParam(name, size, registerIndex);
					}
				}
			}
		}
		lua_settop(pLua, t);
	}



	UINT32 readFbxVertice(FbxMesh* pFbxMesh, INT32 index, CPoint3F* pVertex)
	{
		FbxVector4& v = pFbxMesh->GetControlPointAt(index);
		pVertex->x = (float)v[0];
		pVertex->y = (float)v[1];
		pVertex->z = (float)v[2];
		return sizeof(CPoint3F);
	}

	UINT32 readFbxNormal(FbxMesh* pFbxMesh, INT32 index, INT32 vetexCount, CPoint3F* pNormal)
	{
		UINT32 offset = sizeof(CPoint3F);
		if (pFbxMesh->GetElementNormalCount() < 1)
		{
			return offset;
		}
		FbxGeometryElementNormal* leNormal = pFbxMesh->GetElementNormal(0);
		switch (leNormal->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (leNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				FbxVector4& normalValue = leNormal->GetDirectArray().GetAt(index);
				pNormal->x = (float)normalValue[0];
				pNormal->y = (float)normalValue[1];
				pNormal->z = (float)normalValue[2];
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int id = leNormal->GetIndexArray().GetAt(index);
				FbxVector4& normalValue = leNormal->GetDirectArray().GetAt(id);
				pNormal->x = (float)normalValue[0];
				pNormal->y = (float)normalValue[1];
				pNormal->z = (float)normalValue[2];
			}
			break;

			default:
				break;
			}
		}
		break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (leNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				FbxVector4& normalValue = leNormal->GetDirectArray().GetAt(vetexCount);
				pNormal->x = (float)normalValue[0];
				pNormal->y = (float)normalValue[1];
				pNormal->z = (float)normalValue[2];
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int id = leNormal->GetIndexArray().GetAt(vetexCount);
				FbxVector4& normalValue = leNormal->GetDirectArray().GetAt(id);
				pNormal->x = (float)normalValue[0];
				pNormal->y = (float)normalValue[1];
				pNormal->z = (float)normalValue[2];
			}
			break;

			default:
				break;
			}
		}
		break;
		}
		return offset;
	}

	UINT32	readFbxTangent(FbxMesh* pFbxMesh, INT32 index, CPoint3F* tangent)
	{
		return sizeof(CPoint3F);
	}

	UINT32 readFbxBinormal(FbxMesh* pFbxMesh, INT32 index, CPoint3F* binormal)
	{
		return sizeof(CPoint3F);
	}

	UINT32 readFbxUV(FbxMesh* pFbxMesh, INT32 index, INT32 textureUVIndex, CPoint2F* uv)
	{
		UINT32 offset = sizeof(CPoint2F);
		if (pFbxMesh->GetElementUVCount() <= 0)
		{
			return offset;
		}

		FbxGeometryElementUV* pVertexUV = pFbxMesh->GetElementUV(0);

		switch (pVertexUV->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (pVertexUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				FbxVector2& uvValue = pVertexUV->GetDirectArray().GetAt(index);
				uv->x = (float)uvValue[0];
				uv->y = (float)uvValue[1];
			}
			break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int id = pVertexUV->GetIndexArray().GetAt(index);

				FbxVector2& uvValue = pVertexUV->GetDirectArray().GetAt(id);
				uv->x = (float)uvValue[0];
				uv->y = (float)uvValue[1];
			}
			break;
			default:
				break;
			}
		}
		break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (pVertexUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			case FbxGeometryElement::eIndexToDirect:
			{
				FbxVector2& uvValue = pVertexUV->GetDirectArray().GetAt(textureUVIndex);
				uv->x = (float)uvValue[0];
				uv->y = (float)uvValue[1];
			}
			break;
			default:
				break;
			}
		}
		break;
		}

		return offset;
	}
}

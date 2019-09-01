#include "pch.h"
#include "Unit.h"
#include "UnitDerive.h"
#include "RendererPlugin.h"
#include "ResourceManager.h"
#include "Vector.h"
namespace mini
{
	CUnit::CUnit()
//		:mUnitFlag(unKnow)
	{
		mstartItem = mProperty.end();
	}

	CUnit::~CUnit()
	{

	}

	void CUnit::onInitialise()
	{

	}

	void CUnit::update()
	{

	}

	void CUnit::onUninitialise()
	{

	}

	void CUnit::setUnitFile(const std::string& strFileName)
	{
		mstrFileName = strFileName;
	}

	Property& CUnit::getProperty(UINT32 uIndex)
	{
		if (uIndex < mProperty.size())
		{
			return *(mProperty[uIndex]);
		}
		else
		{
			return (*(Property*)(0));
		}
	}

	Property& CUnit::getProperty(const std::string& strName)
	{
		auto item = mstrProperty.find(strName);
		if (item != mstrProperty.end())
		{
			return item->second;
		}
		else
		{
			return (*(Property*)(0));
		};
	}


	Property* CUnit::getPropertyFirst()
	{
		if (mProperty.size() > 0)
		{
			mstartItem = mProperty.begin();
			Property* pTempProperty = (*mstartItem);
			++mstartItem;
			return pTempProperty;
		}
		else
		{
			return nullptr;
		}
	}

	Property* CUnit::getPropertyNext()
	{
		if (mProperty.size() > 0)
		{
			if (mstartItem != mProperty.end())
			{
				Property* pTempProperty = (*mstartItem);
				++mstartItem;
				return pTempProperty;
			}
		}
		return nullptr;
	}

	Property* CUnit::RegisterProperty(std::string strName, void* pData, PropertyDataType datatype, UINT32 flag, std::function<void()> func)
	{
		if (mstrProperty.find(strName) == mstrProperty.end())
		{
			UINT32 size = mProperty.size();
			mstrProperty[strName] = Property(size, strName, pData, datatype, flag, func);
			mProperty.push_back(&mstrProperty[strName]);
		}
		else
		{
			UINT32 nIndex = mstrProperty[strName].nIndex;
			mstrProperty[strName] = Property(nIndex, strName, pData, datatype, flag, func);
		}
		return &mstrProperty[strName];
	}



	void CUnit::exportPropertyToEditor()
	{

	}

	void CUnit::updateUnitProperty(Property* pProperty)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	CPostRendererUnit::CPostRendererUnit()
		: mpPostRender(nullptr)
	{
//		mUnitFlag = (UnitEnum::render | UnitEnum::client | UnitEnum::postrender);
	}

	CPostRendererUnit::~CPostRendererUnit()
	{
		SafeRelease(mpPostRender);
	}


	//////////////////////////////////////////////////////////////////////////
	CWndUnit::CWndUnit()
		: mpWnd(nullptr)
	{
//		mUnitFlag = (UnitEnum::render | UnitEnum::client | UnitEnum::wnd);
	}

	CWndUnit::~CWndUnit()
	{
		SafeRelease(mpWnd);
	}

	void CWndUnit::onInitialise()
	{
		SafeRelease(mpWnd);
		CResourceBase* pResourceBase = CResourceManager::getSinglePtr()->findResource(mstrFileName);
		if (pResourceBase == nullptr)
		{
			mpWnd = dynamic_cast<ui::CWnd*>(CResourceManager::getSinglePtr()->createResource(mstrFileName, Wnd));
		}
		else
		{
			mpWnd = dynamic_cast<ui::CWnd*>(pResourceBase);
			mpWnd->AddRef();
		}
		Property* pProperty = RegisterProperty("Wnd", mpWnd, _ADDRESS, Logic, nullptr);
	}

	void CWndUnit::onUninitialise()
	{

	}

	void CWndUnit::update()
	{
		if (mbDirty && mpWnd)
		{

		}
	}

	void CWndUnit::updateUnitProperty(Property* pProperty)
	{

	}

	ui::CWnd* CWndUnit::getWnd()
	{
		return mpWnd;
	}


	//////////////////////////////////////////////////////////////////////////
	CMeshUnit::CMeshUnit()
		: mpMesh(nullptr)
		, mpMaterialUnit(nullptr)
	{
	//	mUnitFlag = (UnitEnum::render | UnitEnum::client | UnitEnum::mesh);
		D3DXMatrixIdentity(&mWorldMatrix);
		D3DXMatrixIdentity(&mScaleMatrix);
		D3DXMatrixIdentity(&mPositionMatrix);
		D3DXMatrixIdentity(&mRotationMatrix[0]);
		D3DXMatrixIdentity(&mRotationMatrix[1]);
		D3DXMatrixIdentity(&mRotationMatrix[2]);

		mPosition = CVector(0, 0, 0);
		mScale = CVector(1, 1, 1);
		mRotation = CVector(0, 0, 0);
		mLocalPosition = CVector(0, 0, 0);
		mLocalScale = CVector(1, 1, 1);
		mLocalRotation = CVector(0, 0, 0);
		mParentPosition = CVector(0, 0, 0);
		mParentScale = CVector(0, 0, 0);
		mParentRotation = CVector(0, 0, 0);
		setPositon(mLocalPosition);
		setScale(mScale);
		setRotation(mRotation);
		mbDirty = true;
	}

	CMeshUnit::~CMeshUnit()
	{
		onUninitialise();
	}

	void CMeshUnit::onInitialise()
	{
		SafeRelease(mpMesh);
		CResourceBase* pResourceBase = CResourceManager::getSinglePtr()->findResource(mstrFileName);
		if (pResourceBase == nullptr)
		{
			mpMesh = dynamic_cast<CMesh*>(CResourceManager::getSinglePtr()->createResource(mstrFileName, Mesh));
		}
		else
		{
			mpMesh = dynamic_cast<CMesh*>(pResourceBase);
			mpMesh->AddRef();
		}
		mpMesh->initialise();
		exportPropertyToEditor();
	}


	void CMeshUnit::exportPropertyToEditor()
	{
		RegisterProperty("Mesh", mpMesh, _ADDRESS, Logic | GroundName, nullptr);
		RegisterProperty("Position", mLocalPosition, _FLOAT3, Editor, std::bind(&CMeshUnit::_setPosition, this));
		RegisterProperty("Scale", mLocalScale, _FLOAT3, Editor, std::bind(&CMeshUnit::_setScale, this));
		RegisterProperty("Rotation", mLocalRotation, _FLOAT3, Editor, std::bind(&CMeshUnit::_setRotation, this));
	}

	void CMeshUnit::onUninitialise()
	{
		SafeRelease(mpMesh);
		SafeRelease(mpMaterialUnit);
	}

	void CMeshUnit::updateUnitProperty(Property* pProperty)
	{
		for (auto item : mProperty)
		{
			if (&(*item) == pProperty)
			{
				mbDirty = true;
				pProperty->callback();
			}
		}
		if (mpMaterialUnit != nullptr)
		{
			mpMaterialUnit->updateUnitProperty(pProperty);
		}
	}



	void CMeshUnit::setPositon(CVector& pt)
	{
		mParentPosition = pt;
		_setPosition();
	}

	void CMeshUnit::setRotation(CVector& rotation)
	{
		mParentRotation = rotation;
		_setRotation();
	}

	void CMeshUnit::setRotationX(float angle)
	{
		mParentRotation.x = angle;
		_setRotationX();
	}

	void CMeshUnit::setRotationY(float angle)
	{
		mParentRotation.y = angle;
		_setRotationY();
	}

	void CMeshUnit::setRotationZ(float angle)
	{
		mParentRotation.z = angle;
		_setRotationZ();
	}

	void CMeshUnit::setScale(CVector& scale)
	{
		mParentScale = scale;
		_setScale();
	}

	void CMeshUnit::setLocalPositon(CVector& pt)
	{
		mLocalPosition = pt;
		_setPosition();
	}

	void CMeshUnit::setLocalRotation(CVector& rotation)
	{
		mLocalRotation = rotation;
		_setRotation();
	}

	void CMeshUnit::setLocalRotationX(float angle)
	{
		mLocalRotation.x = angle;
		_setRotationX();
	}

	void CMeshUnit::setLocalRotationY(float angle)
	{
		mLocalRotation.y = angle;
		_setRotationY();
	}

	void CMeshUnit::setLocalRotationZ(float angle)
	{
		mLocalRotation.z = angle;
		_setRotationZ();
	}

	void CMeshUnit::setLocalScale(CVector& scale)
	{
		mLocalScale = scale;
		_setScale();
	}


	void CMeshUnit::update()
	{
		if (mbDirty && mpMesh)
		{
			D3DXMatrixMultiply(&mWorldMatrix, &mRotationMatrix[0], &mRotationMatrix[1]);
			D3DXMatrixMultiply(&mWorldMatrix, &mWorldMatrix, &mRotationMatrix[2]);
			D3DXMatrixMultiply(&mWorldMatrix, &mWorldMatrix, &mScaleMatrix);
			D3DXMatrixMultiply(&mWorldMatrix, &mWorldMatrix, &mPositionMatrix);
			mbDirty = false;
		}
	}

	void CMeshUnit::buildMaterialUnit(CMaterialUnit* pUnit)
	{
		if (pUnit != nullptr && pUnit != mpMaterialUnit)
		{
			SafeRelease(mpMaterialUnit);
			mpMaterialUnit = pUnit;
			mpMaterialUnit->AddRef();
			mpMaterialUnit->exportPropertyToEditor();
			Property* pProperty = mpMaterialUnit->getPropertyFirst();
			while (pProperty != nullptr)
			{
				RegisterProperty(pProperty->strName, pProperty->pMemory, pProperty->dataType, pProperty->nFlag, pProperty->callback);
				pProperty = mpMaterialUnit->getPropertyNext();
			}
		}
	}


	CMaterialUnit* CMeshUnit::getMaterialUnit()
	{
		return mpMaterialUnit;
	}

	CBoundBox& CMeshUnit::getBoundBox()
	{
		return mBoundBox;
	}

	INTERSECTION& CMeshUnit::isPick(const CVector& v, bool isPickBound)
	{
		static INTERSECTION result;
		if (isPickBound)
		{
		}
		else
		{
			CCamera* pCamera = dynamic_cast<CCamera*>(GEngine->getCurrentScene()->getCamera());
			CPoint3F vPickRayDir;
			CPoint3F vPickRayOrig;

			// Get the inverse view matrix
			const CMatrix& matView = pCamera->getViewMatrix();
			const CMatrix& matWorld = mWorldMatrix;
			CMatrix mWorldView = matWorld * matView;
			CMatrix m;
			D3DXMatrixInverse(&m, NULL, &mWorldView);

			vPickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
			vPickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
			vPickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
			vPickRayOrig.x = m._41;
			vPickRayOrig.y = m._42;
			vPickRayOrig.z = m._43;

			float fBary1, fBary2, fDist;
#ifdef WIN32
			VertexFormatVTNTB* pVertices = mpMesh->getVertices();
#else
			VertexFormatVTN* pVertices = mpMesh->getVertices();
#endif
			UINT32* pIndices = mpMesh->getIndices();
			INT32 faceNumber = mpMesh->getFaceNumber();
			INT32 index = 0;
			result.fDist = FLT_MAX;
			bool bcheck = false;
			for (INT32 i = 0; i < faceNumber; ++i)
			{
				index = i * 3;
				if (GEngine->IntersectTriangle(&vPickRayOrig.x,
					&vPickRayDir.x,
					&pVertices[pIndices[index + 0]].position.x,
					&pVertices[pIndices[index + 1]].position.x,
					&pVertices[pIndices[index + 2]].position.x,
					&fDist, &fBary1, &fBary2))
				{
					if (result.fDist > fDist)
					{
						result.fDist = fDist;
						result.fBary1 = fBary1;
						result.fBary2 = fBary2;
					}
				}
			}
		}
		return result;
	}

	void CMeshUnit::render(CDevice* pDevice)
	{
		if (mpMaterialUnit != nullptr)
		{
			update();
			ID3D11DeviceContext* pDeviceContext = pDevice->getDeviceContext();

			// Set vertex buffer stride and offset.
			UINT32 stride = mpMesh->mVertexSize;
			UINT32 offset = 0;

			// Set the vertex buffer to active in the input assembler so it can be rendered.
			pDeviceContext->IASetVertexBuffers(0, 1, &mpMesh->mpVertexBuffer, &stride, &offset);

			// Set the index buffer to active in the input assembler so it can be rendered.
			pDeviceContext->IASetIndexBuffer(mpMesh->mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
			pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			updateMatricesToShader();
			mpMaterialUnit->render(pDevice);
			pDeviceContext->DrawIndexed(mpMesh->mIndexCount, 0, 0);

		}
	}

	void CMeshUnit::updateMatricesToShader()
	{
		CRendererPlugin::getSinglePtr()->setWorldMatrix(&mWorldMatrix);
		auto& matrices = CRendererPlugin::getSinglePtr()->getTransposeMatrices();
		mpMaterialUnit->getShader()->updateVertexConstBuffer(&matrices[0]);
	}

	void CMeshUnit::_setPosition()
	{
		mbDirty = true;
		mPosition = mParentPosition + mLocalPosition;
		mPositionMatrix.m[3][0] = mPosition.x;
		mPositionMatrix.m[3][1] = mPosition.y;
		mPositionMatrix.m[3][2] = mPosition.z;
		mBoundBox.setPosition(mPosition);
	}

	void CMeshUnit::_setRotation()
	{
		mbDirty = true;
		mRotation = mParentRotation + mLocalRotation;
		_setRotationX();
		_setRotationY();
		_setRotationZ();
	}

	void CMeshUnit::_setScale()
	{
		mbDirty = true;
		mScale = mLocalScale + mParentScale;
		mScale.x = mLocalScale.x * mParentScale.x;
		mScale.y = mLocalScale.y * mParentScale.y;
		mScale.z = mLocalScale.z * mParentScale.z;
		mScaleMatrix.m[0][0] = mScale.x;
		mScaleMatrix.m[1][1] = mScale.y;
		mScaleMatrix.m[2][2] = mScale.z;
		mBoundBox.setScale(mScale);
	}


	void CMeshUnit::_setRotationX()
	{
		mbDirty = true;
		mRotation.x = mParentRotation.x + mLocalRotation.x;
		D3DXMatrixRotationX(&mRotationMatrix[0], mRotation.x);
	}

	void CMeshUnit::_setRotationY()
	{
		mbDirty = true;
		mRotation.y = mParentRotation.y + mLocalRotation.y;
		D3DXMatrixRotationY(&mRotationMatrix[1], mRotation.y);
	}

	void CMeshUnit::_setRotationZ()
	{
		mbDirty = true;
		mRotation.z = mParentRotation.z + mLocalRotation.z;
		D3DXMatrixRotationZ(&mRotationMatrix[2], mRotation.z);
	}

	void CMeshUnit::bindMesh(CMesh* pMesh)
	{
		if (mpMesh != pMesh)
		{
			SafeRelease(mpMesh);
			mpMesh = pMesh;
			mpMesh->AddRef();
			if (pMesh->mVerticesSkin.size() > 0 )
			{
				mBoundBox.calculation(&pMesh->mVerticesSkin[0], pMesh->mVertexCount);
			}
			else
			{
				mBoundBox.calculation(&pMesh->mVertices[0], pMesh->mVertexCount);
			}
		}
	}

	void CMeshUnit::bindMaterial(CMaterialUnit* pMaterialUnit)
	{
		if (pMaterialUnit != mpMaterialUnit)
		{
			SafeRelease(mpMaterialUnit);
			mpMaterialUnit = pMaterialUnit;
			mpMaterialUnit->AddRef();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CMaterialUnit::CMaterialUnit()
		: mpMaterial(nullptr)
	{
		mstrFileName = "";
	//	mUnitFlag = (UnitEnum::render | UnitEnum::client | UnitEnum::material);
	}

	CMaterialUnit::~CMaterialUnit()
	{
		onUninitialise();
	}

	void CMaterialUnit::onInitialise()
	{
		SafeRelease(mpMaterial);
		CResourceBase* pResourceBase = CResourceManager::getSinglePtr()->findResource(mstrFileName);
		if (pResourceBase == nullptr)
		{
			mpMaterial = dynamic_cast<CMaterial*>(CResourceManager::getSinglePtr()->createResource(mstrFileName, Material));
		}
		else
		{
			pResourceBase->AddRef();
			mpMaterial = dynamic_cast<CMaterial*>(pResourceBase);
		}
		exportPropertyToEditor();
	}

	void CMaterialUnit::exportPropertyToEditor()
	{
		RegisterProperty("Material", mpMaterial, _ADDRESS, Logic | GroundName, nullptr);
		RegisterProperty("DiffuseColor", &mpMaterial->mConstBuffer.mDiffuseColor[0], _COLOR32, Editor, std::bind(&CMaterialUnit::setDiffuseColor, this));
		RegisterProperty("AmbientColor", &mpMaterial->mConstBuffer.mAmbientColor[0], _COLOR32, Editor, std::bind(&CMaterialUnit::setAmbientColor, this));
		RegisterProperty("SpecularColor", &mpMaterial->mConstBuffer.mSpecularColor[0], _COLOR32, Editor, std::bind(&CMaterialUnit::setSpecularColor, this));
		RegisterProperty("SpecularPower", &mpMaterial->mConstBuffer.mSpecularPower, _FLOAT, Editor, std::bind(&CMaterialUnit::setSpecularPower, this));
		//RegisterProperty("Textures", &mpMaterial->mTextures, _TEXTURE, Editor);
	}

	void CMaterialUnit::setDiffuseColor()
	{
		if (nullptr != mpMaterial)
		{
			mpMaterial->mpShader->updatePixelConstBuffer("DiffuseColor", mpMaterial->mConstBuffer.mDiffuseColor);
		}
	}

	void CMaterialUnit::setAmbientColor()
	{
		if (nullptr != mpMaterial)
		{
			mpMaterial->mpShader->updatePixelConstBuffer("DiffuseColor", mpMaterial->mConstBuffer.mAmbientColor);
		}
	}

	void CMaterialUnit::setSpecularColor()
	{
		if (nullptr != mpMaterial)
		{

		}
	}

	void CMaterialUnit::setSpecularPower()
	{
		if (nullptr != mpMaterial)
		{

		}
	}

	void CMaterialUnit::update()
	{
		if (mbDirty && mpMaterial)
		{

		}
	}

	void CMaterialUnit::updateUnitProperty(Property* pProperty)
	{

	}


	void CMaterialUnit::onUninitialise()
	{
		SafeRelease(mpMaterial);
	}

	UINT32 CMaterialUnit::getShaderId()
	{
		if (mpMaterial != nullptr && mpMaterial->mpShader != nullptr)
		{
			return mpMaterial->mpShader->getResourceID();
		}
		else
		{
			return -1;
		}
	}

	CShader* CMaterialUnit::getShader()
	{
		if (mpMaterial != nullptr)
		{
			return mpMaterial->mpShader;
		}
		return nullptr;
	}

	void CMaterialUnit::render(CDevice* pDevice)
	{
		if (mpMaterial != nullptr)
		{
			mpMaterial->render(pDevice);
		}
	}

	void CMaterialUnit::updateVSConstBuffer(UINT32 uStartSlot, void* pData)
	{
		if (nullptr != mpMaterial)
		{
			if (nullptr != mpMaterial->mpShader)
			{
				mpMaterial->mpShader->updateVertexConstBuffer(pData, uStartSlot);
			}
		}
	}

	void CMaterialUnit::updatePSConstBuffer(const std::string& name, void* pData)
	{
		if (nullptr != mpMaterial)
		{
			if (nullptr != mpMaterial->mpShader)
			{
				mpMaterial->mpShader->updatePixelConstBuffer(name, pData);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CFontUnit::CFontUnit(CFont* pFont)
	{
		mpFont = pFont;
		mpFont->AddRef();
	}

	CFontUnit::~CFontUnit()
	{
		SafeRelease(mpFont);
	}

	const CTexture* const CFontUnit::getFontTexture() const
	{
		return nullptr != mpFont ? mpFont->getFontTexture() : nullptr;
	}

	void CFontUnit::setFontShowMode(ShoweMode mode)
	{
		mFontShowMode = mode;
	}


	void CFontUnit::setFontSize(CPoint2F size)
	{
		mFontSize = size;
	}

	void CFontUnit::setFontColor(CPoint4F color)
	{
		mFontColor = color;
	}

	void CFontUnit::setDrawText(const std::string& text, float x, float y, float width, float height)
	{
		INT32 l = text.length();
		if (l > 0)
		{
			auto& config = GEngine->getDevice()->getConfig();
			float frameHalfWidth = (float)config.screenWidth / 2;
			float frameHalfHeight = (float)config.screenHeight / 2;
			auto&& pt = calculatePositionByShowMode(text, x, y, width, height);
			x = pt.x - frameHalfWidth;
			y = frameHalfHeight - pt.y;
			int index = 0;
			int textCount = 0;
			mFontContentBuffer.resize(l*6);
			const char* pCharBuffer = text.c_str();
			INT32 letter = 0;
			for (UINT32 i = 0; i < text.size(); ++i)
			{
				letter = ((INT32)pCharBuffer[i]) - 32;

				// If the letter is a space then just move over three pixels.
				if (letter != 0)
				{
					index = i * 6;
					++textCount;
					// First triangle in quad.
					mFontContentBuffer[index + 0].position = CPoint3F(x, y, 0.0f);  // Top left.
					mFontContentBuffer[index + 0].uv = CPoint2F(mpFont->mFontCells[letter].left, 0.0f);
					mFontContentBuffer[index + 0].color = mFontColor;

					mFontContentBuffer[index + 1].position = CPoint3F((x + mpFont->mFontCells[letter].size), (y - mFontSize.y), 0.0f);  // Bottom right.
					mFontContentBuffer[index + 1].uv = CPoint2F(mpFont->mFontCells[letter].right, 1.0f);
					mFontContentBuffer[index + 1].color = mFontColor;

					mFontContentBuffer[index + 2].position = CPoint3F(x, (y - mFontSize.y), 0.0f);  // Bottom left.
					mFontContentBuffer[index + 2].uv = CPoint2F(mpFont->mFontCells[letter].left, 1.0f);
					mFontContentBuffer[index + 2].color = mFontColor;

					// Second triangle in quad.
					mFontContentBuffer[index + 3].position = CPoint3F(x, y, 0.0f);  // Top left.
					mFontContentBuffer[index + 3].uv = CPoint2F(mpFont->mFontCells[letter].left, 0.0f);
					mFontContentBuffer[index + 3].color = mFontColor;

					mFontContentBuffer[index + 4].position = CPoint3F(x + mpFont->mFontCells[letter].size, y, 0.0f);  // Top right.
					mFontContentBuffer[index + 4].uv = CPoint2F(mpFont->mFontCells[letter].right, 0.0f);
					mFontContentBuffer[index + 4].color = mFontColor;

					mFontContentBuffer[index + 5].position = CPoint3F((x + mpFont->mFontCells[letter].size), (y - mFontSize.y), 0.0f);  // Bottom right.
					mFontContentBuffer[index + 5].uv = CPoint2F(mpFont->mFontCells[letter].right, 1.0f);
					mFontContentBuffer[index + 5].color = mFontColor;
					// Update the x location for drawing by the size of the letter and one pixel.
					x += mpFont->mFontCells[letter].size + 2.0f;
				}
				else
				{
					x += 3.0f;
				}
			}
			mFontContentBuffer.resize(textCount * 6);
			mFontContentBuffer.shrink_to_fit();
		}
	}

	ShoweMode CFontUnit::getFontShowMode()
	{
		return mFontShowMode;
	}

	CPoint2F CFontUnit::getFontSize()
	{
		return mFontSize;
	}

	CPoint4F CFontUnit::getFontColor()
	{
		return mFontColor;
	}
	
	const std::vector<VertexFormatVTC>& CFontUnit::getTextBuffer() const 
	{
		return mFontContentBuffer;
	}
	CPoint2F CFontUnit::calculatePositionByShowMode( const std::string& text, float x, float y, float width, float height)
	{
		CPoint2F pt = { x,y };
		INT32 letter = 0;
		const char* pChBuffer = text.c_str();
		float pt_x = 0;
		for (UINT32 i = 0; i < text.size(); ++i)
		{
			letter = ((INT32)pChBuffer[i]) - 32;

			// If the letter is a space then just move over three pixels.
			if (letter != 0)
			{
				pt_x += mpFont->mFontCells[letter].size + 2.0f;
			}
			else
			{
				pt_x += 3.0f;
			}
		}	

		if (mFontShowMode || ShoweMode::Center)
		{
			pt.x = x + (width - pt_x ) / 2;
			pt.y = y + (height - mFontSize.y )/2;
		}
		else
		{
			if (mFontShowMode || ShoweMode::Left)
			{
				pt.x = x;
			}
			if (mFontShowMode || ShoweMode::Top)
			{
				pt.y = y;
			}
			if (mFontShowMode || ShoweMode::Right)
			{
				pt.x = x + width - pt_x;
			}
			if (mFontShowMode || ShoweMode::Bottom)
			{
				pt.y = y + height - mFontSize.y;
			}
			if (mFontShowMode || ShoweMode::H_Center)
			{
				pt.x = x + ( width - pt_x ) / 2 ;
			}
			if (mFontShowMode || ShoweMode::U_Center)
			{
				pt.y = y + ( height - mFontSize.y ) / 2;
			}
		}
		return pt;
	}

	CFont* CFontUnit::getFont()
	{
		return mpFont;
	}
}
#pragma once
#include "Unit.h"
#include "BoundBox.h"

////////////////////////////////////////////////////////////////////////////
#include "PostRender.h"
#include "Wnd.h"
#include "Material.h"
#include "Mesh.h"
#include "Font.h"

namespace mini
{
	//////////////////////////////////////////////////////////////////////////
	class CPostRendererUnit : public CUnit
	{
	public:
		CPostRendererUnit();
		virtual ~CPostRendererUnit();

	private:
		CPostRender*	mpPostRender;
	};


	//////////////////////////////////////////////////////////////////////////

	class CWndUnit : public CUnit
	{
	public:
		CWndUnit();
		virtual ~CWndUnit();
		virtual void onInitialise();
		virtual void onUninitialise();
		virtual void update();
		virtual void updateUnitProperty(Property* pProperty);

		ui::CWnd* getWnd();

	private:
		ui::CWnd*	mpWnd;
	};



	//////////////////////////////////////////////////////////////////////////

	class CMaterialUnit : public CUnit
	{
	public:
		CMaterialUnit();
		virtual ~CMaterialUnit();
		virtual void onInitialise();
		virtual void onUninitialise();
		virtual void update();
		virtual void updateUnitProperty(Property* pProperty);
		virtual void exportPropertyToEditor();

		UINT32		getShaderId();
		CShader*	getShader();
		void		render(CDevice* pDevice);
		void		updateVSConstBuffer(UINT32 uStartSlot, void* pData );
		void		updatePSConstBuffer(const std::string& name, void* pData );

	private:
		void setDiffuseColor();
		void setAmbientColor();
		void setSpecularColor();
		void setSpecularPower();

	private:
		CMaterial* mpMaterial;
	};


	//////////////////////////////////////////////////////////////////////////
	class CMeshUnit : public CUnit
	{
	public:
		CMeshUnit();
		virtual ~CMeshUnit();
		virtual void onInitialise();
		virtual void exportPropertyToEditor();
		virtual void update();
		virtual void onUninitialise();
		virtual void updateUnitProperty(Property* pProperty);

		CMaterialUnit* getMaterialUnit();
		void setPositon(CVector& pt);
		void setRotation(CVector& rotation);
		void setRotationX(float angle);
		void setRotationY(float angle);
		void setRotationZ(float angle);
		void setScale(CVector& scale);
		void setLocalPositon(CVector& pt);
		void setLocalRotation(CVector& rotation);
		void setLocalRotationX(float angle);
		void setLocalRotationY(float angle);
		void setLocalRotationZ(float angle);
		void setLocalScale(CVector& scale);
		void buildMaterialUnit(CMaterialUnit* pUnit);
		void render(CDevice* pDevice);
		INTERSECTION& isPick(const CVector& v, bool isPickBound);
		CBoundBox& getBoundBox();
		void bindMesh(CMesh* pMesh);
		void bindMaterial(CMaterialUnit* pMaterialUnit);

	private:
		void updateMatricesToShader();
		void _setPosition();
		void _setRotation();
		void _setScale();
		void _setRotationX();
		void _setRotationY();
		void _setRotationZ();

	private:
		CVector				mPosition;
		CVector				mScale;
		CVector				mRotation;
		CVector				mLocalPosition;
		CVector				mLocalScale;
		CVector				mLocalRotation;
		CVector				mParentPosition;
		CVector				mParentScale;
		CVector				mParentRotation;

		CMatrix				mScaleMatrix;
		CMatrix				mPositionMatrix;
		CMatrix				mRotationMatrix[3];
		CMatrix				mWorldMatrix;
		CMesh*				mpMesh;
		CMaterialUnit*		mpMaterialUnit;
		CBoundBox			mBoundBox;
	};

	//////////////////////////////////////////////////////////////////////////
	class CFontUnit : public CUnit
	{
	public:
		explicit CFontUnit(CFont* pFont);
		virtual ~CFontUnit();
		const CTexture* const getFontTexture() const;
		void setFontShowMode(ShoweMode mode);
		void setFontSize(CPoint2F size);
		void setFontColor(CPoint4F color);
		void setDrawText(const std::string& text, float x, float y, float width, float height);
		
		ShoweMode	getFontShowMode();
		CPoint2F	getFontSize();
		CPoint4F	getFontColor();
		CFont*		getFont();
		const std::vector<VertexFormatVTC>& getTextBuffer() const;

	private:
		CPoint2F calculatePositionByShowMode(const std::string& text, float x, float y, float width, float height);

	private:
		CPoint2F	mFontSize = CPoint2F(16, 16);
		CPoint4F	mFontColor = CPoint4F(1, 1, 1, 1);
		CFont*		mpFont = nullptr;
		ShoweMode	mFontShowMode = Center;
		std::vector<VertexFormatVTC>	mFontContentBuffer;
	};
}

#pragma once
#include "Matrix.h"
#include "Vector.h"
#include "Actor.h"
#include "Renderer.h"
namespace mini
{
	class CFrustum
	{
		friend class CCamera;
	private:
		CFrustum(CMatrix* pViewMatrix, CMatrix* pProjectMatrix, DeviceConfig* pConfig);
		void update();
		bool checkPoint(const CVector& pt);
		bool checkCube(const CVector& pCenter, float radius);
		bool checkSphere(const CVector&pCenter, float radius);
		bool checkRectangle(const CVector& pCenter, const CVector& radius);

	private:
		D3DXPLANE m_planes[6];
		CMatrix*	m_viewMatrix;
		CMatrix*	m_projectMatrix;
		DeviceConfig*	mpDeviceConfig;
	};

	class CCamera : public CActor
	{
		friend class CRenderer;
	public:
		CCamera();
		virtual ~CCamera();
		void	setEye(CVector& eye);
		void	setLookAt(CVector& lookat);
		void	setRotation(CVector& rotation);
		void	createTo2DProjectMatrix();
		void	createTo3DProjectMatrix();
		void	applyToRenderer(UINT32 uFlag);
		UINT32	getApplyRenderer();
		bool	isApplyRenderer(RendererType type);

		CVector& getEye();
		CVector& getLookAt();
		CMatrix& getViewMatrix();
		CMatrix& getProjectMatrix();

		virtual void updateFrame();
		void left();
		void right();
		void front();
		void back();
		void setSpeed(float speed);
		void processMove(event& msg);

	private:
		bool	isInFrustum(CActor* p);
		void	updateFrustum();
		void	updateCamera();

	private:
		DeviceConfig		mDeviceConfig;
		CFrustum*			mpFrustum;
		CVector				mEyePt;
		CVector				mLookat;
		CVector				mUpPt;
		CVector				mDirection;
		CVector				mRotation;
		CVector				mMoveDirection;
		//	CMatrix*			mpWorldMatrix;
		CMatrix			m_viewMatrix;
		CMatrix			mProjectMatrix;
		CShader*			mpShader;
		UINT32				mApplyRendererFlag;
		float				mMoveSpeed = 0.5f;
		UINT32				mKeyChar = 0;
		std::list<CActor*>	mWaitRenderActors;
		bool				mbDirty = true;
		UINT8				mMoustStatus = 0;
	};
}

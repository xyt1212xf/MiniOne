#include "pch.h"
#include "Camera.h"
#include "RendererPlugin.h"
#include "ResourceManager.h"
#include "Engine.h"

namespace mini
{
	CFrustum::CFrustum(CMatrix* pViewMatrix, CMatrix* pProjectMatrix, DeviceConfig* pConfig)
	{
		m_viewMatrix = pViewMatrix;
		m_projectMatrix = pProjectMatrix;
		mpDeviceConfig = pConfig;
	}

	void CFrustum::update()
	{
		CMatrix matrix = *m_projectMatrix;
		// Calculate the minimum Z distance in the frustum.
		float zMinimum = -matrix._43 / matrix._33;
		float r = mpDeviceConfig->cameraFar / (mpDeviceConfig->cameraFar - zMinimum);
		matrix._33 = r;
		matrix._43 = -r * zMinimum;

		// Create the frustum matrix from the view matrix and updated projection matrix.
		D3DXMatrixMultiply(&matrix, m_viewMatrix, &matrix);

		// Calculate near plane of frustum.
		m_planes[0].a = matrix._14 + matrix._13;
		m_planes[0].b = matrix._24 + matrix._23;
		m_planes[0].c = matrix._34 + matrix._33;
		m_planes[0].d = matrix._44 + matrix._43;
		D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

		// Calculate far plane of frustum.
		m_planes[1].a = matrix._14 - matrix._13;
		m_planes[1].b = matrix._24 - matrix._23;
		m_planes[1].c = matrix._34 - matrix._33;
		m_planes[1].d = matrix._44 - matrix._43;
		D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

		// Calculate left plane of frustum.
		m_planes[2].a = matrix._14 + matrix._11;
		m_planes[2].b = matrix._24 + matrix._21;
		m_planes[2].c = matrix._34 + matrix._31;
		m_planes[2].d = matrix._44 + matrix._41;
		D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

		// Calculate right plane of frustum.
		m_planes[3].a = matrix._14 - matrix._11;
		m_planes[3].b = matrix._24 - matrix._21;
		m_planes[3].c = matrix._34 - matrix._31;
		m_planes[3].d = matrix._44 - matrix._41;
		D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

		// Calculate top plane of frustum.
		m_planes[4].a = matrix._14 - matrix._12;
		m_planes[4].b = matrix._24 - matrix._22;
		m_planes[4].c = matrix._34 - matrix._32;
		m_planes[4].d = matrix._44 - matrix._42;
		D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

		// Calculate bottom plane of frustum.
		m_planes[5].a = matrix._14 + matrix._12;
		m_planes[5].b = matrix._24 + matrix._22;
		m_planes[5].c = matrix._34 + matrix._32;
		m_planes[5].d = matrix._44 + matrix._42;
		D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);
	}

	bool CFrustum::checkPoint(const CVector& pt)
	{
		// Check if the point is inside all six planes of the view frustum.
		for (INT32 i = 0; i < 6; i++)
		{
			if (D3DXPlaneDotCoord(&m_planes[i], &pt) < 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	bool CFrustum::checkCube(const CVector& pCenter, float radius)
	{
		// Check if any one point of the cube is in the view frustum.
		for (INT32 i = 0; i < 6; i++)
		{
			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x - radius), (pCenter.y - radius), (pCenter.z - radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x + radius), (pCenter.y - radius), (pCenter.z - radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x - radius), (pCenter.y + radius), (pCenter.z - radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x + radius), (pCenter.y + radius), (pCenter.z - radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x - radius), (pCenter.y - radius), (pCenter.z + radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x + radius), (pCenter.y - radius), (pCenter.z + radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x - radius), (pCenter.y + radius), (pCenter.z + radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x + radius), (pCenter.y + radius), (pCenter.z + radius))) >= 0.0f)
			{
				continue;
			}

			return false;
		}

		return true;
	}

	bool CFrustum::checkSphere(const CVector&pCenter, float radius)
	{
		// Check if the radius of the sphere is inside the view frustum.
		for (INT32 i = 0; i < 6; i++)
		{
			if (D3DXPlaneDotCoord(&m_planes[i], &pCenter) < -radius)
			{
				return false;
			}
		}

		return true;
	}


	bool CFrustum::checkRectangle(const CVector& pCenter, const CVector& radius)
	{
		// Check if any of the 6 planes of the rectangle are inside the view frustum.
		for (INT32 i = 0; i < 6; i++)
		{
			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x - radius.x), (pCenter.y - radius.y), (pCenter.z - radius.z))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x + radius.x), (pCenter.y - radius.y), (pCenter.z - radius.z))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x - radius.x), (pCenter.y + radius.y), (pCenter.z - radius.z))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x - radius.x), (pCenter.y - radius.y), (pCenter.z + radius.z))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x + radius.x), (pCenter.y + radius.y), (pCenter.z - radius.z))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x + radius.x), (pCenter.y - radius.y), (pCenter.z + radius.z))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x - radius.x), (pCenter.y + radius.y), (pCenter.z + radius.z))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((pCenter.x + radius.x), (pCenter.y + radius.y), (pCenter.z + radius.z))) >= 0.0f)
			{
				continue;
			}

			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	CCamera::CCamera()
	{
		//mpWorldMatrix = nullptr;
		mApplyRendererFlag = 0;
		memset(&mEyePt, 0, sizeof(CVector));
		memset(&mRotation, 0, sizeof(CVector));
		memset(&mRotation, 0, sizeof(CVector));
		mUpPt.x = 0.0f;
		mUpPt.y = 1.0f;
		mUpPt.z = 0.0f;
		mbDirty = true;
		mType = Camera;
		CResourceBase* pResource = CResourceManager::getSinglePtr()->findResource("camera.vs");
		if (pResource == nullptr)
		{
			mpShader = dynamic_cast<CShader*>(CResourceManager::getSinglePtr()->createResource(Shader));
			mpShader->setDevice(CRendererPlugin::getSinglePtr()->getDevice());
			CPoint4F constBuffer = { mEyePt.x, mEyePt.y, mEyePt.z, 1.0 };
			mpShader->createVextexShader("resource/Shader/camera.vs", "vMain", "vs_5_0", nullptr, 0);
			mpShader->createVertexConstBuffer(sizeof(CPoint4F), 5);
		}
		else
		{
			mpShader = dynamic_cast<CShader*>(pResource);
			mpShader->AddRef();
		}
		mpFrustum = new CFrustum(&m_viewMatrix, &mProjectMatrix, &mDeviceConfig);
	}


	CCamera::~CCamera()
	{
		SafeRelease(mpShader);
		SafeDelete(mpFrustum);
	}

	void CCamera::processMove(event& msg)
	{
		static POINT pt;
		static CVector dirLookAt;
		static CVector dirEye;
		static CVector offsetLookAt;
		static CVector offsetEye;
		switch (msg.Messtype)
		{
		case WMKEYDOWN:
		{
			mKeyChar = msg.wParam;
		}
		break;
		case WMKEYUP:
		{
			mKeyChar = 0;
		}
		break;
		case LEFTDOWN:
		{
			mMoustStatus = 1;
			dirEye = mEyePt - mLookat;
			offsetLookAt = mLookat;
			//	D3DXVec3Normalize(&dirEye, &dirEye);
			pt = msg.point;
		}
		break;
		case LEFTUP:
		{
			mMoustStatus = 0;
		}
		break;
		case RIGHTDOWN:
		{
			mMoustStatus = 2;
			dirLookAt = mLookat - mEyePt;
			offsetEye = mEyePt;
		//	D3DXVec3Normalize(&dirLookAt, &dirLookAt);
			pt = msg.point;
		}
		break;
		case RIGHTUP:
		{
			mMoustStatus = 0;
		}
		break;
		case MOUSEMOVE: 
		{
			if (0 != mMoustStatus)
			{
				mbDirty = true;
				if (2 == mMoustStatus)
				{
					CMatrix matrixX;
					D3DXMatrixRotationX(&matrixX, 0.001f * (pt.y - msg.point.y));

					D3DXVECTOR4 r(dirLookAt, 1);
					D3DXVec4Transform(&r, &r, &matrixX);
					mLookat.x = r.x + mEyePt.x;
					mLookat.y = r.y + mEyePt.y;
					mLookat.z = r.z + mEyePt.z;				
				}
				else if(1 == mMoustStatus )
				{
					CMatrix matrixY;
					D3DXMatrixRotationY(&matrixY, 0.001f * (pt.x - msg.point.x));
					D3DXVECTOR4 r(dirEye, 1);
					D3DXVec4Transform(&r, &r, &matrixY);
					mEyePt.x = r.x + offsetLookAt.x;
					mEyePt.y = r.y + offsetLookAt.y;
					mEyePt.z = r.z + offsetLookAt.z;
				}
			}
		}
		break;
		case MOUSEWHEEL:
		{
			CVector result;
			if (-120 == (short)HIWORD(msg.wParam))
			{
				result = mEyePt + mDirection;
			}
			else
			{
				result = mEyePt - mDirection;
			}
			CVector lResult = result - mLookat;
			mEyePt = result;
			setEye(mEyePt);
		}
		break;
		default:
			break;
		}
		updateCamera();
	}

	void CCamera::setEye(CVector& pt)
	{
		mbDirty = true;
		mEyePt = pt;
		mDirection = mEyePt - mLookat;
		D3DXVec3Normalize(&mDirection, &mDirection);
	}

	void CCamera::setLookAt(CVector& pt)
	{
		mLookat = pt;
		mbDirty = true;
		mDirection = mEyePt - mLookat;
		D3DXVec3Normalize(&mDirection, &mDirection);
	}

	void CCamera::setRotation(CVector& rotation)
	{
		mRotation = rotation;
	}

	CMatrix& CCamera::getViewMatrix()
	{
		return m_viewMatrix;
	}

	CMatrix& CCamera::getProjectMatrix()
	{
		return mProjectMatrix;
	}


	void CCamera::createTo2DProjectMatrix()
	{
		// Create an orthographic projection matrix for 2D rendering.
		if (GEngine != nullptr)
		{
			mDeviceConfig = GEngine->getDevice()->getConfig();
			D3DXMatrixOrthoLH(&mProjectMatrix, (float)mDeviceConfig.screenWidth, (float)mDeviceConfig.screenHeight, (float)mDeviceConfig.cameraNear, (float)mDeviceConfig.cameraFar);
		}
	}

	void CCamera::createTo3DProjectMatrix()
	{
		// Create the projection matrix for 3D rendering.
		if (GEngine != nullptr)
		{
			mDeviceConfig = GEngine->getDevice()->getConfig();
			float fieldOfView = (float)D3DX_PI / 4.0f;
			float screenAspect = (float)mDeviceConfig.screenWidth / (float)mDeviceConfig.screenHeight;
			D3DXMatrixPerspectiveFovLH(&mProjectMatrix, fieldOfView, screenAspect, (float)mDeviceConfig.cameraNear, (float)mDeviceConfig.cameraFar);
		}
	}

	void CCamera::applyToRenderer(UINT32 uFlag)
	{
		mApplyRendererFlag = uFlag;
	}

	UINT32 CCamera::getApplyRenderer()
	{
		return mApplyRendererFlag;
	}

	bool CCamera::isApplyRenderer(RendererType type)
	{
		return (mApplyRendererFlag & type) == 0 ? false : true;
	}

	CVector& CCamera::getEye()
	{
		return mEyePt;
	}

	CVector& CCamera::getLookAt()
	{
		return mLookat;
	}

	bool CCamera::isInFrustum(CActor* p)
	{
		if (p->getEntityType() != Gui)
		{
			if (p->getEntityType() != EntityType::Camera)
			{
				CBoundBox& box = p->getBoundBox();
				return mpFrustum->checkRectangle(box.getCenterPt(), box.getRadius());
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	void CCamera::updateFrustum()
	{
		mpFrustum->update();
	}

	void CCamera::updateCamera()
	{
		if (0 != mKeyChar)
		{
			mbDirty = true;
			CVector result = mEyePt - mLookat;
			D3DXVec3Normalize(&result, &result);
			result.y = 0;
			CMatrix	matirx;
			switch (mKeyChar)
			{
			case VK_LEFT:
			{
				D3DXMatrixRotationY(&matirx, (float)D3DX_PI / 180.0f * 270.0f);
				D3DXVECTOR4 r(result, 1);
				D3DXVec4Transform(&r, &r, &matirx);
				mMoveDirection.x = r.x;
				mMoveDirection.y = r.y;
				mMoveDirection.z = r.z;
				mEyePt -= (mMoveDirection * mMoveSpeed);
				mLookat -= (mMoveDirection * mMoveSpeed);
			}
			break;
			case VK_RIGHT:
			{
				D3DXMatrixRotationY(&matirx, (float)D3DX_PI / 180.0f * 90.0f);
				D3DXVECTOR4 r(result, 1);
				D3DXVec4Transform(&r, &r, &matirx);
				mMoveDirection.x = r.x;
				mMoveDirection.y = r.y;
				mMoveDirection.z = r.z;
				mEyePt -= (mMoveDirection * mMoveSpeed);
				mLookat -= (mMoveDirection * mMoveSpeed);
			}
			break;
			case VK_DOWN:
			{
				mMoveDirection.x = result.x;
				mMoveDirection.y = 0;
				mMoveDirection.z = result.z;
				mEyePt += (mMoveDirection * mMoveSpeed);
				mLookat += (mMoveDirection * mMoveSpeed );
			}
			break;
			case VK_UP:
			{
				mMoveDirection.x = result.x;
				mMoveDirection.y = 0;
				mMoveDirection.z = result.z;
				mEyePt -= (mMoveDirection * mMoveSpeed);
				mLookat -= (mMoveDirection * mMoveSpeed);
			}
			break;
			default:
				mbDirty = false;
				break;
			}
			if (mbDirty)
			{
				mpFrustum->update();
			}
		}
	}

	void CCamera::updateFrame()
	{
		if (mbDirty)
		{
			mbDirty = false;
			/*Áé»îÉãÏñ»ú
			float yaw, pitch, roll;
			CMatrix rotationMatrix;

			// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
			pitch = mRotation.x * 0.0174532925f;
			yaw = mRotation.y * 0.0174532925f;
			roll = mRotation.z * 0.0174532925f;

			// Create the rotation matrix from the yaw, pitch, and roll values.
			D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

			// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
			D3DXVec3TransformCoord(&mLookat, &mLookat, &rotationMatrix);
			D3DXVec3TransformCoord(&mUpPt, &mUpPt, &rotationMatrix);
			*/
			// Translate the rotated camera position to the location of the viewer.
			//mLookat = mEyePt + mLookat;

			// Finally create the view matrix from the three updated vectors.
			D3DXMatrixLookAtLH(&m_viewMatrix, &mEyePt, &mLookat, &mUpPt);
		}
		if (mpShader != nullptr)
		{
			CRendererPlugin::getSinglePtr()->setViewMatrix(&m_viewMatrix);
			CRendererPlugin::getSinglePtr()->setProjectMatrix(&mProjectMatrix);

			CPoint4F constBuffer = { mEyePt.x, mEyePt.y, mEyePt.z, 1.0 };
			mpShader->commitToGpu();
			mpShader->updateVertexConstBuffer(&constBuffer, 5);
		}
	}

	void CCamera::left()
	{
		mEyePt.x -= mMoveSpeed;
		mLookat.x -= mMoveSpeed;
	}

	void CCamera::right()
	{
		mEyePt.x += mMoveSpeed;
		mLookat.x += mMoveSpeed;
	}

	void CCamera::front()
	{

	}

	void CCamera::back()
	{

	}

	void CCamera::setSpeed(float speed)
	{
		mMoveSpeed = speed;
	}

}

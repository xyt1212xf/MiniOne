#include "pch.h"
#include "GuiRenderer.h"
#include "Camera.h"
#include "RendererPlugin.h"
#include "UIMain.h"
namespace mini
{
	namespace ui
	{
		const INT32 VertexCount = 1800;
		const INT32 IndexIncreaseValue = 6;
		CGuiRenderer::CGuiRenderer(CDevice* pDevice)
			: CRenderer(pDevice)
			, m_vertexBuffer(nullptr)
			, m_indexBuffer(nullptr)
			, mpShader(nullptr)
#ifdef EDITOR
			, mpShaderLine(nullptr)
			, m_linesBuffer(nullptr)
			, mbDrawLine(false)
#endif // EDITOR
		{
			auto& config = GEngine->getDevice()->getConfig();
			mFrameHalfWidth = config.screenWidth / 2;
			mFrameHalfHeight = config.screenHeight / 2;
			mDrawLinePointCount = 0;
			mFreeStartPoint = 0;
			mOrder = 255;
			mType = RendererType::GuiRenderer;
			mVertexCount = 0;

			initShaderWithGpuBuffer();
			initSamplrState();
			mpCamera = dynamic_cast<CCamera*>(GEngine->createActor(Camera));
			mpCamera->setEye(CVector(0, 0, -10));
			mpCamera->setLookAt(CVector(0, 0, 0));
			mpCamera->createTo2DProjectMatrix();
			mpCamera->applyToRenderer(RendererType::GuiRenderer);
			D3DXMatrixIdentity(&mWorldMatrix);
			mpWnds = &CUIMain::pMySelf->mWindows;
			mRenderGroups.clear();
		}

		CGuiRenderer::~CGuiRenderer()
		{
			SafeRelease(mpCamera);
			SafeRelease(m_vertexBuffer);
			SafeRelease(m_indexBuffer);
			SafeRelease(m_sampleState);
			SafeRelease(mpShader);
#ifdef EDITOR
			SafeRelease(mpShaderLine);
			SafeRelease(m_linesBuffer);
#endif
		}

		void CGuiRenderer::_render(CWnd* pWnd, UINT32 layer)
		{
			if (pWnd->isVisible())
			{
				mRenderWnds[layer].push_back(pWnd);
				if (pWnd->mpChildren.size() > 0)
				{
					for (auto& item : pWnd->mpChildren)
					{
						_render(item, layer);
					}
				}
			}
		}

		void CGuiRenderer::render()
		{
			if (mpCamera != nullptr && 	mpWnds->size() > 0)
			{
				mRenderWnds.clear();
				mpCamera->updateFrame();
				UINT32 nlayer = 0;
				for (auto item : *mpWnds)
				{
					if (item->isVisible())
					{
						_render(item, nlayer);
						++nlayer;
					}

				}
				renderGroup* renderItem = nullptr;
				CTexture* pTexture = nullptr;
				auto addItem = [&](CTexture* pUseTexture, INT32 count)
				{
					pTexture = pUseTexture;
					mRenderGroups.push_back(renderGroup());
					renderItem = &mRenderGroups.back();
					renderItem->pTexture = pTexture;
					renderItem->begin = mFreeStartPoint;	
					renderItem->count = count;

				};
				for (auto& renderWnds : mRenderWnds)
				{
					for (auto& subItem : renderWnds.second)
					{
						//if (subItem->mScissor)
						if( subItem->mScissor)
						{
							addItem(subItem->mpUserTexture, IndexIncreaseValue);
							renderItem->scissorRt.left = (INT32)subItem->mPosition.x;
							renderItem->scissorRt.top = (INT32)subItem->mPosition.y;
							renderItem->scissorRt.right = (INT32)subItem->mPosition.x + (INT32)subItem->mSize.x;
							renderItem->scissorRt.bottom = (INT32)subItem->mPosition.y + (INT32)subItem->mSize.y;
						}
						else if (pTexture != subItem->mpUserTexture)
						{
							addItem(subItem->mpUserTexture, IndexIncreaseValue);
						}
						else
						{
							renderItem->count += IndexIncreaseValue;
						}
						fillRenderBuffer(subItem);
						//ÎÄ×ÖÏÔÊ¾
						if (nullptr != subItem->mpFontUnit)
						{
							const auto& vertexBuffer = subItem->mpFontUnit->getTextBuffer();
							size_t size = vertexBuffer.size();
							if (size > 0)
							{
								VertexFormatVTC* pFontVertices = applyVertexBuffer(size);
								if (nullptr == pFontVertices)
								{
									commitToGpu();
								}
								addItem(const_cast<CTexture*>(subItem->mpFontUnit->getFontTexture()), size);
								pFontVertices = applyVertexBuffer(size);
								memcpy(pFontVertices, &vertexBuffer[0], sizeof(VertexFormatVTC) * size);
							}
						}
					}
				}
				commitToGpu();
			}
		}

		void CGuiRenderer::clearAllUnit()
		{

		}

		void CGuiRenderer::commitToGpu()
		{
			ID3D11DeviceContext* pDeviceContext = mpDevice->getDeviceContext();
			mpDevice->setDepth(false);
			mpDevice->turnOnAlphaBlending();
			if (mFreeStartPoint > 0 && mpShader != nullptr)
			{
				drawBuffer(pDeviceContext);
			}
			mFreeStartPoint = 0;
#ifdef EDITOR
			if (mbEditor)
			{
				if (mDrawLinePointCount > 0 && mpShaderLine != nullptr)
				{
					drawLineBuffer(pDeviceContext);
				}
			}
#endif
			mpDevice->setDepth(true);
			mpDevice->turnOffAlphaBlending();
		}

		void CGuiRenderer::drawBuffer(ID3D11DeviceContext* pDeviceContext)
		{
			mpShader->commitToGpu();
			// Set vertex buffer stride and offset.
			static UINT32 stride = sizeof(VertexFormatVTC);
			UINT32 offset = 0;

			// Set the vertex buffer to active in the input assembler so it can be rendered.
			pDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

			// Set the index buffer to active in the input assembler so it can be rendered.
			pDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
			pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			CRendererPlugin::getSinglePtr()->setWorldMatrix(&mWorldMatrix);
			auto& matrices = CRendererPlugin::getSinglePtr()->getTransposeMatrices();
			mpShader->updateVertexConstBuffer(&matrices[0]);

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			// Lock the vertex buffer so it can be written to.
			HRESULT result = pDeviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return;
			}

			// Copy the data into the vertex buffer.
			memcpy(mappedResource.pData, &m_Verteices[0], sizeof(VertexFormatVTC) * mFreeStartPoint);

			// Unlock the vertex buffer.
			pDeviceContext->Unmap(m_vertexBuffer, 0);

			if (m_sampleState != nullptr)
			{
				pDeviceContext->PSSetSamplers(0, 1, &m_sampleState);
			}
		
			UINT32 rtIdx = 1;
			RECT rectOld;		
			pDeviceContext->RSGetScissorRects(&rtIdx, &rectOld);
			
			for (auto& item : mRenderGroups)
			{
				if (item.needScissor > -1)
				{
					pDeviceContext->RSSetScissorRects(1, &item.scissorRt);
					item.pTexture->commitToGpu(mpDevice, 0);
					pDeviceContext->DrawIndexed(item.count, 0, item.begin);
					pDeviceContext->RSSetScissorRects(1, &rectOld);
				}
				else
				{
					item.pTexture->commitToGpu(mpDevice, 0);
					pDeviceContext->DrawIndexed(item.count, 0, item.begin);
				}
			}
			pDeviceContext->RSSetScissorRects(1, &rectOld);
			mRenderGroups.clear();
		}

		void CGuiRenderer::drawLineBuffer(ID3D11DeviceContext* pDeviceContext)
		{
			mpShaderLine->commitToGpu();
			// Set vertex buffer stride and offset.
			UINT32 stride = sizeof(VertexFormatVC);
			UINT32 offset = 0;

			// Set the lines buffer to active in the input assembler so it can be rendered.
			pDeviceContext->IASetVertexBuffers(0, 1, &m_linesBuffer, &stride, &offset);

			// Set the type of primitive that should be rendered from this lines buffer, in this case lines.
			pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

			CRendererPlugin::getSinglePtr()->setWorldMatrix(&mWorldMatrix);
			auto& matrices = CRendererPlugin::getSinglePtr()->getTransposeMatrices();
			mpShaderLine->updateVertexConstBuffer(&matrices[0]);

			pDeviceContext->Unmap(m_linesBuffer, 0);

			pDeviceContext->Draw(mDrawLinePointCount, 0);
		}

		void CGuiRenderer::fillRenderBuffer(CWnd* pWnd)
		{
			int freeStartPoint = mFreeStartPoint;
			if (pWnd != nullptr)
			{
				GuiStyle* pGuiStyle = pWnd->getGuiStyle();
				//note: when the buffer is not enough, we must render the buffer and continue pushing vertex data to the buffer
				VertexFormatVTC* pVertices = applyVertexBuffer(IndexIncreaseValue);
				if (nullptr != pVertices && nullptr != pGuiStyle)
				{
					freeStartPoint += IndexIncreaseValue;

					float left = pWnd->getX() - mFrameHalfWidth;

					// Calculate the screen coordinates of the right side of the bitmap.
					float right = left + pWnd->getWidth();

					// Calculate the screen coordinates of the top of the bitmap.
					float top = mFrameHalfHeight - pWnd->getY();

					// Calculate the screen coordinates of the bottom of the bitmap.
					float bottom = top - pWnd->getHeight();
					int status = pWnd->mState;
					CRectF& uv = pGuiStyle->mUVlist[status];
					// Load the vertex array with data.
					// First triangle.
					pVertices[0].position = CPoint3F(left, top, 0.0f);  // Top left.
					pVertices[0].uv = CPoint2F(uv.left, uv.top);
					pVertices[0].color = CPoint4F(1, 1, 1, 1);

					pVertices[1].position = CPoint3F(right, bottom, 0.0f);  // Bottom right.
					pVertices[1].uv = CPoint2F(uv.right, uv.bottom);
					pVertices[1].color = CPoint4F(1, 1, 1, 1);

					pVertices[2].position = CPoint3F(left, bottom, 0.0f);  // Bottom left.
					pVertices[2].uv = CPoint2F(uv.left, uv.bottom);
					pVertices[2].color = CPoint4F(1, 1, 1, 1);

					// Second triangle.
					pVertices[3].position = CPoint3F(left, top, 0.0f);  // Top left.
					pVertices[3].uv = CPoint2F(uv.left, uv.top);
					pVertices[3].color = CPoint4F(1, 1, 1, 1);

					pVertices[4].position = CPoint3F(right, top, 0.0f);  // Top right.
					pVertices[4].uv = CPoint2F(uv.right, uv.top);
					pVertices[4].color = CPoint4F(1, 1, 1, 1);

					pVertices[5].position = CPoint3F(right, bottom, 0.0f);  // Bottom right.
					pVertices[5].uv = CPoint2F(uv.right, uv.bottom);
					pVertices[5].color = CPoint4F(1, 1, 1, 1);
				}
				else
				{
					commitToGpu();
					fillRenderBuffer(pWnd);
				}
			}
		}

		bool CGuiRenderer::initShaderWithGpuBuffer()
		{
			mpShader = dynamic_cast<CShader*>(CResourceManager::getSinglePtr()->createResource(Shader));
			mpShader->setDevice(mpDevice);

			mpShaderLine = dynamic_cast<CShader*>(CResourceManager::getSinglePtr()->createResource(Shader));
			mpShaderLine->setDevice(mpDevice);
			std::vector<D3D11_INPUT_ELEMENT_DESC> input_element;
			input_element.resize(3);
			input_element[0].SemanticName = "POSITION";
			input_element[0].SemanticIndex = 0;
			input_element[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			input_element[0].InputSlot = 0;
			input_element[0].AlignedByteOffset = 0;
			input_element[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			input_element[0].InstanceDataStepRate = 0;

			input_element[1].SemanticName = "TEXCOORD";
			input_element[1].SemanticIndex = 0;
			input_element[1].Format = DXGI_FORMAT_R32G32_FLOAT;
			input_element[1].InputSlot = 0;
			input_element[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			input_element[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			input_element[1].InstanceDataStepRate = 0;

			input_element[2].SemanticName = "COLOR";
			input_element[2].SemanticIndex = 0;
			input_element[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			input_element[2].InputSlot = 0;
			input_element[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			input_element[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			input_element[2].InstanceDataStepRate = 0;

#if 0
			CResourceBase* pResource = CResourceManager::getSinglePtr()->createResource("shaderRenderGui", Shader);
			if (pResource != nullptr)
			{
				mpShader = dynamic_cast<CShader*>(pResource);
				if (mpShader == nullptr)
				{
					return false;
				}
			}
#else
			if (!(mpShader->createVextexShader("resource/Shader/guiRender.vs", "guiVertexShader", "vs_5_0", &input_element[0], input_element.size()) &&
				mpShader->createPixelShader("resource/Shader/guiRender.ps", "guiPixelShader", "ps_5_0")))
			{
				return false;
			}
#endif
			auto matrices = CRendererPlugin::getSinglePtr()->getTransposeMatrices();
			mpShader->createVertexConstBuffer(matrices.size() * sizeof(CMatrix), 0);
			m_Verteices.resize(VertexCount);
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;

			// Set up the description of the static vertex buffer.
			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			vertexBufferDesc.ByteWidth = sizeof(VertexFormatVTC) * VertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the vertex data.
			vertexData.pSysMem = &m_Verteices[0];
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			HRESULT result = mpDevice->getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
			if (FAILED(result))
			{
				return false;
			}


			std::vector<UINT32> indices;
			indices.resize(VertexCount);
			for (int i = 0; i < VertexCount; ++i)
			{
				indices[i] = i;
			}
			// Set up the description of the static index buffer.
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(UINT32) * indices.size();
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the index data.
			indexData.pSysMem = &indices[0];
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			// Create the index buffer.
			result = mpDevice->getDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
			if (FAILED(result))
			{
				return false;
			}

#ifdef EDITOR
			mDrawLinePointCount = 100 * 20;
			m_lines.resize(mDrawLinePointCount);

			for (int i = 0; i < 100; ++i)
			{
				m_lines[i * 4 + 0].position = CPoint3F((float)i * 100 - 5000, 10000, 1.0f);  // Top left.
				m_lines[i * 4 + 0].color = CPoint4F(0.3f, 0.3f, 0.3f, 1.0f);

				m_lines[i * 4 + 1].position = CPoint3F((float)i * 100 - 5000, -10000, 1.0f);  // Bottom right.
				m_lines[i * 4 + 1].color = CPoint4F(0.3f, 0.3f, 0.3f, 1.0f);

				m_lines[i * 4 + 2].position = CPoint3F(-10000, (float)i * 100 - 5000, 1.0f);  // Top left.
				m_lines[i * 4 + 2].color = CPoint4F(0.3f, 0.3f, 0.3f, 1.0f);

				m_lines[i * 4 + 3].position = CPoint3F(10000, (float)i * 100 - 5000, 1.0f);  // Bottom right.
				m_lines[i * 4 + 3].color = CPoint4F(0.3f, 0.3f, 0.3f, 1.0f);


				m_lines[i * 4 + 400].position = CPoint3F((float)i * 100 - 5000 + 20, 10000, 1.0f);  // Top left.
				m_lines[i * 4 + 400].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);

				m_lines[i * 4 + 401].position = CPoint3F((float)i * 100 - 5000 + 20, -10000, 1.0f);  // Bottom right.
				m_lines[i * 4 + 401].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);


				m_lines[i * 4 + 402].position = CPoint3F(-10000, (float)i * 100 - 5000 + 20, 1.0f);  // Top left.
				m_lines[i * 4 + 402].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);

				m_lines[i * 4 + 403].position = CPoint3F(10000, (float)i * 100 - 5000 + 20, 1.0f);  // Bottom right.
				m_lines[i * 4 + 403].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);


				m_lines[i * 4 + 800].position = CPoint3F((float)i * 100 - 5000 + 40, 10000, 1.0f);  // Top left.
				m_lines[i * 4 + 800].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);

				m_lines[i * 4 + 801].position = CPoint3F((float)i * 100 - 5000 + 40, -10000, 1.0f);  // Bottom right.
				m_lines[i * 4 + 801].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);

				m_lines[i * 4 + 802].position = CPoint3F(-10000, (float)i * 100 - 5000 + 40, 1.0f);  // Top left.
				m_lines[i * 4 + 802].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);

				m_lines[i * 4 + 803].position = CPoint3F(10000, (float)i * 100 - 5000 + 40, 1.0f);  // Bottom right.
				m_lines[i * 4 + 803].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);


				m_lines[i * 4 + 1200].position = CPoint3F((float)i * 100 - 5000 + 60, 10000, 1.0f);  // Top left.
				m_lines[i * 4 + 1200].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);

				m_lines[i * 4 + 1201].position = CPoint3F((float)i * 100 - 5000 + 60, -10000, 1.0f);  // Bottom right.
				m_lines[i * 4 + 1201].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);

				m_lines[i * 4 + 1202].position = CPoint3F(-10000, (float)i * 100 - 5000 + 60, 1.0f);  // Top left.
				m_lines[i * 4 + 1202].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);

				m_lines[i * 4 + 1203].position = CPoint3F(10000, (float)i * 100 - 5000 + 60, 1.0f);  // Bottom right.
				m_lines[i * 4 + 1203].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);



				m_lines[i * 4 + 1600].position = CPoint3F((float)i * 100 - 5000 + 80, 10000, 1.0f);  // Top left.
				m_lines[i * 4 + 1600].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);

				m_lines[i * 4 + 1601].position = CPoint3F((float)i * 100 - 5000 + 80, -10000, 1.0f);  // Bottom right.
				m_lines[i * 4 + 1601].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);

				m_lines[i * 4 + 1602].position = CPoint3F(-10000, (float)i * 100 - 5000 + 80, 1.0f);  // Top left.
				m_lines[i * 4 + 1602].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);

				m_lines[i * 4 + 1603].position = CPoint3F(10000, (float)i * 100 - 5000 + 80, 1.0f);  // Bottom right.
				m_lines[i * 4 + 1603].color = CPoint4F(0.1f, 0.1f, 0.1f, 1.0f);
			}




			// Set up the description of the static vertex buffer.
			D3D11_BUFFER_DESC linesBufferDesc;
			linesBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			linesBufferDesc.ByteWidth = sizeof(VertexFormatVC) * mDrawLinePointCount;
			linesBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			linesBufferDesc.CPUAccessFlags = 0;
			linesBufferDesc.MiscFlags = 0;
			linesBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the vertex data.
			vertexData.pSysMem = &m_lines[0];
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			input_element[1].SemanticName = "COLOR";
			input_element[1].SemanticIndex = 0;
			input_element[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			input_element[1].InputSlot = 0;
			input_element[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			input_element[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			input_element[1].InstanceDataStepRate = 0;
			input_element.resize(2);
			result = mpDevice->getDevice()->CreateBuffer(&linesBufferDesc, &vertexData, &m_linesBuffer);
			if (FAILED(result))
			{
				return false;
			}
			if (!(mpShaderLine->createVextexShader("resource/Shader/renderLine.vs", "guiVertexShader", "vs_5_0", &input_element[0], input_element.size()) &&
				mpShaderLine->createPixelShader("resource/Shader/renderLine.ps", "guiPixelShader", "ps_5_0")))
			{
				return false;
			}
			return mpShaderLine->createVertexConstBuffer(matrices.size() * sizeof(CMatrix), 0);

#endif
			return true;
		}

		bool CGuiRenderer::initSamplrState()
		{
			D3D11_SAMPLER_DESC samplerDesc;
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.BorderColor[0] = 0;
			samplerDesc.BorderColor[1] = 0;
			samplerDesc.BorderColor[2] = 0;
			samplerDesc.BorderColor[3] = 0;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			// Create the texture sampler state.
			HRESULT result = mpDevice->getDevice()->CreateSamplerState(&samplerDesc, &m_sampleState);
			if (FAILED(result))
			{
				return false;
			}
			return true;
		}

		VertexFormatVTC* CGuiRenderer::applyVertexBuffer(INT32 nCount)
		{
			VertexFormatVTC* pBuffer = nullptr;
			if (mFreeStartPoint + nCount <= (INT32)m_Verteices.size())
			{
				pBuffer = &m_Verteices[mFreeStartPoint];
				mFreeStartPoint += nCount;
			}
			return pBuffer;
		}
	}
}

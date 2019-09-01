#include "pch.h"
#include "Terrain.h"
#include "EngineHead.h"
namespace mini
{
	CTerrain::CTerrain()
	{
	}

	CTerrain::~CTerrain()
	{
	}

	bool CTerrain::init(INT32 width, INT32 height) noexcept
	{
		if (0 == width || 0 == height)
		{
			return false;
		}
		else
		{
			m_terrainWidth = width;
			m_terrainHeight = height;
			mpMeshUnit = new CMeshUnit;
			mpUnits.push_back(mpMeshUnit);
			createTerrainData();
			createMaterial();
			return true;
		}
	}

	bool CTerrain::init(std::string strHeightMap) noexcept
	{
		if (strHeightMap.length() > 1)
		{
			mpMeshUnit = new CMeshUnit;
			mpUnits.push_back(mpMeshUnit);
			createTerrainDataByHeightMap(strHeightMap);
			createMaterial();
			return true;
		}
		else
		{
			return false;
		}
	}

	bool CTerrain::initlalise(const std::string& file, const std::string& materialName)
	{
		return true;
	}

	void CTerrain::createTerrainData()noexcept
	{
		// Calculate the number of vertices in the terrain mesh.
		INT32 vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 8;

		// Set the index count to the same as the vertex count.
		INT32	indexCount = vertexCount;

		float positionX, positionZ;
		int index = 0;


		mVertices.resize(vertexCount);
		mIndices.resize(indexCount);
		for (INT32 i = 0; i < m_terrainHeight - 1; ++i)
		{
			for (INT32 j = 0; j < m_terrainWidth - 1; ++j)
			{
				// LINE 1
				// Upper left.
				positionX = (float)i;
				positionZ = (float)(j + 1);

				mVertices[index].position = CPoint3F(positionX, 0.0f, positionZ);
				mVertices[index].normal = CPoint3F(0.0f, 1.0f, 0.0f);
				mIndices[index] = index++;
				// Upper right.
				positionX = (float)(i + 1);
				positionZ = (float)(j + 1);

				mVertices[index].position = CPoint3F(positionX, 0.0f, positionZ);
				mVertices[index].normal = CPoint3F(0.0f, 1.0f, 0.0f);
				mIndices[index] = index++;
				// LINE 2
	// Upper right.
				positionX = (float)(i + 1);
				positionZ = (float)(j + 1);

				mVertices[index].position = CPoint3F(positionX, 0.0f, positionZ);
				mVertices[index].normal = CPoint3F(0.0f, 1.0f, 0.0f);
				mIndices[index] = index++;

				// Bottom right.
				positionX = (float)(i + 1);
				positionZ = (float)j;

				mVertices[index].position = CPoint3F(positionX, 0.0f, positionZ);
				mVertices[index].normal = CPoint3F(0.0f, 1.0f, 0.0f);
				mIndices[index] = index++;

				// LINE 3
				// Bottom right.
				positionX = (float)(i + 1);
				positionZ = (float)j;

				mVertices[index].position = CPoint3F(positionX, 0.0f, positionZ);
				mVertices[index].normal = CPoint3F(0.0f, 1.0f, 0.0f);
				mIndices[index] = index;
				index++;

				// Bottom left.
				positionX = (float)i;
				positionZ = (float)j;

				mVertices[index].position = CPoint3F(positionX, 0.0f, positionZ);
				mVertices[index].normal = CPoint3F(0.0f, 1.0f, 0.0f);
				mIndices[index] = index++;

				// LINE 4
				// Bottom left.
				positionX = (float)i;
				positionZ = (float)j;

				mVertices[index].position = CPoint3F(positionX, 0.0f, positionZ);
				mVertices[index].normal = CPoint3F(0.0f, 1.0f, 0.0f);
				mIndices[index] = index++;

				// Upper left.
				positionX = (float)i;
				positionZ = (float)(j + 1);

				mVertices[index].position = CPoint3F(positionX, 0.0f, positionZ);
				mVertices[index].normal = CPoint3F(0.0f, 1.0f, 0.0f);
				mIndices[index] = index++;
			}
		}
		CResourceBase* pResource = CResourceManager::getSinglePtr()->createResource(Mesh);
		CMesh* pMesh = dynamic_cast<CMesh*>(pResource);

		pMesh->fillVertexBuffer(&mVertices[0], sizeof(VertexFormatVTN) * mVertices.size(), sizeof(VertexFormatVTN));
		pMesh->fillIndexBuffer(&mIndices[0], sizeof(UINT32)*mIndices.size(), sizeof(UINT32));
		mpMeshUnit->bindMesh(pMesh);

		SafeRelease(pMesh);
	}

	void CTerrain::createMaterial()noexcept
	{
		CMaterialUnit* pMaterialUnit = new CMaterialUnit;
		pMaterialUnit->setUnitFile("resource/Material/terrainMaterial.ma");
		pMaterialUnit->onInitialise();
		mpMeshUnit->bindMaterial(pMaterialUnit);
		SafeRelease(pMaterialUnit);
	}

	void CTerrain::createTerrainDataByHeightMap(std::string strHeightMap)noexcept
	{
		std::ifstream stream;
		stream.open(strHeightMap.c_str(), std::ios_base::in | std::ios::binary);
		if (!stream.bad())
		{
			std::stringstream buffer;
			buffer << stream.rdbuf();
			std::string contents(buffer.str());
			stream.close();

			BITMAPFILEHEADER bitmapFileHeader;
			BITMAPINFOHEADER bitmapInfoHeader;
			size_t pt = 0;
			memcpy(&bitmapFileHeader, &contents[pt], sizeof(bitmapFileHeader));
			pt += sizeof(bitmapFileHeader);
			memcpy(&bitmapInfoHeader, &contents[pt], sizeof(bitmapFileHeader));

			m_terrainWidth = bitmapInfoHeader.biWidth;
			m_terrainHeight = bitmapInfoHeader.biHeight;
			pt = bitmapFileHeader.bfOffBits;

			UINT8 height = 0;

			int index = 0;
			mVertices.resize(m_terrainWidth * m_terrainHeight);
			// Read the image data into the height map.
			for (INT32 j = 0; j < m_terrainHeight; j++)
			{
				for (INT32 i = 0; i < m_terrainWidth; i++)
				{
					height = contents[pt];

					index = (m_terrainHeight * j) + i;

					mVertices[index].position.x = (float)i;
					mVertices[index].position.y = (float)height;
					mVertices[index].position.z = (float)j;

					pt += 3;
				}
			}

			//normalize HeightMap
			for (INT32 j = 0; j < m_terrainHeight; j++)
			{
				for (INT32 i = 0; i < m_terrainWidth; i++)
				{
					mVertices[(m_terrainHeight * j) + i].position.y /= 15.0f;
				}
			}
			calculateNormals();
			reInitliseTerrainData();
		}
	}

	void CTerrain::calculateNormals()
	{
		INT32 i, j, index1, index2, index3, index, count;
		float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;

		// Create a temporary array to hold the un-normalized normal vectors.
		std::vector<D3DVECTOR> normals;
		normals.resize((m_terrainHeight - 1) * (m_terrainWidth - 1));

		// Go through all the faces in the mesh and calculate their normals.
		for (INT32 j = 0; j < (m_terrainHeight - 1); j++)
		{
			for (INT32 i = 0; i < (m_terrainWidth - 1); i++)
			{
				index1 = (j * m_terrainHeight) + i;
				index2 = (j * m_terrainHeight) + (i + 1);
				index3 = ((j + 1) * m_terrainHeight) + i;

				// Get three vertices from the face.
				vertex1[0] = mVertices[index1].position.x;
				vertex1[1] = mVertices[index1].position.y;
				vertex1[2] = mVertices[index1].position.z;

				vertex2[0] = mVertices[index2].position.x;
				vertex2[1] = mVertices[index2].position.y;
				vertex2[2] = mVertices[index2].position.z;

				vertex3[0] = mVertices[index3].position.x;
				vertex3[1] = mVertices[index3].position.y;
				vertex3[2] = mVertices[index3].position.z;

				// Calculate the two vectors for this face.
				vector1[0] = vertex1[0] - vertex3[0];
				vector1[1] = vertex1[1] - vertex3[1];
				vector1[2] = vertex1[2] - vertex3[2];
				vector2[0] = vertex3[0] - vertex2[0];
				vector2[1] = vertex3[1] - vertex2[1];
				vector2[2] = vertex3[2] - vertex2[2];

				index = (j * (m_terrainHeight - 1)) + i;

				// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
				normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
				normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
				normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
			}
		}

		// Now go through all the vertices and take an average of each face normal 	
		// that the vertex touches to get the averaged normal for that vertex.
		for (j = 0; j < m_terrainHeight; j++)
		{
			for (i = 0; i < m_terrainWidth; i++)
			{
				// Initialize the sum.
				sum[0] = 0.0f;
				sum[1] = 0.0f;
				sum[2] = 0.0f;

				// Initialize the count.
				count = 0;

				// Bottom left face.
				if (((i - 1) >= 0) && ((j - 1) >= 0))
				{
					index = ((j - 1) * (m_terrainHeight - 1)) + (i - 1);

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
					count++;
				}

				// Bottom right face.
				if ((i < (m_terrainWidth - 1)) && ((j - 1) >= 0))
				{
					index = ((j - 1) * (m_terrainHeight - 1)) + i;

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
					count++;
				}

				// Upper left face.
				if (((i - 1) >= 0) && (j < (m_terrainHeight - 1)))
				{
					index = (j * (m_terrainHeight - 1)) + (i - 1);

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
					count++;
				}

				// Upper right face.
				if ((i < (m_terrainWidth - 1)) && (j < (m_terrainHeight - 1)))
				{
					index = (j * (m_terrainHeight - 1)) + i;

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
					count++;
				}

				// Take the average of the faces touching this vertex.
				sum[0] = (sum[0] / (float)count);
				sum[1] = (sum[1] / (float)count);
				sum[2] = (sum[2] / (float)count);

				// Calculate the length of this normal.
				length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

				// Get an index to the vertex location in the height map array.
				index = (j * m_terrainHeight) + i;

				// Normalize the final shared normal for this vertex and store it in the height map array.
				mVertices[index].normal.x = (sum[0] / length);
				mVertices[index].normal.y = (sum[1] / length);
				mVertices[index].normal.z = (sum[2] / length);
			}
		}
	}

	void CTerrain::reInitliseTerrainData()
	{
		// Calculate the number of vertices in the terrain mesh.
		m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;
		INT32 index1, index2, index3, index4;
		// Load the vertex and index array with the terrain data.
		INT32 index = 0;
		std::vector<VertexFormatVTNTB> vertices;
		std::vector<UINT32> indices;
		vertices.resize(m_vertexCount);
		indices.resize(m_vertexCount);

		for (INT32 j = 0; j < (m_terrainHeight - 1); j++)
		{
			for (INT32 i = 0; i < (m_terrainWidth - 1); i++)
			{
				index1 = (m_terrainHeight * j) + i;          // Bottom left.
				index2 = (m_terrainHeight * j) + (i + 1);      // Bottom right.
				index3 = (m_terrainHeight * (j + 1)) + i;      // Upper left.
				index4 = (m_terrainHeight * (j + 1)) + (i + 1);  // Upper right.

				// Upper left.
				vertices[index].position = mVertices[index3].position;
				vertices[index].normal = mVertices[index3].normal;
				indices[index] = index;
				index++;

				// Upper right.
				vertices[index].position = mVertices[index4].position;
				vertices[index].normal = mVertices[index4].normal;
				indices[index] = index;
				index++;

				// Bottom left.
				vertices[index].position = mVertices[index1].position;
				vertices[index].normal = mVertices[index1].normal;
				indices[index] = index;
				index++;

				// Bottom left.
				vertices[index].position = mVertices[index1].position;
				vertices[index].normal = mVertices[index1].normal;
				indices[index] = index;
				index++;

				// Upper right.
				vertices[index].position = mVertices[index4].position;
				vertices[index].normal = mVertices[index4].normal;
				indices[index] = index;
				index++;

				// Bottom right.
				vertices[index].position = mVertices[index2].position;
				vertices[index].normal = mVertices[index2].normal;
				indices[index] = index;
				index++;
			}
		}
		CResourceBase* pResource = CResourceManager::getSinglePtr()->createResource(Mesh);
		CMesh* pMesh = dynamic_cast<CMesh*>(pResource);

		pMesh->fillVertexBuffer(&vertices[0], sizeof(VertexFormatVTNTB) * vertices.size(), sizeof(VertexFormatVTNTB));
		pMesh->fillIndexBuffer(&indices[0], sizeof(UINT32)*indices.size(), sizeof(UINT32));
		mpMeshUnit->bindMesh(pMesh);
		SafeRelease(pMesh);
	}
}

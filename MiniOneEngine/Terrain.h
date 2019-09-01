#pragma once
#include "Actor.h"
namespace mini
{
	class CTerrain : public CActor
	{
	public:
		CTerrain();
		virtual ~CTerrain();
		bool init(INT32 width, INT32 height) noexcept;
		bool init(std::string strHeightMap) noexcept;
		virtual bool initlalise(const std::string& file, const std::string& materialName) override;
	
	private:
		void createTerrainData()noexcept;
		void createMaterial()noexcept;
		void createTerrainDataByHeightMap(std::string strHeightMap)noexcept;
		void calculateNormals();

		//��������ǲ���Ҫ�ģ�����ʵ����ֻ��Ϊ�˼ӿ�Ե�����һ���׶εĴ�������һ��Ҫɾ��������Ȼlod�о�����Ū
		void reInitliseTerrainData();

	private:
		INT32 m_terrainWidth = 0;
		INT32 m_terrainHeight = 0;
		UINT32 m_vertexCount = 0;
		UINT32 m_indexCount = 0;
		std::vector<VertexFormatVTN> mVertices;
		std::vector<UINT32> mIndices;
	};
}

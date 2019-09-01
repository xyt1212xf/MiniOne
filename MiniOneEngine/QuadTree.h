#pragma once
#include "VertexFormat.h"

class CQuadTree
{
public:
	CQuadTree();
	~CQuadTree();
	bool creaetChildrenNode(VertexFormatVTN* pVertices, size_t VerticesNumber);
};




vertexPositionUnit = 
{
	SemanticName = "POSITION",
	SemanticIndex = 0,
	Format = 6, 						-- DXGI_FORMAT_R32G32B32_FLOAT
	InputSlot = 0,
	AlignedByteOffset = 0,
	InputSlotClass = 0, 				--D3D11_INPUT_PER_VERTEX_DATA
	InstanceDataStepRate = 0,
}

vertexTexcoordUnit = 
{
	SemanticName = "TEXCOORD",
	SemanticIndex = 0,
	Format = 16,						--DXGI_FORMAT_R32G32_FLOAT
	InputSlot = 0,
	AlignedByteOffset = 0xffffffff,		--D3D11_APPEND_ALIGNED_ELEMENT,
	InputSlotClass = 0, 				--D3D11_INPUT_PER_VERTEX_DATA
	InstanceDataStepRate = 0,
}

vertexNormalUnit = 
{
	SemanticName = "NORMAL",
	SemanticIndex = 0,
	Format = 6,							--DXGI_FORMAT_R32G32B32_FLOAT
	InputSlot = 0,
	AlignedByteOffset = 0xffffffff,		--D3D11_APPEND_ALIGNED_ELEMENT,
	InputSlotClass = 0, 				--D3D11_INPUT_PER_VERTEX_DATA
	InstanceDataStepRate = 0,
}

vertexTangentUnit = 
{
	SemanticName = "TANGENT",
	SemanticIndex = 0,
	Format = 6,							--DXGI_FORMAT_R32G32B32_FLOAT
	InputSlot = 0,
	AlignedByteOffset = 0xffffffff,		--D3D11_APPEND_ALIGNED_ELEMENT,
	InputSlotClass = 0, 				--D3D11_INPUT_PER_VERTEX_DATA
	InstanceDataStepRate = 0,
}

vertexBinormalUnit = 
{
	SemanticName = "BINORMAL",
	SemanticIndex = 0,
	Format = 6,							--DXGI_FORMAT_R32G32B32_FLOAT
	InputSlot = 0,
	AlignedByteOffset = 0xffffffff,		--D3D11_APPEND_ALIGNED_ELEMENT,
	InputSlotClass = 0, 				--D3D11_INPUT_PER_VERTEX_DATA
	InstanceDataStepRate = 0,
}

vertexFormat = {}
vertexFormat[0] =
{
	[0] = vertexPosition,
	[1] = vertexTexcoord,
}

vertexFormat[1] = 
{
	[0] = vertexPosition,
	[1] = vertexTexcoord,
	[2] = vertexNormal,
	[3] = vertexTangent,
	[4] = vertexBinormal,
}


shaderDiffuse = 
{
	vsVersion = "vs_5_0",
	psVersion = "ps_5_0",
	vs = "Resource/Shader/diffuse.vs",
	ps = "Resource/Shader/diffuse.ps",
	vsMain = "DiffuseVertexShader",
	psMain = "DiffusePixelShader",
	vertexFormat = vertexFormat[1],
}

renderToTexture = 
{
	vsVersion = "vs_5_0",
	psVersion = "ps_5_0",
	vs = "Resource/Shader/diffuse.vs",
	ps = "Resource/Shader/diffuse.ps",
	vsMain = "DiffuseVertexShader",
	psMain = "DiffusePixelShader",
	vertexFormat = vertexFormat[1],
}

--use RenderGui of name is failed ,why？？？？？
shaderRenderGui = 
{
	vsVersion = "vs_5_0",
	psVersion = "ps_5_0",
	vs = "Resource/Shader/diffuse.vs",
	ps = "Resource/Shader/diffuse.ps",
	vsMain = "DiffuseVertexShader",
	psMain = "DiffusePixelShader",
	vertexFormat = vertexFormat[0],
}



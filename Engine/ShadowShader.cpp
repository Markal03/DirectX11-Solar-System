#include "pch.h"
#include "ShadowShader.h"


ShadowShader::ShadowShader()
{
}


ShadowShader::~ShadowShader()
{
}

bool ShadowShader::InitStandard(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC	matrixBufferDesc;
	D3D11_SAMPLER_DESC	samplerDesc;
	D3D11_BUFFER_DESC	lightBufferDesc;
	D3D11_BUFFER_DESC	lightBufferDesc2;

	//LOAD SHADER:	VERTEX
	auto vertexShaderBuffer = DX::ReadData(vsFilename);
	HRESULT result = device->CreateVertexShader(vertexShaderBuffer.data(), vertexShaderBuffer.size(), NULL, &m_vertexShader);
	if (result != S_OK)
	{
		//if loading failed.
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the MeshClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Get a count of the elements in the layout.
	unsigned int numElements;
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer.data(), vertexShaderBuffer.size(), &m_layout);

	//LOAD SHADER:	PIXEL
	auto pixelShaderBuffer = DX::ReadData(psFilename);
	result = device->CreatePixelShader(pixelShaderBuffer.data(), pixelShaderBuffer.size(), NULL, &m_pixelShader);
	if (result != S_OK)
	{
		//if loading failed.
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);


	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);

	lightBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc2.ByteWidth = sizeof(LightBufferType2);
	lightBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc2.MiscFlags = 0;
	lightBufferDesc2.StructureByteStride = 0;

	device->CreateBuffer(&lightBufferDesc2, NULL, &m_lightBuffer2);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
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
	device->CreateSamplerState(&samplerDesc, &m_sampleStateWrap);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);



	return true;
}

bool ShadowShader::SetShaderParameters(
	ID3D11DeviceContext* context,
	DirectX::SimpleMath::Matrix* world,
	DirectX::SimpleMath::Matrix* view,
	DirectX::SimpleMath::Matrix* projection,
	DirectX::SimpleMath::Matrix* lightProjectionMatrix,
	DirectX::SimpleMath::Matrix* lightViewMatrix,
	Light* sceneLight1,
	ID3D11ShaderResourceView* depthMapTexture,
	ID3D11ShaderResourceView* texture
)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	LightBufferType2* dataPtr3;

	DirectX::SimpleMath::Matrix  tworld, tview, tproj, lproj, lview;

	// Transpose the matrices to prepare them for the shader.
	tworld = world->Transpose();
	tview = view->Transpose();
	tproj = projection->Transpose();
	lproj = lightProjectionMatrix->Transpose();
	lview = lightViewMatrix->Transpose();

	context->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	dataPtr->lightView = lview;
	dataPtr->lightProjection = lproj;

	context->Unmap(m_matrixBuffer, 0);
	context->VSSetConstantBuffers(0, 1, &m_matrixBuffer);	//note the first variable is the mapped buffer ID.  Corresponding to what you set in the VS
	context->PSSetShaderResources(0, 1, &texture);
	context->PSSetShaderResources(1, 1, &depthMapTexture);

	context->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr2 = (LightBufferType*)mappedResource.pData;
	dataPtr2->ambient = sceneLight1->getAmbientColour();
	dataPtr2->diffuse = sceneLight1->getDiffuseColour();
	context->Unmap(m_lightBuffer, 0);
	context->PSSetConstantBuffers(0, 1, &m_lightBuffer);	//note the first variable is the mapped buffer ID.  Corresponding to what you set in the PS

	context->Map(m_lightBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr3 = (LightBufferType2*)mappedResource.pData;
	dataPtr3->position = sceneLight1->getPosition();
	dataPtr3->padding = 0.0f;
	context->Unmap(m_lightBuffer2, 0);
	context->PSSetConstantBuffers(1, 1, &m_lightBuffer2);
	//pass the desired texture to the pixel shader.



	return false;
}

void ShadowShader::EnableShader(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(m_layout);							//set the input layout for the shader to match out geometry
	context->VSSetShader(m_vertexShader.Get(), 0, 0);				//turn on vertex shader
	context->PSSetShader(m_pixelShader.Get(), 0, 0);				//turn on pixel shader
	// Set the sampler state in the pixel shader.
	context->PSSetSamplers(0, 1, &m_sampleStateClamp);
	context->PSSetSamplers(1, 1, &m_sampleStateWrap);

}
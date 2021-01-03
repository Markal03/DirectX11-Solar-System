
#include "pch.h"
#include "Game.h"


//toreorganise
#include <fstream>

# define M_PI 3.14159265358979323846

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;
float screen_width = 1980;
float screen_height = 1080;

//Mercury
float mercury_angle = 0.0f;
double mercury_orbit_radians = 0.0;
float mercury_angle_2 = 0.0f;
double mercury_orbit_radians_2 = 0.0;

//Venus
double venus_angle = 0.0f;
double venus_orbit_radians = 0.0;
double venus_angle_2 = 0.0f;
double venus_orbit_radians_2 = 0.0;

//Earth
float earth_angle = 0.0f;
double earth_orbit_radians = 0.0;
float earth_angle_2 = 0.0f;
double earth_orbit_radians_2 = 0.0;

//Mars
float mars_angle = 0.0f;
double mars_orbit_radians = 0.0;
float mars_angle_2 = 0.0f;
double mars_orbit_radians_2 = 0.0;

//Jupiter
float jupiter_angle = 0.0f;
double jupiter_orbit_radians = 0.0;
float jupiter_angle_2 = 0.0f;
double jupiter_orbit_radians_2 = 0.0;

//Saturn
float saturn_angle = 0.0f;
double saturn_orbit_radians = 0.0;
float saturn_angle_2 = 0.0f;
double saturn_orbit_radians_2 = 0.0;

//Uranus
float uranus_angle = 0.0f;
double uranus_orbit_radians = 0.0;
float uranus_angle_2 = 0.0f;
double uranus_orbit_radians_2 = 0.0;

//Neptune
float neptune_angle = 0.0f;
double neptune_orbit_radians = 0.0;
float neptune_angle_2 = 0.0f;
double neptune_orbit_radians_2 = 0.0;

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game() {}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{

	m_input.Initialise(window);

    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

	//setup light
	m_Light.setAmbientColour(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light.setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
    m_Light.setLookAt(0.0f, 0.0f, 0.0f);

    m_Light.generateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);
	m_Light.setPosition(0.0f, 5.f, 0.0f);

    m_Light.generateViewMatrix();
    m_lightViewMatrix = m_Light.getViewMatrix();
    m_lightProjectionMatrix = m_Light.getProjectionMatrix();

	//setup camera
	m_Camera01.setPosition(Vector3(0.0f, 0.0f, 20.0f));
    m_Camera01.setForward(Vector3(0.0f, 0.0f, 0.0f));
	m_Camera01.setRotation(Vector3(-90.0f, 0.0f, 0.0f));
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
	//take in input
	m_input.Update();								//update the hardware
	m_gameInputCommands = m_input.getGameInput();	//retrieve the input for our game

	//Update all game objects
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

	//Render all game content.
    Render();
}

void Game::Update(DX::StepTimer const& timer)
{
	if (m_gameInputCommands.left)
	{

        Vector3 position = m_Camera01.getPosition(); //get the position
        position -= m_Camera01.getRight() * m_Camera01.getMoveSpeed(); //add the forward vector
        m_Camera01.setPosition(position);
	}
	if (m_gameInputCommands.right)
	{

        Vector3 position = m_Camera01.getPosition(); //get the position
        position += m_Camera01.getRight() * m_Camera01.getMoveSpeed(); //add the forward vector
        m_Camera01.setPosition(position);
	}
	if (m_gameInputCommands.forward)
	{
        Vector3 position = m_Camera01.getPosition(); //get the position
        position += (m_Camera01.getForward()*m_Camera01.getMoveSpeed()); //add the forward vector
        m_Camera01.setPosition(position);

	}
	if (m_gameInputCommands.back)
	{
		Vector3 position = m_Camera01.getPosition(); //get the position
		position -= (m_Camera01.getForward()*m_Camera01.getMoveSpeed()); //add the forward vector
		m_Camera01.setPosition(position);
	}

    if (m_gameInputCommands.rotDown) {
        Vector3 rotation = m_Camera01.getRotation();
        if (rotation.x > -160 && rotation.x < -17)
        rotation.x -= m_Camera01.getRotationSpeed();
        m_Camera01.setRotation(rotation);
    }

    if (m_gameInputCommands.rotUp) {
        Vector3 rotation = m_Camera01.getRotation();
        if (rotation.x > -163 && rotation.x < -20)
        rotation.x += m_Camera01.getRotationSpeed();
        m_Camera01.setRotation(rotation);
    }

    if (m_gameInputCommands.rotRight) {
        Vector3 rotation = m_Camera01.getRotation();
        rotation.y = rotation.y -= m_Camera01.getRotationSpeed();
        m_Camera01.setRotation(rotation);
    }

    if (m_gameInputCommands.rotLeft) {
        Vector3 rotation = m_Camera01.getRotation();
        rotation.y = rotation.y += m_Camera01.getRotationSpeed();
        m_Camera01.setRotation(rotation);
    }

    if (m_gameInputCommands.up)
    {
        Vector3 position = m_Camera01.getPosition();
        position -= m_Camera01.getUp() * m_Camera01.getMoveSpeed();
        m_Camera01.setPosition(position);
    }

    if (m_gameInputCommands.down)
    {
        Vector3 position = m_Camera01.getPosition();
        position += m_Camera01.getUp() * m_Camera01.getMoveSpeed();
        m_Camera01.setPosition(position);
    }

    if (m_gameInputCommands.reset)
    {
        m_Camera01.setPosition(Vector3(0.0f, 0.0f, 20.0f));
        m_Camera01.setRotation(Vector3(-90.0f, 0.0f, 0.0f));
    }

	m_Camera01.Update();	//camera update.
    m_Light.generateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);
    m_Light.generateViewMatrix();
	m_view = m_Camera01.getCameraMatrix();
	m_world = Matrix::Identity;

	if (m_input.Quit())
	{
		ExitGame();
	}
}
#pragma endregion

#pragma region Frame Render
void Game::Render()
{
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();
    
    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    

	//Set Rendering states.
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	//context->RSSetState(m_states->Wireframe());

    const int vCountCircle = 101;
    const int iCountCircle = 300;
    uint16_t iArrayCircle[iCountCircle];
    VertexPositionColor vArrayCircle[vCountCircle];

    float radius = 6.0;
    float circleDeltaTheta = 2 * 3.14159265358979323846 / float(vCountCircle - 1);
    vArrayCircle[0] = VertexPositionColor(Vector3(0.f, 0.f, 0.f), Colors::Teal);     //center
    vArrayCircle[1] = VertexPositionColor(Vector3(radius * cos(0.f), 0.f, radius * sin(0.f)), Colors::Lime);
    vArrayCircle[2] = VertexPositionColor(Vector3(radius * cos(circleDeltaTheta), 0.f, radius * sin(circleDeltaTheta)), Colors::Lime);

    iArrayCircle[0] = 0;
    iArrayCircle[1] = 1;
    iArrayCircle[2] = 2;
    float circleTheta = 0.0f;

    for (int i = 1; i < vCountCircle - 2; i++)
    {
        int index = 3 * i;
        iArrayCircle[index] = 0;
        iArrayCircle[index + 1] = iArrayCircle[index - 1];
        iArrayCircle[index + 2] = iArrayCircle[index - 1] + 1;

        circleTheta = circleTheta + circleDeltaTheta;
        vArrayCircle[i + 2] = VertexPositionColor(Vector3(radius * cos(circleTheta + circleDeltaTheta), 0.f, radius * sin(circleTheta + circleDeltaTheta)), Colors::Lime);
    }

    iArrayCircle[iCountCircle - 3] = 0;
    iArrayCircle[iCountCircle - 2] = vCountCircle - 1;
    iArrayCircle[iCountCircle - 1] = 1;

    RenderTexturePass1();

    Quaternion quaternionRotation;

    context->RSSetState(m_states->CullNone());
    //Skybox
    SimpleMath::Matrix _cameraTranslation = SimpleMath::Matrix::CreateTranslation(m_Camera01.getPosition());
    m_world = m_world * _cameraTranslation;
    m_noLightShader.EnableShader(context);
    m_noLightShader.SetShaderParameters(context, &m_world, &m_view, &m_projection, &m_Light, m_backSkyboxTexture.Get());
    m_SkyBoxModel.Render(context);


    m_world = SimpleMath::Matrix::Identity;

    //Sun
    context->RSSetState(m_states->CullClockwise());
    m_noLightShader.EnableShader(context);
    m_noLightShader.SetShaderParameters(context, &m_world, &m_view, &m_projection, &m_Light, m_sunTexture.Get());
    m_SunModel.Render(context);

    

    //Mercury
    mercury_angle += MERCURY_ORBITAL_VELOCITY;
    float radians = mercury_angle * M_PI / 180;
    mercury_orbit_radians += radians;

    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    SimpleMath::Matrix rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    SimpleMath::Matrix newPosition = SimpleMath::Matrix::CreateTranslation(7 * cos(radians), 0, 7 * sin(radians));

    m_world = m_world * rotationY * newPosition;
    m_shadowShader.EnableShader(context);
    m_shadowShader.SetShaderParameters(context, &m_world, &m_view, &m_projection, &m_lightProjectionMatrix, &m_lightViewMatrix, &m_Light, m_RenderTexture->getShaderResourceView(), m_mercuryTexture.Get());
    m_MercuryModel.Render(context);
    m_world = SimpleMath::Matrix::Identity;

    //Venus
    venus_angle += VENUS_ORBITAL_VELOCITY;
    radians = venus_angle * M_PI / 180;
    venus_orbit_radians += radians;

    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(10 * cos(radians), 0, 10 * sin(radians));
    m_world = m_world * rotationY * newPosition;

    m_shadowShader.EnableShader(context);
    m_shadowShader.SetShaderParameters(context, &m_world, &m_view, &m_projection, &m_lightProjectionMatrix, &m_lightViewMatrix, &m_Light, m_RenderTexture->getShaderResourceView(), m_venusTexure.Get());
    m_VenusModel.Render(context);

    m_world = SimpleMath::Matrix::Identity;

    //Earth
    earth_angle += EARTH_ORBITAL_VELOCITY;
    radians = earth_angle * M_PI / 180;
    earth_orbit_radians += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(15 * cos(radians), 0, 15 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_shadowShader.EnableShader(context);
    m_shadowShader.SetShaderParameters(context, &m_world, &m_view, &m_projection, &m_lightProjectionMatrix, &m_lightViewMatrix, &m_Light, m_RenderTexture->getShaderResourceView(), m_earthTexure.Get());
    m_EarthModel.Render(context);

    //Mars
    mars_angle += MARS_ORBITAL_VELOCITY;
    radians = mars_angle * M_PI / 180;
    mars_orbit_radians += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(20 * cos(radians), 0, 20 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_shadowShader.EnableShader(context);
    m_shadowShader.SetShaderParameters(context, &m_world, &m_view, &m_projection, &m_lightProjectionMatrix, &m_lightViewMatrix, &m_Light, m_RenderTexture->getShaderResourceView(), m_marsTexure.Get());
    m_MarsModel.Render(context);

    //Jupiter
    jupiter_angle += JUPITER_ORBITAL_VELOCITY;
    radians = jupiter_angle * M_PI / 180;
    jupiter_orbit_radians += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(30 * cos(radians), 0, 30 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_shadowShader.EnableShader(context);
    m_shadowShader.SetShaderParameters(context, &m_world, &m_view, &m_projection, &m_lightProjectionMatrix, &m_lightViewMatrix, &m_Light, m_RenderTexture->getShaderResourceView(), m_jupiterTexure.Get());
    m_JupiterModel.Render(context);

    //Saturn
    saturn_angle += SATURN_ORBITAL_VELOCITY;
    radians = saturn_angle * M_PI / 180;
    saturn_orbit_radians += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(40 * cos(radians), 0, 40 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_shadowShader.EnableShader(context);
    m_shadowShader.SetShaderParameters(context, &m_world, &m_view, &m_projection, &m_lightProjectionMatrix, &m_lightViewMatrix, &m_Light, m_RenderTexture->getShaderResourceView(), m_saturnTexure.Get());
    m_SaturnModel.Render(context);

    context->RSSetState(m_states->CullNone());
    m_noLightShader.EnableShader(context);
    m_noLightShader.SetShaderParameters(context, &m_world, &m_view, &m_projection, &m_Light, m_saturnRingTexture.Get());

    m_batch->Begin();
    m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, iArrayCircle, iCountCircle, vArrayCircle, vCountCircle);
    m_batch->End();


    context->RSSetState(m_states->CullClockwise());
    //Uranus
    uranus_angle += URANUS_ORBITAL_VELOCITY;
    radians = uranus_angle * M_PI / 180;
    uranus_orbit_radians += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(48 * cos(radians), 0, 48 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_shadowShader.EnableShader(context);
    m_shadowShader.SetShaderParameters(context, &m_world, &m_view, &m_projection, &m_lightProjectionMatrix, &m_lightViewMatrix, &m_Light, m_RenderTexture->getShaderResourceView(), m_uranusTexure.Get());
    m_UranusModel.Render(context);

    //Neptune
    neptune_angle += NEPTUNE_ORBITAL_VELOCITY;
    radians = neptune_angle * M_PI / 180;
    neptune_orbit_radians += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(55 * cos(radians), 0, 55 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_shadowShader.EnableShader(context);
    m_shadowShader.SetShaderParameters(context, &m_world, &m_view, &m_projection, &m_lightProjectionMatrix, &m_lightViewMatrix, &m_Light, m_RenderTexture->getShaderResourceView(), m_neptuneTexure.Get());
    m_NeptuneModel.Render(context);


    m_world = SimpleMath::Matrix::Identity;
    // Show the new frame.
    m_deviceResources->PIXBeginEvent(L"Draw sprite");
    m_sprites->Begin();
    m_font->DrawString(m_sprites.get(), L"Assessment Scene - Marco Lori", XMFLOAT2(10, 10), Colors::Yellow);
    m_font->DrawString(m_sprites.get(), L"Camera controls:", XMFLOAT2(10, 30), Colors::Yellow);
    m_font->DrawString(m_sprites.get(), L"Movement: W, A, S, D", XMFLOAT2(10, 50), Colors::Yellow);
    m_font->DrawString(m_sprites.get(), L"UP and DOWN: SPACE BAR, X", XMFLOAT2(10, 70), Colors::Yellow);
    m_font->DrawString(m_sprites.get(), L"Rotation: UP, LEFT, DOWN, RIGHT or MOUSE LB drag", XMFLOAT2(10, 90), Colors::Yellow);
    m_font->DrawString(m_sprites.get(), L"Camera reset: R", XMFLOAT2(10, 110), Colors::Yellow);
    m_sprites->End();
    m_deviceResources->PIXEndEvent();

    m_deviceResources->Present();
}

void Game::RenderTexturePass1()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTargetView = m_deviceResources->GetRenderTargetView();
    auto depthTargetView = m_deviceResources->GetDepthStencilView();

    // Set the render target to be the render to texture.
    m_RenderTexture->setRenderTarget(context);
    // Clear the render to texture.
    m_RenderTexture->clearRenderTarget(context, 0.0f, 0.0f, 1.0f, 1.0f);

    m_Light.generateViewMatrix();

    m_lightViewMatrix = m_Light.getViewMatrix();
    m_lightProjectionMatrix = m_Light.getProjectionMatrix();


    const int vCountCircle = 101;
    const int iCountCircle = 300;
    uint16_t iArrayCircle[iCountCircle];
    VertexPositionColor vArrayCircle[vCountCircle];

    float radius = 6.0;
    float circleDeltaTheta = 2 * 3.14159265358979323846 / float(vCountCircle - 1);
    vArrayCircle[0] = VertexPositionColor(Vector3(0.f, 0.f, 0.f), Colors::Teal);     //center
    vArrayCircle[1] = VertexPositionColor(Vector3(radius * cos(0.f), 0.f, radius * sin(0.f)), Colors::Lime);
    vArrayCircle[2] = VertexPositionColor(Vector3(radius * cos(circleDeltaTheta), 0.f, radius * sin(circleDeltaTheta)), Colors::Lime);

    iArrayCircle[0] = 0;
    iArrayCircle[1] = 1;
    iArrayCircle[2] = 2;
    float circleTheta = 0.0f;

    for (int i = 1; i < vCountCircle - 2; i++)
    {
        int index = 3 * i;
        iArrayCircle[index] = 0;
        iArrayCircle[index + 1] = iArrayCircle[index - 1];
        iArrayCircle[index + 2] = iArrayCircle[index - 1] + 1;

        circleTheta = circleTheta + circleDeltaTheta;
        vArrayCircle[i + 2] = VertexPositionColor(Vector3(radius * cos(circleTheta + circleDeltaTheta), 0.f, radius * sin(circleTheta + circleDeltaTheta)), Colors::Lime);
    }

    iArrayCircle[iCountCircle - 3] = 0;
    iArrayCircle[iCountCircle - 2] = vCountCircle - 1;
    iArrayCircle[iCountCircle - 1] = 1;

    Quaternion quaternionRotation;

    m_DepthShader.EnableShader(context);

    SimpleMath::Matrix _cameraTranslation = SimpleMath::Matrix::CreateTranslation(m_Camera01.getPosition());
    m_world = m_world * _cameraTranslation;
    
    m_DepthShader.SetShaderParameters(context, &m_world, &m_lightViewMatrix, &m_lightProjectionMatrix);
    m_SkyBoxModel.Render(context);

    m_world = SimpleMath::Matrix::Identity;

    //Sun
    context->RSSetState(m_states->CullClockwise());
    m_DepthShader.EnableShader(context);
    m_DepthShader.SetShaderParameters(context, &m_world, &m_lightViewMatrix, &m_lightProjectionMatrix);
    m_SunModel.Render(context);

    

    //Mercury
    mercury_angle_2 += MERCURY_ORBITAL_VELOCITY;
    double radians = mercury_angle_2 * M_PI / 180;
    mercury_orbit_radians_2 += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    SimpleMath::Matrix rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    SimpleMath::Matrix newPosition = SimpleMath::Matrix::CreateTranslation(7 * cos(radians), 0, 7 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_DepthShader.EnableShader(context);
    m_DepthShader.SetShaderParameters(context, &m_world, &m_lightViewMatrix, &m_lightProjectionMatrix);
    m_MercuryModel.Render(context);

    //Venus
    venus_angle_2 += VENUS_ORBITAL_VELOCITY;
    radians = venus_angle_2 * M_PI / 180;
    venus_orbit_radians_2 += radians;

    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(10 * cos(radians), 0, 10 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_DepthShader.EnableShader(context);
    m_DepthShader.SetShaderParameters(context, &m_world, &m_lightViewMatrix, &m_lightProjectionMatrix);
    m_VenusModel.Render(context);

    //Earth
    earth_angle_2 += EARTH_ORBITAL_VELOCITY;
    radians = earth_angle_2 * M_PI / 180;
    earth_orbit_radians_2 += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(15 * cos(radians), 0, 15 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_DepthShader.EnableShader(context);
    m_DepthShader.SetShaderParameters(context, &m_world, &m_lightViewMatrix, &m_lightProjectionMatrix);
    m_EarthModel.Render(context);

    //Mars
    mars_angle_2 += MARS_ORBITAL_VELOCITY;
    radians = mars_angle_2 * M_PI / 180;
    mars_orbit_radians_2 += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(20 * cos(radians), 0, 20 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_DepthShader.EnableShader(context);
    m_DepthShader.SetShaderParameters(context, &m_world, &m_lightViewMatrix, &m_lightProjectionMatrix);
    m_MarsModel.Render(context);

    //Jupiter
    jupiter_angle_2 += JUPITER_ORBITAL_VELOCITY;
    radians = jupiter_angle_2 * M_PI / 180;
    jupiter_orbit_radians_2 += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(30 * cos(radians), 0, 30 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_DepthShader.EnableShader(context);
    m_DepthShader.SetShaderParameters(context, &m_world, &m_lightViewMatrix, &m_lightProjectionMatrix);
    m_JupiterModel.Render(context);

    //Saturn
    saturn_angle_2 += SATURN_ORBITAL_VELOCITY;
    radians = saturn_angle_2 * M_PI / 180;
    saturn_orbit_radians_2 += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(40 * cos(radians), 0, 40 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_DepthShader.EnableShader(context);
    m_DepthShader.SetShaderParameters(context, &m_world, &m_lightViewMatrix, &m_lightProjectionMatrix);
    m_SaturnModel.Render(context);

    context->RSSetState(m_states->CullNone());
    m_DepthShader.EnableShader(context);
    m_DepthShader.SetShaderParameters(context, &m_world, &m_lightViewMatrix, &m_lightProjectionMatrix);

    m_batch->Begin();
    m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, iArrayCircle, iCountCircle, vArrayCircle, vCountCircle);
    m_batch->End();

    context->RSSetState(m_states->CullClockwise());

    //Uranus
    uranus_angle_2 += URANUS_ORBITAL_VELOCITY;
    radians = uranus_angle_2 * M_PI / 180;
    uranus_orbit_radians_2 += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(48 * cos(radians), 0, 48 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_DepthShader.EnableShader(context);
    m_DepthShader.SetShaderParameters(context, &m_world, &m_lightViewMatrix, &m_lightProjectionMatrix);
    m_UranusModel.Render(context);

    //Neptune
    neptune_angle_2 += NEPTUNE_ORBITAL_VELOCITY;
    radians = neptune_angle_2 * M_PI / 180;
    neptune_orbit_radians_2 += radians;

    m_world = SimpleMath::Matrix::Identity;
    quaternionRotation = XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
    rotationY = DirectX::XMMatrixRotationQuaternion(quaternionRotation);
    newPosition = SimpleMath::Matrix::CreateTranslation(55 * cos(radians), 0, 55 * sin(radians));
    m_world = m_world * rotationY * newPosition;
    m_DepthShader.EnableShader(context);
    m_DepthShader.SetShaderParameters(context, &m_world, &m_lightViewMatrix, &m_lightProjectionMatrix);
    m_NeptuneModel.Render(context);


    m_world = SimpleMath::Matrix::Identity;
 
    context->OMSetRenderTargets(1, &renderTargetView, depthTargetView);

}
// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}

#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();
}

void Game::OnWindowMoved()
{
    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    width = 1980;
    height = 1080;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto device = m_deviceResources->GetD3DDevice();

    m_states = std::make_unique<CommonStates>(device);
    m_fxFactory = std::make_unique<EffectFactory>(device);
    m_sprites = std::make_unique<SpriteBatch>(context);
    m_font = std::make_unique<SpriteFont>(device, L"Courier_16.spritefont");
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	//setup our test model
	m_SunModel.InitializeModel(device, "Sphere.obj");
	m_MercuryModel.InitializeSphere(device, 1);
	m_VenusModel.InitializeSphere(device, 2);
	m_EarthModel.InitializeSphere(device, 3);
	m_MarsModel.InitializeSphere(device, 2);
	m_JupiterModel.InitializeSphere(device, 8);
	m_SaturnModel.InitializeSphere(device, 7);
	m_UranusModel.InitializeSphere(device, 5);
	m_NeptuneModel.InitializeSphere(device, 5);
    m_SkyBoxModel.InitializeSphere(device, 200);
    m_TestBoxModel.InitializeBox(device, 200, 10, 200);
	//load and set up our Vertex and Pixel Shaders
	m_BasicShaderPair.InitStandard(device, L"light_vs.cso", L"light_ps.cso");
    m_noLightShader.InitStandard(device, L"nolight_vs.cso", L"nolight_ps.cso");
    m_SkyBoxShader.InitStandard(device, L"SkyBox_vs.cso", L"SkyBox_ps.cso");
    m_shadowShader.InitStandard(device, L"shadow_vs.cso", L"shadow_ps.cso");
    m_DepthShader.InitStandard(device, L"depth_vs.cso", L"depth_ps.cso");

	//load Textures
	CreateDDSTextureFromFile(device, L"SunTexture.dds",		nullptr,	    m_sunTexture.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"MercuryTexture.dds", nullptr,	    m_mercuryTexture.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"VenusTexture.dds", nullptr,          m_venusTexure.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"EarthTexture.dds", nullptr,          m_earthTexure.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"MarsTexture.dds", nullptr,           m_marsTexure.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"JupiterTexture.dds", nullptr,        m_jupiterTexure.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"SaturnTexture.dds", nullptr,         m_saturnTexure.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"UranusTexture.dds", nullptr,         m_uranusTexure.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"NeptuneTexture.dds", nullptr,        m_neptuneTexure.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"JupiterTexture.dds", nullptr,     m_saturnRingTexture.ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile(device, L"back.dds", nullptr,                  m_backSkyboxTexture.ReleaseAndGetAddressOf());

    m_RenderTexture = new RenderTexture(device, screen_width, screen_height, SCREEN_DEPTH, SCREEN_NEAR);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    auto size = m_deviceResources->GetOutputSize();
    float aspectRatio = float(size.right) / float(size.bottom);
    float fovAngleY = 70.0f * XM_PI / 180.0f;
    screen_width = size.right;
    screen_height = size.bottom;
    // This is a simple example of change that can be made when the app is in
    // portrait or snapped view.
    if (aspectRatio < 1.0f)
    {
        fovAngleY *= 2.0f;
    }

    // This sample makes use of a right-handed coordinate system using row-major matrices.
    m_projection = Matrix::CreatePerspectiveFieldOfView(
        fovAngleY,
        aspectRatio,
        0.01f,
        100.0f
    );
}


void Game::OnDeviceLost()
{
    m_states.reset();
    m_fxFactory.reset();
    m_sprites.reset();
    m_font.reset();
	m_batch.reset();
	m_testmodel.reset();
    m_batchInputLayout.Reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

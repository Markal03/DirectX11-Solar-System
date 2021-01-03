//
// Game.h
//
#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "Shader.h"
#include "ShadowShader.h"
#include "Depthshader.h"
#include "modelclass.h"
#include "Light.h"
#include "Input.h"
#include "Camera.h"
#include "RenderTexture.h"

const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.f;

//Planets orbital velocity -> 1 = 30km/s~
const float MERCURY_ORBITAL_VELOCITY = 1.57f;
const float VENUS_ORBITAL_VELOCITY = 1.17f;
const float EARTH_ORBITAL_VELOCITY = 1.0f;
const float MARS_ORBITAL_VELOCITY = 0.8f;
const float JUPITER_ORBITAL_VELOCITY = 0.44f;
const float SATURN_ORBITAL_VELOCITY = 0.33f;
const float URANUS_ORBITAL_VELOCITY = 0.27f;
const float NEPTUNE_ORBITAL_VELOCITY = 0.18f;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);
#ifdef DXTK_AUDIO
    void NewAudioDevice();
#endif

    // Properties
    void GetDefaultSize( int& width, int& height ) const;
	
private:

	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	}; 

    void Update(DX::StepTimer const& timer);
    void Render();
    void Clear();
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    void RenderTexturePass1();
    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

	//input manager. 
	Input									m_input;
	InputCommands							m_gameInputCommands;

    // DirectXTK objects.
    std::unique_ptr<DirectX::CommonStates>                                  m_states;
    std::unique_ptr<DirectX::BasicEffect>                                   m_batchEffect;	
    std::unique_ptr<DirectX::EffectFactory>                                 m_fxFactory;
    std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
    std::unique_ptr<DirectX::SpriteFont>                                    m_font;

	// Scene Objects
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                               m_batchInputLayout;
	std::unique_ptr<DirectX::GeometricPrimitive>                            m_testmodel;

	//lights
	Light																	m_Light;
	Light																	m_Light1;

	//Cameras
	Camera																	m_Camera01;

	//textures 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_sunTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_mercuryTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_venusTexure;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_earthTexure;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_marsTexure;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_jupiterTexure;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_saturnTexure;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_uranusTexure;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_neptuneTexure;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_saturnRingTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_backSkyboxTexture;

	//Shaders
	Shader																	m_BasicShaderPair;
	Shader																	m_SkyBoxShader;
    Shader                                                                  m_noLightShader;
    DepthShader                                                             m_DepthShader;
    ShadowShader                                                            m_shadowShader;
	ModelClass																m_SunModel;
	ModelClass																m_MercuryModel;
	ModelClass																m_VenusModel;
	ModelClass																m_EarthModel;
	ModelClass																m_MarsModel;
	ModelClass																m_JupiterModel;
	ModelClass																m_SaturnModel;
	ModelClass																m_UranusModel;
	ModelClass																m_NeptuneModel;
    ModelClass                                                              m_SkyBoxModel;
    ModelClass                                                              m_TestBoxModel;

    //Textures
    RenderTexture*                                                          m_RenderTexture;

#ifdef DXTK_AUDIO
    std::unique_ptr<DirectX::AudioEngine>                                   m_audEngine;
    std::unique_ptr<DirectX::WaveBank>                                      m_waveBank;
    std::unique_ptr<DirectX::SoundEffect>                                   m_soundEffect;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect1;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect2;
#endif
    

#ifdef DXTK_AUDIO
    uint32_t                                                                m_audioEvent;
    float                                                                   m_audioTimerAcc;

    bool                                                                    m_retryDefault;
#endif

    DirectX::SimpleMath::Matrix                                             m_world;
    DirectX::SimpleMath::Matrix                                             m_view;
    DirectX::SimpleMath::Matrix                                             m_projection;
    DirectX::SimpleMath::Matrix                                             m_lightProjectionMatrix;
    DirectX::SimpleMath::Matrix                                             m_lightViewMatrix;
};
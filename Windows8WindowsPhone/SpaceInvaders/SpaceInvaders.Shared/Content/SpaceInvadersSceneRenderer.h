#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "Sprite.h"

namespace SpaceInvaders
{
	// This sample renderer instantiates a basic rendering pipeline.
	class SpaceInvadersSceneRenderer
	{
	public:
		SpaceInvadersSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();

		void CreateBackground();
		void CreateShip();
		void CreateEnemies();
		void ControlShip();
		void FireMissile();
		void HandleMissiles();
		bool CollidingWithEnemyAndKilledIt(std::shared_ptr<Sprite> missile);
		void MoveEnemies();

	private:
		void Rotate(float radians);

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<DirectX::CommonStates> states;
		std::shared_ptr<DirectX::SpriteBatch> spriteBatch;
		Windows::Foundation::Size screenSize;

		float time;
		float timeDelta;
		float lastTimeShot;
		std::shared_ptr<Sprite> background;
		std::shared_ptr<Sprite> ship;
		float shipPosition = 0;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> missileTexture;
		std::vector<std::shared_ptr<Sprite>> activeMissiles;
		std::vector<std::shared_ptr<Sprite>> enemies;
		float enemiesPosition = 0;
		bool enemiesMovingRight = true;

		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> background;
		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ship;

		/*not needed
		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;

		// System resources for cube geometry.
		ModelViewProjectionConstantBuffer	m_constantBufferData;
		uint32	m_indexCount;
		
		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;
		*/
	};
}


#include "pch.h"
#include "SpaceInvadersSceneRenderer.h"
#include "..\Common\DirectXHelper.h"

using namespace SpaceInvaders;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Windows::UI::Core;

SpaceInvadersSceneRenderer::SpaceInvadersSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	CoreWindow::GetForCurrentThread()->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>([&](CoreWindow^ window, PointerEventArgs^ args)
	{
		shipPosition = -1 + 2 * (args->CurrentPoint->Position.X / window->Bounds.Width);
	});
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void SpaceInvadersSceneRenderer::CreateDeviceDependentResources()
{
	states.reset(new CommonStates(m_deviceResources->GetD3DDevice()));
	spriteBatch.reset(new SpriteBatch(m_deviceResources->GetD3DDeviceContext()));
	CreateBackground();
	CreateShip();
	CreateEnemies();
}

void SpaceInvadersSceneRenderer::CreateBackground()
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> backgroundTexture;
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"Background.png", nullptr,
		backgroundTexture.ReleaseAndGetAddressOf()));
	background = std::make_shared<Sprite>(backgroundTexture, 0.0f, 0.0f, 2.0f, 1.0f);
}

void SpaceInvadersSceneRenderer::CreateShip()
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shipTexture;
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"Ship.png", nullptr,
		shipTexture.ReleaseAndGetAddressOf()));
	ship = std::make_shared<Sprite>(shipTexture, 0.0f, -0.8f, 0.075f, 0.075f);
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"Missile.png", nullptr,
		missileTexture.ReleaseAndGetAddressOf()));
}

void SpaceInvadersSceneRenderer::CreateEnemies()
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> enemyTexture;
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"Enemy.png", nullptr,
		enemyTexture.ReleaseAndGetAddressOf()));
	for (float y = 0.7f; y > 0.3f; y -= 0.3f)
		for (float x = -0.8f; x <= 0.8f; x += 0.2f)
			enemies.push_back(std::make_shared<Sprite>(enemyTexture, x, y, 0.05f, 0.05f));
}

void SpaceInvadersSceneRenderer::CreateWindowSizeDependentResources()
{
	screenSize = m_deviceResources->GetOutputSize();
}

void SpaceInvadersSceneRenderer::Update(DX::StepTimer const& timer)
{
	time = (float)timer.GetTotalSeconds();
	timeDelta = (float)timer.GetElapsedSeconds();
	ControlShip();
	MoveEnemies();
}

void SpaceInvadersSceneRenderer::ControlShip()
{
	if (lastTimeShot < time - 0.33f)
		FireMissile();
	HandleMissiles();
}

void SpaceInvadersSceneRenderer::FireMissile()
{
	lastTimeShot = time;
	activeMissiles.push_back(std::make_shared<Sprite>(missileTexture, shipPosition, -0.67f, 0.02f, 0.02f));
}

void SpaceInvadersSceneRenderer::HandleMissiles()
{
	activeMissiles.erase(std::remove_if(activeMissiles.begin(), activeMissiles.end(),
		[=](std::shared_ptr<Sprite> missile)
	{
		return missile->IncreaseY(timeDelta * 3) || CollidingWithEnemyAndKilledIt(missile);
	}), activeMissiles.end());
}

bool SpaceInvadersSceneRenderer::CollidingWithEnemyAndKilledIt(std::shared_ptr<Sprite> missile)
{
	for (auto enemy : enemies)
		if (enemy->DistanceTo(missile, enemiesPosition) < 0.05f)
		{
			enemy->IncreaseY(1.0f); // Move out of screen
			return true;
		}
	return false;
}

void SpaceInvadersSceneRenderer::MoveEnemies()
{
	enemiesPosition += (enemiesMovingRight ? 0.2f : -0.2f) * timeDelta;
	if (enemiesPosition < -0.1f || enemiesPosition > 0.1f)
		enemiesMovingRight = !enemiesMovingRight;
}

void SpaceInvadersSceneRenderer::Render()
{
	spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());
	background->Draw(spriteBatch, screenSize);
	for (auto missile : activeMissiles)
		missile->Draw(spriteBatch, screenSize);
	ship->Draw(spriteBatch, screenSize, shipPosition);
	for (auto enemy : enemies)
		enemy->Draw(spriteBatch, screenSize, enemiesPosition);
	spriteBatch->End();
}

void SpaceInvadersSceneRenderer::ReleaseDeviceDependentResources()
{
	// We should also release the textures of each sprite here
	states.reset();
	spriteBatch.reset();
}
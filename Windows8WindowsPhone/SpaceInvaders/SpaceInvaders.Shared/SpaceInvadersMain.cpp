﻿#include "pch.h"
#include "SpaceInvadersMain.h"
#include "Common\DirectXHelper.h"

using namespace SpaceInvaders;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

SpaceInvadersMain::SpaceInvadersMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	m_deviceResources->RegisterDeviceNotify(this);
	m_sceneRenderer = std::unique_ptr<SpaceInvadersSceneRenderer>(new SpaceInvadersSceneRenderer(m_deviceResources));
	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

SpaceInvadersMain::~SpaceInvadersMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void SpaceInvadersMain::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
	/*trying to fix issue on Windows Phone http://www.catalinzima.com/2012/12/handling-orientation-in-a-windows-phone-8-game/
	auto m_window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
	auto m_windowBounds = m_deviceResources->GetOutputSize();
	if (m_window->Bounds.Width != m_windowBounds.Width ||
		m_window->Bounds.Height != m_windowBounds.Height)
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_deviceResources->GetD3DDeviceContext()->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		m_deviceResources->GetD3DDeviceContext()->Flush();
		CreateWindowSizeDependentResources();
	}*/
}

// Updates the application state once per frame.
void SpaceInvadersMain::Update() 
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRenderer->Update(m_timer);
		m_fpsTextRenderer->Update(m_timer);
	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool SpaceInvadersMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::Black);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	m_sceneRenderer->Render();
	m_fpsTextRenderer->Render();
	return true;
}

// Notifies renderers that device resources need to be released.
void SpaceInvadersMain::OnDeviceLost()
{
	m_sceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void SpaceInvadersMain::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

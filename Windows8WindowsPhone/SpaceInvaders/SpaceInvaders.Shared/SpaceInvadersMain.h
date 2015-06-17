#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\SpaceInvadersSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace SpaceInvaders
{
	class SpaceInvadersMain : public DX::IDeviceNotify
	{
	public:
		SpaceInvadersMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~SpaceInvadersMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<SpaceInvadersSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		// Rendering loop timer.
		DX::StepTimer m_timer;
	};
}
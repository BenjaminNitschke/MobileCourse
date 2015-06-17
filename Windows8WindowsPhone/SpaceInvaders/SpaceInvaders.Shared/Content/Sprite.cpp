#include "pch.h"
#include "Sprite.h"

using namespace SpaceInvaders;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;

// Remap -1 to +1 space with bottom up to normal screen coordinates (top left 0, 0)
void Sprite::Draw(std::shared_ptr<DirectX::SpriteBatch> spriteBatch, Size screenSize, float x, float y)
{
	RECT backgroundRect;
	backgroundRect.left = (int)((1 + (initialX + x - width)) * screenSize.Width / 2);
	backgroundRect.top = (int)((1 + (-initialY + y - height)) * screenSize.Height / 2);
	backgroundRect.right = (int)((1 + (initialX + x + width)) * screenSize.Width / 2);
	backgroundRect.bottom = (int)((1 + (-initialY + y + height)) * screenSize.Height / 2);
	spriteBatch->Draw(texture.Get(), backgroundRect, nullptr, Colors::White);
}
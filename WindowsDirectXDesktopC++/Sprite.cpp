#include "stdafx.h"
#include "Sprite.h"

using namespace SpaceInvaders;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Windows::Foundation;

// Remap -1 to +1 space with bottom up to normal screen coordinates (top left 0, 0)
void Sprite::Draw(std::shared_ptr<DirectX::SpriteBatch> spriteBatch, float screenWidth, float screenHeight, float x, float y)
{
	RECT backgroundRect;
	backgroundRect.left = (int)((1 + (initialX + x - width / 2)) * screenWidth / 2);
	backgroundRect.top = (int)((1+ (-initialY + y - height / 2)) * screenHeight / 2);
	backgroundRect.right = (int)((1 + (initialX + x + width / 2)) * screenWidth / 2);
	backgroundRect.bottom = (int)((1 + (-initialY + y + height / 2)) * screenHeight / 2);
	spriteBatch->Draw(texture.Get(), backgroundRect);
}
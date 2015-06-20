#include "Sprite.h"
#include <GLES/gl.h>

using namespace SpaceInvaders;

void Sprite::Draw(float x, float y)
{
	glBindTexture(GL_TEXTURE_2D, texture->GetHandle());
	glTexCoordPointer(2, GL_FLOAT, 0, uvBuffer);
	vertices[0] = initialX + x - width; vertices[1] = initialY + y + height; // top left
	vertices[3] = initialX + x - width; vertices[4] = initialY + y - height; // bottom left
	vertices[6] = initialX + x + width; vertices[7] = initialY + y - height; // bottom right
	vertices[9] = initialX + x + width; vertices[10] = initialY + y + height; // top right
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_SHORT, indexBuffer);
}
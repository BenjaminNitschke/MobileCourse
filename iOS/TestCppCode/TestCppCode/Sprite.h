#ifndef __TestCppCode__Sprite__
#define __TestCppCode__Sprite__

#include <stdio.h>
#include <memory>
#include <stdlib.h>
#include <math.h>

class Sprite {
public:
	Sprite(int texture, float x, float y, float width, float height)
		: texture(texture), initialX(x), initialY(y), width(width),
	height(height * 2.0f) {}
	void Draw(float x = 0.0f, float y = 0.0f);
	float GetWidth() { return width; }
	bool IncreaseY(float amount) {
		initialY += amount;
		return initialY > 1.0f;
	}
	float DistanceTo(std::shared_ptr<Sprite> other, float xOffset) {
		float distanceX = abs(initialX + xOffset - other->initialX);
		float distanceY = abs(initialY - other->initialY);
		return sqrt(distanceX*distanceX + distanceY*distanceY);
	}
private:
	int texture;
	float initialX;
	float initialY;
	float width;
	float height;
};

#endif /* defined(__TestCppCode__Sprite__) */

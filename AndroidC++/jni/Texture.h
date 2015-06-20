#pragma once
#include <GLES\gl.h>
#include <string>
#include <android\asset_manager.h>

namespace SpaceInvaders
{
	class Texture
	{
	public:
		Texture(AAssetManager* assetManager, std::string filename);
		class ImageNotFound : public std::exception {};
		~Texture();

		int GetHandle()
		{
			return handle;
		}

	private:
		GLuint handle;
		int width;
		int height;
	};
}
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <exception>
#include "Texture.h"

Texture::Texture()
{

	glGenTextures(1, &id);
	if (!id)
	{
		throw std::exception();
	}
	unsigned char* texture = stbi_load("Models/curuthers/Whiskers_diffuse.png", &w, &h, &channels, 4);
	if (!texture)
	{
		throw std::exception();
	}
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	free(texture);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(const char* data)
{
	
	unsigned char* texture = stbi_load(data, &w, &h, &channels, 4);
	if (!texture)
	{
		throw std::exception();
	}

	GLuint textureId = 0;

	glGenTextures(1, &id);

	if (!id)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, id);

	// Upload image data to bound texture unit in the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

	// Free the loaded data cus it has a copy on the gpu
	free(texture);

	// Generate mipmap so texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, id);
	glEnable(GL_DEPTH_TEST);


}

Texture::~Texture()
{
	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 Texture::getSize()
{
	return size;
}

GLuint Texture::getId()
{
	
	return id;
}
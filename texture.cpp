#include "texture.h"

// this is a header only library that is only used here, so include it here
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int createTexture(const std::string& path)
{
	return createTexture(path, GL_RGB);
}

unsigned int createTexture(const std::string& path, unsigned int color)
{
	// create a texture
	unsigned int texture;
	glGenTextures(1, &texture);

	// bind to the texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// setup texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filter parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load the image upside down
	stbi_set_flip_vertically_on_load(true);

	// load the image data for the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if(data)
	{
		// modifying GL_TEXTURE_2D, mipmap level, store in RGB, width, height, (always 0), source is RGB, source is UNSIGNED_BYTE
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, color, GL_UNSIGNED_BYTE, data);
		// create the MipMap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture \"" << path << "\"!" << std::endl;
		/*const unsigned char missing_tex[] = {
			255, 0, 255,   0, 0, 0,
			0, 0, 0,       255, 0, 255,
		};*/
		const unsigned char missing_tex[] = {
			0, 0, 0,     255, 0, 255,  0,0,
			255, 0, 255,     0, 0, 0,  0,0
		};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, missing_tex);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	// free the loaded image data
	stbi_image_free(data);

	return texture;
}

unsigned int createTextureOfColor(unsigned char red, unsigned char green, unsigned char blue)
{
	// create a texture
	unsigned int texture;
	glGenTextures(1, &texture);

	// bind to the texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// setup texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filter parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*const unsigned char missing_tex[] = {
		255, 0, 255,   0, 0, 0,
		0, 0, 0,       255, 0, 255,
	};*/
	const unsigned char missing_tex[] = {
		red, green, blue,     0, 0
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, missing_tex);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}
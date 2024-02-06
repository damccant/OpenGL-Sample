#include "texture.h"

// this is a header only library that is only used here, so include it here
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


unsigned int createTexture(const std::string& path)
{
	return createTexture(path, GL_RGB);
}

/**
 * Loads a texture from a file using the given color format
 * Loads a default purple/black square pattern instead if the file cannot be read
 */
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, color, GL_UNSIGNED_BYTE, data);
		// create the MipMap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture \"" << path << "\"!" << std::endl;
		#define black 0, 0, 0
		#define purple 255, 0, 255
		const unsigned char missing_tex[] = {
			black, black, purple, purple,
			black, black, purple, purple,
			purple, purple, black, black,
			purple, purple, black, black,
		};
		#undef black
		#undef purple
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, missing_tex);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	// free the loaded image data
	stbi_image_free(data);

	return texture;
}

/**
 * Creates a default yellow/black square pattern texture for objects that do not have
 * a texture assigned
 */
unsigned int createUndefinedTexture()
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

	#define black 0, 0, 0
	#define yellow 255, 255, 0
	const unsigned char missing_tex[] = {
		black, black, yellow, yellow,
		black, black, yellow, yellow,
		yellow, yellow, black, black,
		yellow, yellow, black, black,
	};
	#undef black
	#undef yellow
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, missing_tex);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	return texture;
}

/**
 * Creates a 1x1 texture of a single color
 * Used to draw sprites with defining another shader
 */
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

	const unsigned char color_tex[] = {
		red, green, blue,     0, 0
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, color_tex);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}
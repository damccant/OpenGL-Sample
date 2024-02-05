#include <string>
#include <iostream>

#include "glad/glad.h"

unsigned int createTexture(const std::string& path);
unsigned int createTexture(const std::string& path, unsigned int color);
unsigned int createTextureOfColor(unsigned char red, unsigned char green, unsigned char blue);
unsigned int createUndefinedTexture();
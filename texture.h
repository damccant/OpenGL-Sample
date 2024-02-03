#include <string>
#include "glad/glad.h"
#include <iostream>

unsigned int createTexture(const std::string& path);
unsigned int createTexture(const std::string& path, unsigned int color);
unsigned int createTextureOfColor(unsigned char red, unsigned char green, unsigned char blue);
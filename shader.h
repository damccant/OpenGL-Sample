#ifndef SHADER_H
#define SHADER_H

// glad headers don't contain this guard for some reason
#ifndef _GLAD_H_
#define _GLAD_H_
#include "glad/glad.h"
#endif /* _GLAD_H_ */

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <iomanip>
#include <iostream>

class Shader
{
public:
	unsigned int id;

	Shader();
	~Shader();

	void use(void);
	//template<class T> void setUniform(const std::string &name, T... value) const;
	void setValue(const std::string &name, bool value) const;
	void setValue(const std::string &name, int value) const;
	void setValue(const std::string &name, float value) const;
	void setValue(const std::string &name, const glm::mat4 &mat);
private:
	
};

#endif /* SHADER_H */
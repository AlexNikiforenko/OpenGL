#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
public:
	// Program ID
	unsigned int m_ID = 0;

	Shader() = default;

	// Constructor reads and build shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// use/activate shader
	void use();

	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMatrix(const std::string& name, glm::mat4 value) const;
	void setVec3f(const std::string& name, glm::vec3 value) const;
};
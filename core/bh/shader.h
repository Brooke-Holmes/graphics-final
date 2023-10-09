#pragma once
#include <sstream>
#include <fstream>
#include "../ew/ewMath/mat4.h"
namespace bh
{
	std::string loadShaderSourceFromFile(const std::string& filePath);

	class Shader {
	public:
		Shader(const std::string& vertexShader, const std::string& fragmentShader);
		void use();
		void setInt(const std::string& name, int v) const;
		void setFloat(const std::string& name, float v) const;
		void setMat4(const std::string& name, const ew::Mat4& v) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;
	private:
		unsigned int m_id; //OpenGL program handle
	};

	void bh::Shader::setVec2(const std::string& name, float x, float y) const
	{
		GLint location = glGetUniformLocation(m_id, name.c_str());
		glUseProgram(m_id);
		glUniform2f(location, x, y);
	}

	inline void bh::Shader::setVec3(const std::string& name, float x, float y, float z) const
	{
		GLint location = glGetUniformLocation(m_id, name.c_str());
		glUseProgram(m_id);
		glUniform3f(location, x, y, z);
	}

	inline void bh::Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		GLint location = glGetUniformLocation(m_id, name.c_str());
		glUseProgram(m_id);
		glUniform4f(location, x, y, z, w);
	}
}

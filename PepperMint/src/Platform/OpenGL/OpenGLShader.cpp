#include "pmpch.h"

#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace PepperMint {

OpenGLShader::OpenGLShader(const std::string& iVertexShader, const std::string& iFragmentShader) {

	// Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = iVertexShader.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertexShader);

		PM_CORE_ERROR("{0}", infoLog.data());
		PM_CORE_ASSERT(false, "Vertex shader compilation failure!");
		return;
	}

	// Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = iFragmentShader.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		PM_CORE_ERROR("{0}", infoLog.data());
		PM_CORE_ASSERT(false, "Fragment shader compilation failure!");
		return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	_rendererId = glCreateProgram();
	GLuint program = _rendererId;

	// Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		PM_CORE_ERROR("{0}", infoLog.data());
		PM_CORE_ASSERT(false, "Shader link failure!");
		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
}

OpenGLShader::~OpenGLShader() {
	glDeleteProgram(_rendererId);
}

void OpenGLShader::bind() const {
	glUseProgram(_rendererId);
}

void OpenGLShader::unbind() const {
	glUseProgram(0);
}

void OpenGLShader::uploadUniformInt(const std::string& iName, int iValue) {
	GLint location = glGetUniformLocation(_rendererId, iName.c_str());
	glUniform1i(location, iValue);
}

void OpenGLShader::uploadUniformFloat(const std::string& iName, float iValue) {
	GLint location = glGetUniformLocation(_rendererId, iName.c_str());
	glUniform1f(location, iValue);
}

void OpenGLShader::uploadUniformFloat2(const std::string& iName, const glm::vec2& iValue) {
	GLint location = glGetUniformLocation(_rendererId, iName.c_str());
	glUniform2f(location, iValue.x, iValue.y);
}

void OpenGLShader::uploadUniformFloat3(const std::string& iName, const glm::vec3& iValue) {
	GLint location = glGetUniformLocation(_rendererId, iName.c_str());
	glUniform3f(location, iValue.x, iValue.y, iValue.z);
}

void OpenGLShader::uploadUniformFloat4(const std::string& iName, const glm::vec4& iValue) {
	GLint location = glGetUniformLocation(_rendererId, iName.c_str());
	glUniform4f(location, iValue.x, iValue.y, iValue.z, iValue.w);
}

void OpenGLShader::uploadUniformMat3(const std::string& iName, const glm::mat3& iValue) {
	GLint location = glGetUniformLocation(_rendererId, iName.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(iValue));
}

void OpenGLShader::uploadUniformMat4(const std::string& iName, const glm::mat4& iValue) {
	GLint location = glGetUniformLocation(_rendererId, iName.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(iValue));
}
}
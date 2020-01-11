#include "pmpch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace PepperMint {

namespace {

GLenum shaderTypeFromString(const std::string& iType) {
	if (iType == "vertex") {
		return GL_VERTEX_SHADER;
	} else if (iType == "fragment" || iType == "pixel") {
		return GL_FRAGMENT_SHADER;
	}

	PM_CORE_ASSERT(false, "Unknown shader type");
	return 0;
}
}

OpenGLShader::OpenGLShader(const std::string& iShaderFile) {
	const std::string& source = readFile(iShaderFile);
	const auto& shaderSources = preProcess(source);
	compile(shaderSources);

	auto lastSlash = iShaderFile.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	auto lastDot = iShaderFile.rfind('.');
	auto count = lastDot == std::string::npos ? iShaderFile.size() - lastSlash : lastDot - lastSlash;
	_name = iShaderFile.substr(lastSlash, count);
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

void OpenGLShader::setInt(const std::string& iName, int iValue) {
	uploadUniformInt(iName, iValue);
}

void OpenGLShader::setFloat(const std::string& iName, float iValue) {
	uploadUniformFloat(iName, iValue);
}

void OpenGLShader::setFloat2(const std::string& iName, const glm::vec2& iValue) {
	uploadUniformFloat2(iName, iValue);
}

void OpenGLShader::setFloat3(const std::string& iName, const glm::vec3& iValue) {
	uploadUniformFloat3(iName, iValue);
}

void OpenGLShader::setFloat4(const std::string& iName, const glm::vec4& iValue) {
	uploadUniformFloat4(iName, iValue);
}

void OpenGLShader::setMat3(const std::string& iName, const glm::mat3& iValue) {
	uploadUniformMat3(iName, iValue);
}

void OpenGLShader::setMat4(const std::string& iName, const glm::mat4& iValue) {
	uploadUniformMat4(iName, iValue);
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

std::string OpenGLShader::readFile(const std::string& iShaderFile) {
	std::string result;

	std::ifstream in(iShaderFile, std::ios::in | std::ios::binary);
	if (in) {
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	} else {
		PM_CORE_ERROR("Could not open file '{0}'", iShaderFile);
	}

	return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& iSource) {
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = iSource.find(typeToken, 0); // Start of shader type declaration line

	while (pos != std::string::npos) {
		size_t eol = iSource.find_first_of("\r\n", pos); // End of shader type declaration line
		PM_CORE_ASSERT(eol != std::string::npos, "Syntax error");

		size_t begin = pos + typeTokenLength + 1; // Start of shader type name (after "#type " keyword)
		const std::string& type = iSource.substr(begin, eol - begin);
		PM_CORE_ASSERT(shaderTypeFromString(type), "Invalid shader type specified");

		size_t nextLinePos = iSource.find_first_not_of("\r\n", eol);  // Start of shader code after shader type declaration line
		PM_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
		pos = iSource.find(typeToken, nextLinePos); // Start of next shader type declaration line

		shaderSources[shaderTypeFromString(type)] = (pos == std::string::npos) ? iSource.substr(nextLinePos) 
																			   : iSource.substr(nextLinePos, pos - nextLinePos);
	}

	return shaderSources;
}

void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& iShaderSources) {
	GLuint program = glCreateProgram();

	PM_CORE_ASSERT(iShaderSources.size() <= 2, "We only support 2 shaders for now");
	std::array<GLenum, 2> glShaderIds;
	int glShaderIdsIdx = 0;

	for (auto&& shaderSource : iShaderSources) {
		GLenum type = shaderSource.first;
		const std::string& source = shaderSource.second;

		GLuint shader = glCreateShader(type);

		const GLchar* source_cstr = source.c_str();
		glShaderSource(shader, 1, &source_cstr, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			PM_CORE_ERROR("{0}", infoLog.data());
			PM_CORE_ASSERT(false, "Shader compilation failure!");
			break;
		}

		glAttachShader(program, shader);
		glShaderIds[glShaderIdsIdx++] = shader;
	}

	_rendererId = program;

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(program);

		for (auto&& shaderId : glShaderIds) {
			glDeleteShader(shaderId);
		}

		PM_CORE_ERROR("{0}", infoLog.data());
		PM_CORE_ASSERT(false, "Shader link failure!");
		return;
	}

	for (auto&& shaderId : glShaderIds) {
		glDetachShader(program, shaderId);
		glDeleteShader(shaderId);
	}
}
}
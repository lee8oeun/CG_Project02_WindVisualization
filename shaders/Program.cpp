#include <stdexcept>
#include <glm\gtc\type_ptr.hpp>
#include <limits>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Program.h"


//fix is to use smart pointers later
std::unique_ptr<Shader> Shader::GenerateFromSource(std::string name, GLenum shaderType, const std::string& source)
{
	// Read the file
	const char* sourceChar = source.c_str();
	const GLint sourceSize = static_cast<GLint>(source.size());

		
	// Create and set the shader source
	GLuint shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &sourceChar, &sourceSize);
	glCompileShader(shaderID);

	// Check for errors
	int compile_result = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);

	if (compile_result == GL_FALSE)
		Logging::LogError(GetShaderErrorLog(name, shaderID, shaderType));

	return std::make_unique<Shader>(name, shaderID);
}

std::unique_ptr<Shader> Shader::GenerateFromFilename(GLenum shaderType, const std::string& filepath)
{
	// Read contents of file
	std::ifstream file(filepath, std::ios::in);
	std::string fileContents;

	// Ensure the file is open and readable
	if (!file.good())
		Logging::LogError("Cannot find file at path: '{0}'", filepath);

	// Determine the size of of the file in characters and resize the string variable to accomodate
	file.seekg(0, std::ios::end);
	fileContents.resize((unsigned int)file.tellg());

	// Set the position of the next character to be read back to the beginning
	file.seekg(0, std::ios::beg);

	// Extract the contents of the file and store in the string variable
	file.read(&fileContents[0], fileContents.size());
	file.close();

	return GenerateFromSource(filepath, shaderType, fileContents);
}

std::string Shader::GetShaderErrorLog(const std::string& name, GLuint shaderID, GLenum shaderType)
{
	int infoLogLength;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	std::string shaderTypeStr = "Unknown";

	if (shaderType == GL_VERTEX_SHADER)
		shaderTypeStr = "Vertex Shader";
	else if (shaderType == GL_FRAGMENT_SHADER)
		shaderTypeStr = "Fragment Shader";
	else if (shaderType == GL_COMPUTE_SHADER)
		shaderTypeStr = "Compute Shader";

	if (infoLogLength != 0)
	{
		// Retrieve the log info and populate log variable
		std::vector<char> log(infoLogLength);
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, &log[0]);

		std::string errorLog(log.begin(), log.end());
		return "Error compiling " + name + " " + shaderTypeStr + ":\n" + errorLog;
	}
	else
	{
		return "Unknown Error compiling " + name + " " + shaderTypeStr;
	}
}

Shader::Shader(std::string name, GLuint shaderID)
{
	ShaderID = shaderID;
	Name = name;
}

Shader::~Shader()
{
	glDeleteShader(ShaderID);
}




Program::Program(GLuint programID)
{
	ProgramID = programID;
	IsBound = false;
}

Program::~Program()
{
	glDeleteProgram(ProgramID);
}

void Program::BindProgram()
{
	glUseProgram(ProgramID);
	IsBound = true;
}

void Program::UnbindProgram()
{
	for (int i = 0; i < TextureCount; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glUseProgram(0);
	IsBound = false;
	TextureCount = 0;
}

GLuint Program::GetUniformID(const std::string& name)
{
	GLuint id = glGetUniformLocation(ProgramID, name.c_str());

	if (id == UINT_MAX)
		Logging::LogError("No uniform ID exists under the name '{0}'", name); // Invalid ID

	return id;
}

GLuint Program::GetProgramID()
{
	return ProgramID;
}

void Program::SetBool(GLuint id, bool value)
{
	ValidateSetUniform(id);
	glUniform1i(id, value ? 1 : 0);
}

void Program::SetBool(const std::string& name, bool value)
{
	ValidateSetUniform(name);
	glUniform1i(GetUniformID(name), value ? 1 : 0);
}

void Program::SetFloat(GLuint id, float value)
{
	ValidateSetUniform(id);
	glUniform1f(id, value);
}

void Program::SetInt(GLuint id, int value)
{
	ValidateSetUniform(id);
	glUniform1i(id, value);
}

void Program::SetFloat(const std::string& name, float value)
{
	ValidateSetUniform(name);
	SetFloat(GetUniformID(name), value);
}

void Program::SetInt(const std::string& name, int value)
{
	ValidateSetUniform(name);
	SetInt(GetUniformID(name), value);
}

void Program::SetVector(GLuint id, glm::vec2 value)
{
	ValidateSetUniform(id);
	glUniform2f(id, value.x, value.y);
}

void Program::SetVector(GLuint id, glm::vec3 value)
{
	ValidateSetUniform(id);
	glUniform3f(id, value.x, value.y, value.z);
}

void Program::SetVector(GLuint id, glm::vec4 value)
{
	ValidateSetUniform(id);
	glUniform4f(id, value.x, value.y, value.z, value.w);
}

void Program::SetVector(const std::string& name, glm::vec2 value)
{
	ValidateSetUniform(name);
	SetVector(GetUniformID(name), value);
}

void Program::SetVector(const std::string& name, glm::vec3 value)
{
	ValidateSetUniform(name);
	SetVector(GetUniformID(name), value);
}

void Program::SetVector(const std::string& name, glm::vec4 value)
{
	ValidateSetUniform(name);
	SetVector(GetUniformID(name), value);
}

void Program::SetIVector(GLuint id, glm::ivec2 value)
{
	ValidateSetUniform(id);
	glUniform2i(id, value.x, value.y);
}

void Program::SetIVector(GLuint id, glm::ivec3 value)
{
	ValidateSetUniform(id);
	glUniform3i(id, value.x, value.y, value.z);
}

void Program::SetIVector(GLuint id, glm::ivec4 value)
{
	ValidateSetUniform(id);
	glUniform4i(id, value.x, value.y, value.z, value.w);
}

void Program::SetIVector(const std::string& name, glm::ivec2 value)
{
	ValidateSetUniform(name);
	SetIVector(GetUniformID(name), value);
}

void Program::SetIVector(const std::string& name, glm::ivec3 value)
{
	ValidateSetUniform(name);
	SetIVector(GetUniformID(name), value);
}

void Program::SetIVector(const std::string& name, glm::ivec4 value)
{
	ValidateSetUniform(name);
	SetIVector(GetUniformID(name), value);
}

void Program::SetMatrix(GLuint id, const glm::mat3x3& value)
{
	ValidateSetUniform(id);
	glUniformMatrix3fv(id, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetMatrix(GLuint id, const glm::mat4x4& value)
{
	ValidateSetUniform(id);
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetMatrix(const std::string& name, const glm::mat3x3& value)
{
	ValidateSetUniform(name);
	SetMatrix(GetUniformID(name), value);
}

void Program::SetMatrix(const std::string& name, const glm::mat4x4& value)
{
	ValidateSetUniform(name);
	SetMatrix(GetUniformID(name), value);
}

void Program::SetTexture(GLuint id, GLuint textureID)
{
	ValidateSetUniform(id);

	glActiveTexture(GL_TEXTURE0 + TextureCount);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glUniform1i(id, TextureCount);
	TextureCount++;
}

void Program::SetTexture(const std::string& name, GLuint textureID)
{
	ValidateSetUniform(name);

	glActiveTexture(GL_TEXTURE0 + TextureCount);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glUniform1i(GetUniformID(name), TextureCount);
	TextureCount++;
}

void Program::ValidateSetUniform(GLuint id)
{
	if (!IsBound)
		Logging::ThrowError("Trying to set Program variable while unbound");
}

void Program::ValidateSetUniform(const std::string& name)
{
	if (!IsBound)
		Logging::ThrowError("Trying to set Program variable while unbound");
//fix is to use smart pointers later
}


std::unique_ptr<Program> Program::GenerateFromFileVsFs(const std::string& filepathVs, const std::string& filepathFs)
{
	// Create shaders using smart pointers
	auto vsShader = std::unique_ptr<Shader>(Shader::GenerateFromFilename(GL_VERTEX_SHADER, filepathVs));
	auto fsShader = std::unique_ptr<Shader>(Shader::GenerateFromFilename(GL_FRAGMENT_SHADER, filepathFs));

	// Move shaders into vector
	std::vector<std::unique_ptr<Shader>> shaders;
	shaders.push_back(std::move(vsShader));
	shaders.push_back(std::move(fsShader));

	return GenerateProgram(std::move(shaders));
}


std::unique_ptr<Program> Program::GenerateFromFileCs(const std::string& filepathCs)
{
	// Create shader using smart pointer
	auto csShader = std::unique_ptr<Shader>(Shader::GenerateFromFilename(GL_COMPUTE_SHADER, filepathCs));

	// Move shader into vector
	std::vector<std::unique_ptr<Shader>> shaders;
	shaders.push_back(std::move(csShader));

	return GenerateProgram(std::move(shaders));
}

std::unique_ptr<Program> Program::GenerateProgram(std::vector<std::unique_ptr<Shader>> shaders)
{
	// Generate and attach the shaders to the program
	GLuint programID = glCreateProgram();

	for (const auto& shader : shaders)
		glAttachShader(programID, shader->ShaderID);

	glLinkProgram(programID);

	// Check for link errors
	int link_result = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &link_result);

	if (link_result == GL_FALSE)
		Logging::ThrowError(GetProgramErrorLog(shaders, programID).c_str());

	// Shaders are automatically cleaned up when they go out of scope
	return std::make_unique<Program>(programID);
}


std::string Program::GetProgramErrorLog(const std::vector<std::unique_ptr<Shader>>& shaders, GLuint programID)
{
	std::string name = shaders.size() > 0 ? shaders[0]->Name : "";

	for (size_t i = 1; i < shaders.size(); ++i)
		name += ", " + shaders[i]->Name;

	int infoLogLength;
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength != 0)
	{
		// Retrieve the log info and populate log variable
		std::vector<char> log(infoLogLength);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &log[0]);

		std::string errorLog(log.begin(), log.end());
		return "Program Error Linking Shaders: " + name + "\n" + errorLog;
	}
	else
	{
		return "Program Unknown Error Linking Shaders: " + name;
	}
}
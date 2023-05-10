#pragma once

#include <string>
#include <unordered_map>

class Shader
{
public:
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};
private:
	unsigned int RendererID;
	std::string shaderFilepath;
	ShaderProgramSource src;
	//Cache
	std::unordered_map<std::string, int> UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	//Uniforms
public:
	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
private:
	int GetUniformLocation(const std::string& name);
};


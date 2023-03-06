#include "ShaderTest.h"

ShaderTest::ShaderTest(const std::string& id, const std::string& v_str, const std::string& f_str) : id(id), img(NULL)
{
	v = GPU_LoadShader(GPU_VERTEX_SHADER, v_str.c_str());

	if (!v)
	std::cout << "Failed to load vertex shader: " << GPU_GetShaderMessage() << "\n";

	f = GPU_LoadShader(GPU_FRAGMENT_SHADER, f_str.c_str());

	if (!f)
	std::cout << "Failed to load fragment shader: " << GPU_GetShaderMessage() << "\n";

	p = GPU_LinkShaders(v, f);

	if (!p)
	std::cout << "Failed to link shader program: " << GPU_GetShaderMessage() << "\n";

	block = GPU_LoadShaderBlock(p, "gpu_Vertex", "gpu_TexCoord", NULL, "gpu_ModelViewProjectionMatrix");
}

void ShaderTest::freeImg() {
	GPU_FreeImage(img);
}

void ShaderTest::addVariable(std::string idV) {
	Uint32 location = GPU_GetUniformLocation(p, idV.c_str());
	variables.push_back(std::make_pair(idV, location));
}

Uint32 ShaderTest::getVar(std::string idV) {
	auto it = std::find_if(variables.begin(), variables.end(), [idV](std::pair<std::string, Uint32> p) { return p.first == idV; });

	if (it != variables.end())
		return it->second;

	return (Uint32)(-1);
}

void ShaderTest::addImg(std::string path) {
	img = GPU_LoadImage(path.c_str());

	GPU_SetSnapMode(img, GPU_SNAP_NONE);
	GPU_SetWrapMode(img, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
}

void ShaderTest::setImgShader() {
	GPU_SetShaderImage(img, getVar("tex1"), 1);
}

void ShaderTest::activate() {
	GPU_ActivateShaderProgram(p, &block);
}
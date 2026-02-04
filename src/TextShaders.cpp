#include "TextShaders.hpp"

using namespace geode::prelude;

std::string DeltaruneTextShaders::getFragmentShaderLog(CCGLProgram* program) {
	GLint length, written;
	
	glGetShaderiv(program->m_uFragShader, GL_INFO_LOG_LENGTH, &length);
	log::info("Length: {}", length);
	
	if (length <= 0) return "";
	std::string result;
	result.resize(length);
	glGetShaderInfoLog(program->m_uFragShader, length, &written, result.data());

	return result;
}

CCGLProgram* DeltaruneTextShaders::getShader(bool const noShadow, bool const noGradient) {
    auto ccsc = CCShaderCache::sharedShaderCache();
    auto program = ccsc->programForKey("text-shader"_spr);

    if (program) {
		program->updateUniforms();
		program->setUniformLocationWith1i(program->getUniformLocationForName("u_noShadow"), static_cast<int>(noShadow));
    	program->setUniformLocationWith1i(program->getUniformLocationForName("u_noGradient"), static_cast<int>(noGradient));
		
		return program;
	}

    program = new CCGLProgram();
    bool res = program->initWithVertexShaderFilename("default.vsh"_spr, "textShader.fsh"_spr);

	if (!res)
		log::error("GLSL Error: \n{}", getFragmentShaderLog(program));
	
    program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	
    if (!program->link()) {
		log::error("Shader link failed!");
        program->release();
        return nullptr;
    }
	
    program->updateUniforms();
	
	auto const director = CCDirector::get();
	auto const pixelSize = director->getWinSizeInPixels();
	float const pixelWidth = pixelSize.width;
	float const pixelHeight = pixelSize.height;
	
	auto const winSize = director->getWinSize();
	float constexpr clippingNodeHeight = 120;
	
    program->setUniformLocationWith1i(program->getUniformLocationForName("u_noShadow"), static_cast<int>(noShadow));
    program->setUniformLocationWith1i(program->getUniformLocationForName("u_noGradient"), static_cast<int>(noGradient));
	program->setUniformLocationWith2f(program->getUniformLocationForName("u_texelSize"), 1.f / pixelWidth, 1.f / pixelHeight);
	program->setUniformLocationWith1f(program->getUniformLocationForName("u_yTop"), 1.f - clippingNodeHeight / winSize.height);
    ccsc->addProgram(program, "text-shader"_spr);
    
	program->release();

	return program;
}
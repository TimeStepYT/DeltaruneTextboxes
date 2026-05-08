#include "TextShaders.hpp"
#include "Global.hpp"

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

DeltaruneTextShaders DeltaruneTextShaders::create(cocos2d::CCNode* border) {
	DeltaruneTextShaders instance;
	instance.m_borderNode = border;

	return instance;
}

// void DeltaruneTextShaders::updateLastShader() {
//     auto ccsc = CCShaderCache::sharedShaderCache();
// 	auto program = ccsc->programForKey("text-shader"_spr);

// 	log::debug("Debug value: {}", global::debugVal);

//     if (!program)
// 		return;
	
// 	program->updateUniforms();
// 	program->setUniformLocationWith1i(program->getUniformLocationForName("u_debugVal"), global::debugVal);
// }

void DeltaruneTextShaders::setShadowDistance(CCGLProgram* program) {
	auto director = CCDirector::get();

	auto const pixelSize = this->m_borderNode->getContentSize();
	float const texelWidth = 1.f / (pixelSize.width * 3);
	float const texelHeight = 1.f / (pixelSize.height * 3);
	
	auto textureQuality = director->getLoadedTextureQuality();
	float qualityCompensation = 1.f;

	if (textureQuality == cocos2d::kTextureQualityMedium) {
		qualityCompensation = 1.5f;
	}
	else if (textureQuality == cocos2d::kTextureQualityLow) {
		qualityCompensation = 2.f;
	}

	float const shadowDistance = 2.f;
	program->setUniformLocationWith2f(program->getUniformLocationForName("u_shadowDistance"), shadowDistance * texelWidth * qualityCompensation, shadowDistance * texelHeight * qualityCompensation);
}

CCGLProgram* DeltaruneTextShaders::getShader(bool const noShadow, bool const noGradient) {
    auto ccsc = CCShaderCache::sharedShaderCache();
    auto program = ccsc->programForKey("text-shader"_spr);

    if (program) {
		program->updateUniforms();
		this->setShadowDistance(program);
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
	
	auto const pixelSize = this->m_borderNode->getContentSize();
	float const texelHeight = 1.f / (pixelSize.height * 3);

	this->setShadowDistance(program);
	
    program->setUniformLocationWith1i(program->getUniformLocationForName("u_noShadow"), static_cast<int>(noShadow));
    program->setUniformLocationWith1i(program->getUniformLocationForName("u_noGradient"), static_cast<int>(noGradient));
    program->setUniformLocationWith1f(program->getUniformLocationForName("u_yTop"), 40.f * texelHeight);
    // program->setUniformLocationWith1i(program->getUniformLocationForName("u_debugVal"), global::debugVal);
    ccsc->addProgram(program, "text-shader"_spr);
    
	program->release();

	return program;
}
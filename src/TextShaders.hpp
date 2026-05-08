#pragma once

#include <Geode/Geode.hpp>
class DeltaruneTextShaders {
private:
	cocos2d::CCNode* m_borderNode = nullptr;	
	void setShadowDistance(cocos2d::CCGLProgram* program);

public:
	// static void updateLastShader();

	static DeltaruneTextShaders create(cocos2d::CCNode* border);
	cocos2d::CCGLProgram* getShader(bool const noShadow, bool const noGradient);
	static std::string getFragmentShaderLog(cocos2d::CCGLProgram* program);
};
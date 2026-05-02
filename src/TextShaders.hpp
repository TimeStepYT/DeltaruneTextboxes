#include <Geode/Geode.hpp>

class DeltaruneTextShaders {
private:
	cocos2d::CCNode* m_borderNode = nullptr;	

public:
	static DeltaruneTextShaders create(cocos2d::CCNode* border);
	cocos2d::CCGLProgram* getShader(bool const noShadow, bool const noGradient);
	static std::string getFragmentShaderLog(cocos2d::CCGLProgram* program);
};
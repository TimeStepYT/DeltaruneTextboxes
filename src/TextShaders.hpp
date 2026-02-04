#include <Geode/Geode.hpp>

class DeltaruneTextShaders {
	public:
	static cocos2d::CCGLProgram* getShader(bool const noShadow, bool const noGradient);
	static std::string getFragmentShaderLog(cocos2d::CCGLProgram* program);
};
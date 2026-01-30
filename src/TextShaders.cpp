#include "TextShaders.hpp"

using namespace geode::prelude;

static constexpr auto VERTEX_SHADER = R"(
		attribute vec4 a_position;
		attribute vec4 a_color;
		attribute vec2 a_texCoord;

		#ifdef GL_ES
		varying lowp vec4 v_fragmentColor;
		varying mediump vec2 v_texCoord;
		#else
		varying vec4 v_fragmentColor;
		varying vec2 v_texCoord;
		#endif

		void main() {
			gl_Position = CC_MVPMatrix * a_position;
			v_fragmentColor = a_color;
			v_texCoord = a_texCoord;
		}
	)";

static constexpr auto FRAGMENT_SHADER = R"(
		#ifdef GL_ES
		precision mediump float;
		#endif

		varying vec4 v_fragmentColor;
		varying vec2 v_texCoord;
		uniform sampler2D u_texture;

		uniform bool u_noGradient;
		uniform bool u_noShadow;

		void main(void) {
			vec4 color = texture2D(u_texture, v_texCoord);
			vec3 tintedColor = v_fragmentColor.rgb * color.rgb;
			gl_FragColor = vec4(tintedColor.rgb, color.a * v_fragmentColor.a);
		}
	)";

CCGLProgram* DeltaruneTextShaders::getShader(bool const noShadow, bool const noGradient) {
    auto ccsc = CCShaderCache::sharedShaderCache();
    auto program = ccsc->programForKey("text-shader"_spr);

    if (program)
        return program;

    program = new CCGLProgram();
    program->initWithVertexShaderByteArray(VERTEX_SHADER, FRAGMENT_SHADER);
    program->addAttribute("a_position", kCCVertexAttrib_Position);
    program->addAttribute("a_texCoord", kCCVertexAttrib_TexCoords);
    program->addAttribute("a_color", kCCVertexAttrib_Color);

    if (!program->link()) {
        log::error("Shader link failed!");
        program->release();
        return nullptr;
    }

    program->updateUniforms();

    program->setUniformLocationWith1i(program->getUniformLocationForName("u_noShadow"), static_cast<int>(noShadow));
    program->setUniformLocationWith1i(program->getUniformLocationForName("u_noGradient"), static_cast<int>(noGradient));

    ccsc->addProgram(program, "text-shader"_spr);
    program->release();

    return program;
}
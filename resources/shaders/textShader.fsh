#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D u_texture;

uniform bool u_noGradient;
uniform bool u_noShadow;
uniform vec2 u_shadowDistance;
uniform float u_yTop;

float invert(float val) {
    return 1.0 - val;
}

vec3 rgbToDec(float r, float g, float b) {
    return vec3(r / 255.0, g / 255.0, b / 255.0);
}

vec3 getShadowColor(vec4 shadowSourceColor) {
    vec3 shadowColor = vec3(0, 0, 0);

    // I am so sorry for what you're about to see

    if (shadowSourceColor.r == shadowSourceColor.g && shadowSourceColor.g == shadowSourceColor.b && shadowSourceColor.r != 0)
        shadowColor = rgbToDec(15.0, 15.0, 127.0);
    else if (shadowSourceColor.rgb == vec3(1.0, 0.0, 1.0))
        shadowColor = rgbToDec(76.0, 0.0, 76.0);
    else if (shadowSourceColor.rgb == rgbToDec(255, 90, 90))
        shadowColor = rgbToDec(76.0, 0.0, 0.0);
    else if (shadowSourceColor.rgb == rgbToDec(255, 165, 75))
        shadowColor = rgbToDec(76.0, 38.0, 18.0);
    else if (shadowSourceColor.rgb == vec3(1.0, 1.0, 0.0))
        shadowColor = rgbToDec(76.0, 76.0, 0.0);
    else if (shadowSourceColor.rgb == rgbToDec(64, 227, 72))
        shadowColor = rgbToDec(0.0, 76.0, 0.0);
    else if (shadowSourceColor.rgb == rgbToDec(74, 82, 255))
        shadowColor = rgbToDec(0.0, 0.0, 76.0);
    else
        shadowColor = vec3(shadowSourceColor.r / 5.0, shadowSourceColor.g / 5.0, shadowSourceColor.b / 5.0);

    return shadowColor;
}


float getGradientBrightness(float y) {
    const float margin = 0.12;
    const float padding = 0.2;
    const int segments = 3;
    float brightness = 0.49;

    const float gradientEffectHeight = 1.0 - (margin * 2.0);
    const float segmentHeight = gradientEffectHeight / segments;

    float ySegment = y - margin;

    while (ySegment > segmentHeight) {
        ySegment -= segmentHeight;
    }

    ySegment /= segmentHeight;

    
    float ySegmentScaled = ySegment * (1.0 + padding) - padding / 2;
    
    float invertedGradientBrightness = ySegmentScaled;
    float gradientBrightness = invert(invertedGradientBrightness) * brightness;
    
    return gradientBrightness;
}

void main(void) {
    if (v_texCoord.y < u_yTop) {
        gl_FragColor = vec4(0, 0, 0, 0);
        return;
    }

    vec4 color = texture2D(u_texture, v_texCoord);
    vec3 tintedColor = v_fragmentColor.rgb * color.rgb;

    vec4 resColor = vec4(tintedColor.rgb, color.a);

    if (u_noShadow == 0) {
        vec2 shadowSourceCoord = vec2(v_texCoord.x - u_shadowDistance.x, v_texCoord.y - u_shadowDistance.y);
        vec4 shadowSourceColor = texture2D(u_texture, shadowSourceCoord);
        
        if (shadowSourceCoord.x < 0 || shadowSourceCoord.y < 0) {
            shadowSourceColor = vec4(0, 0, 0, 0);
        }
        vec3 shadowColor = getShadowColor(shadowSourceColor);
        
        resColor.r = resColor.r + shadowColor.r * shadowSourceColor.a * invert(color.a);
        resColor.g = resColor.g + shadowColor.g * shadowSourceColor.a * invert(color.a);
        resColor.b = resColor.b + shadowColor.b * shadowSourceColor.a * invert(color.a);
        resColor.a = max(color.a, shadowSourceColor.a);
    }
    else {
        resColor.a = color.a;
    }

    if (u_noGradient == 0) {
        vec2 gradientBrightness = getGradientBrightness(v_texCoord.y);
        float gradientFactor = gradientBrightness;

        vec3 gradientColor = vec3(0, 0, 0);

        gradientColor.r = tintedColor.r + gradientFactor;
        gradientColor.g = tintedColor.g + gradientFactor;
        gradientColor.b = tintedColor.b + gradientFactor;

        resColor.r = resColor.r * invert(color.a) + gradientColor.r * color.a;
        resColor.g = resColor.g * invert(color.a) + gradientColor.g * color.a;
        resColor.b = resColor.b * invert(color.a) + gradientColor.b * color.a;
    }
    
    gl_FragColor = vec4(resColor.rgb, resColor.a * v_fragmentColor.a);
}
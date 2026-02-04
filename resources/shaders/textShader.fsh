#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D u_texture;

uniform bool u_noGradient;
uniform int u_noShadow;
uniform vec2 u_texelSize;
uniform float u_yTop;

vec3 rgbToDec(float r, float g, float b) {
    return vec3(r / 255.0, g / 255.0, b / 255.0);
}

vec3 getShadowColor(vec4 shadowSourceColor) {
    vec3 shadowColor = vec3(0, 0, 0);

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

void main(void) {
    vec2 shadowDistance = vec2(3.0 * u_texelSize.x, 3.0 * u_texelSize.y);
    vec2 shadowSourceCoord = vec2(v_texCoord.x - shadowDistance.x, v_texCoord.y - shadowDistance.y);

    
    vec4 color = texture2D(u_texture, v_texCoord);
    vec3 tintedColor = v_fragmentColor.rgb * color.rgb;
    
    vec4 resColor = vec4(0, 0, 0, 0);
    
    resColor = vec4(tintedColor.rgb, color.a);
    
    if (u_noShadow == 0) {
        vec4 shadowSourceColor = texture2D(u_texture, shadowSourceCoord);
        
        if (shadowSourceCoord.x < 0 || shadowSourceCoord.y < 0) {
            shadowSourceColor = vec4(0, 0, 0, 0);
        }
        vec3 shadowColor = getShadowColor(shadowSourceColor);
        
        resColor.r = resColor.r + shadowColor.r * shadowSourceColor.a * abs(color.a - 1.0);
        resColor.g = resColor.g + shadowColor.g * shadowSourceColor.a * abs(color.a - 1.0);
        resColor.b = resColor.b + shadowColor.b * shadowSourceColor.a * abs(color.a - 1.0);
        resColor.a = max(color.a, shadowSourceColor.a);
    }
    else {
        resColor.a = color.a;
    }
    
    gl_FragColor = vec4(resColor.rgb, resColor.a * v_fragmentColor.a);
}
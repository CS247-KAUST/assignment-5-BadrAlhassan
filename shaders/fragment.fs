#version 450

in vec2 texCoord;

uniform sampler2D txtr;
uniform vec4 vertexColor;

uniform int colormapMode;
uniform float blendFactor;

out vec4 fragColor;

vec3 rainbow(float t) {
    vec3 blue   = vec3(0.0, 0.0, 1.0);
    vec3 cyan   = vec3(0.0, 1.0, 1.0);
    vec3 green  = vec3(0.0, 1.0, 0.0);
    vec3 yellow = vec3(1.0, 1.0, 0.0);
    vec3 red    = vec3(1.0, 0.0, 0.0);

    t = clamp(t, 0.0, 1.0);
    if (t < 0.25) return mix(blue,   cyan,   t / 0.25);
    if (t < 0.50) return mix(cyan,   green,  (t - 0.25) / 0.25);
    if (t < 0.75) return mix(green,  yellow, (t - 0.50) / 0.25);
    return            mix(yellow, red,    (t - 0.75) / 0.25);
}

vec3 coolwarm(float t) {
    vec3 blue  = vec3(0.231, 0.298, 0.753);
    vec3 white = vec3(0.865, 0.865, 0.865);
    vec3 red   = vec3(0.706, 0.016, 0.150);

    t = clamp(t, 0.0, 1.0);
    if (t < 0.5) return mix(blue, white, t / 0.5);
    return            mix(white, red,  (t - 0.5) / 0.5);
}

void main() {
    vec4 tex = texture(txtr, texCoord);
    float val = tex.r;

    vec3 color = vec3(val);
    if (colormapMode == 1) {
        vec3 mapped = rainbow(val);
        color = mix(vec3(val), mapped, blendFactor);
    } else if (colormapMode == 2) {
        vec3 mapped = coolwarm(val);
        color = mix(vec3(val), mapped, blendFactor);
    }

    fragColor = vertexColor + vec4(color, 1.0);
}

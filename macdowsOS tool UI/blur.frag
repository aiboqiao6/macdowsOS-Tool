uniform sampler2D u_texture;
uniform vec2 u_resolution;   // 当前纹理分辨率
uniform float u_radius;      // 模糊半径

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution;
    // 五点采样高斯核示例（可扩展）
    float offset = u_radius / u_resolution.x;
    vec4 sum = texture(u_texture, uv) * 4.0;
    sum += texture(u_texture, uv + vec2(-offset, 0)) * 2.0;
    sum += texture(u_texture, uv + vec2(offset, 0)) * 2.0;
    sum += texture(u_texture, uv + vec2(0, -offset)) * 2.0;
    sum += texture(u_texture, uv + vec2(0, offset)) * 2.0;
    sum += texture(u_texture, uv + vec2(-offset, -offset));
    sum += texture(u_texture, uv + vec2(-offset, offset));
    sum += texture(u_texture, uv + vec2(offset, -offset));
    sum += texture(u_texture, uv + vec2(offset, offset));
    gl_FragColor = sum / 16.0;
}
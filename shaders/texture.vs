attribute vec4 a_position;
attribute vec2 a_texCoord;

varying vec2 v_texCoord;

void main() {
    gl_Position = a_position;
    // Just pass through texture coordinates normally - no scaling
    v_texCoord = a_texCoord;
}

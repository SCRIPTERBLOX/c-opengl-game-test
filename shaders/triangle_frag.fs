precision mediump float;
varying vec4 vPosition;
void main() {
  // Use position to influence color
  float r = (vPosition.x + 1.0) * 0.5;  // Map -1 to 1 -> 0 to 1
  float g = (vPosition.y + 1.0) * 0.5;  // Map -1 to 1 -> 0 to 1
  float b = (vPosition.z + 1.0) * 0.5;  // Map -1 to 1 -> 0 to 1
  gl_FragColor = vec4(r, g, b, 1.0);
}

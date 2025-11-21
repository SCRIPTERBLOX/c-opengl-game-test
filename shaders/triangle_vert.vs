attribute vec4 pos;
varying vec4 vPosition;
void main() {
  gl_Position = pos;
  vPosition = pos;
}

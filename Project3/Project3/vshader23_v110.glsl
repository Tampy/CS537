#version 110

attribute  vec4 vPosition;

uniform  vec4 vColor;

uniform mat4 modelview;

varying    vec4 color;

void main() 
{ 
  gl_Position = modelview*vPosition;
  color = vColor;
}
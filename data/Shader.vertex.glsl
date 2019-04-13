#version 430

layout(location=0) in vec2 in_position;

out vec4 color;

void main(void){
    gl_Position = vec4(in_position,0,1);
    gl_PointSize = 6;
    color = vec4(1,0,0,1);
}

#version 300 es
precision highp float;

layout(location = 0) in vec4 in_pos;
layout(location = 1) in vec4 in_color;
uniform mat4 matrWVP;

out vec4 v_color;

void main() {
    gl_Position = matrWVP * in_pos;
    v_color = in_color;
}
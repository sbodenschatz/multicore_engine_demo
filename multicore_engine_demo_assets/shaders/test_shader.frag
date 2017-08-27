#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 output_color;

layout(location = 0) in vec3 color;

void main() {
    output_color = vec4(color, 1.0);
}

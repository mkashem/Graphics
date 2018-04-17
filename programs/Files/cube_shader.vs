#version 430 core

layout (location = 0) in vec4 position;
uniform mat4 model_matrix;
uniform mat4 projection_matrix;
//uniform mat4 view_matrix;


out Fragment
{
    vec4 color;
} fragment;

void main(void)
{
    gl_Position = (projection_matrix*model_matrix*position);
    fragment.color = vec4 (1.0f, 0.0f, 0.0f, 1.0f);

}

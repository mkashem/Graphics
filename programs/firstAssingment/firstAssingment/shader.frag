#version 430 core


in Fragment
{
   // vec3 normal;
    vec4 color;
} fragment;

out vec4 color;

void main(){


     color = fragment.color;
}

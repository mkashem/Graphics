#version 430

in Fragment{
    vec4 color;
}fragment;

layout( location = 0 ) out vec4 FragColor;

void main() {
    FragColor = fragment.color;
}

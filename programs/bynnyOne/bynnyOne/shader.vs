#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;


out vec3 fN;
out vec3 fE;
out vec3 fL;

uniform mat4 model;
uniform mat4 view;

uniform mat4 projection;
uniform vec4 LightPosition;



void main()
{
   fN = mat3(view*model)*VertexNormal;
   vec4 fv = view*model*vec4(VertexPosition,1.0); // this is vertex coordinet at eye space
   fE = vec3(-fv);
   fL = (LightPosition.xyz-fv.xyz);
   
   gl_Position = projection*view*model * vec4(VertexPosition,1.0);
}

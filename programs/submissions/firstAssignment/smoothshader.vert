#version 430

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec3 VertexNormal;


out Fragment{
    vec4 color;
}fragment;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;
uniform vec4 LightPosition;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;



void main()
{
    vec3 norm;
    vec4 position;

// Get the position and normal in eye space
// Calculate eyespace normal and eyespace  
    // position 
vec3 eyespaceNormal = NormalMatrix*VertexNormal;
vec4 eyespacePosition = ModelViewMatrix*VertexPosition;

// normalize eyespace normal
vec3 N = normalize(eyespaceNormal); //

//Compute direction from the camera origin 

vec3 E = normalize(-eyespacePosition.xyz);

// Calculate light direction
vec3 L = normalize(LightPosition.xyz-eyespacePosition.xyz);
vec4 ambient = AmbientProduct;

// Compute diffuse light
vec4 diffuse = DiffuseProduct*(max(dot(L,N), 0.0));

// Calculate half vector
vec3 H = normalize(L+E); // half vector

   // Calculate diffuse and specular reflection and add to the light intensity
// Compute Specular light

vec4 specular = SpecularProduct *( pow(max(dot(N, H), 0.0), Shininess));

        
   /* fragment.color =  min(vec4(vec3(ambient + diffuse + specular), 1.0), vec4(1.0));*/
fragment.color = min(vec4(vec3(ambient + diffuse + specular), 1.0), vec4(1.0));


        gl_Position = MVP * VertexPosition;

}

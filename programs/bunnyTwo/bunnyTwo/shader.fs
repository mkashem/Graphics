#version 430

in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec2 TexCoord;

out vec4 fColor;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;
uniform sampler2D Tex;


void main(void)
{

// Normalize the input lighting vectors
vec3 N = normalize(fN);
vec3 E = normalize(fE);
vec3 L = normalize(fL);

vec3 H = normalize(L+E);

vec3 ambient = AmbientProduct.xyz;
float Kd = max(dot(L,N), 0.0);
vec3 diffuse = Kd*DiffuseProduct.xyz;

float Ks = pow(max(dot(N, H), 0.0), Shininess);
vec3 specular = Ks*SpecularProduct.xyz;

vec4 TexColor = texture(Tex, TexCoord);
fColor = vec4((ambient + diffuse)*vec3(TexColor),1.0)+ vec4(specular, 1.0);
//fColor = TexColor;

//fColor = vec4(1.0,0.0,0.0, 1.0);
//fColor.a = 1.0;

}



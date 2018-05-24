#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec4 color;
vec3 fN;
vec3 fL;
vec3 fE;

uniform mat4 model;
uniform mat4 view;

uniform mat4 projection;


uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;

uniform float Spot_exponent;
uniform float Spot_cutoff;
uniform vec3 Spot_direction; // directions are vector



void main()
{
 fN = mat3(view*model)*VertexNormal;
   vec4 fv = view*model*vec4(VertexPosition,1.0); // this is vertex coordinet at eye space
   fE = vec3(-fv);
   fL = (LightPosition.xyz-fv.xyz);
   
	
// Normalize the input lighting vectors
vec3 N = normalize(fN);
vec3 E = normalize(fE);
vec3 L = normalize(fL);

vec3 H = normalize(L+E);

vec4 ambient = AmbientProduct;
float Kd = max(dot(L,N), 0.0);
vec4 diffuse = Kd*DiffuseProduct;

float Ks = pow(max(dot(N, H), 0.0), Shininess);
vec4 specular = Ks*SpecularProduct;

float cosBeta = dot(Spot_direction, -L); 
float beta = acos(cosBeta);
if(beta < radians(Spot_cutoff)){
	float attenuation = pow(cos(beta),Spot_exponent);
	color = ambient + (diffuse + specular)*attenuation;
	}
else{
	color = ambient;
	color.a = 1.0;
}

   gl_Position = projection*view*model * vec4(VertexPosition,1.0);
}

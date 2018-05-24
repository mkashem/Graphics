#version 430

in vec3 fN;
in vec3 fL;
in vec3 fE;


out vec4 fColor;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;

uniform float Shininess;

uniform float Spot_exponent;
uniform float Spot_cutoff;
uniform vec3 Spot_direction; // directions are vector


void main(void)
{

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

float cosBeta = dot(Spot_direction, -L); //
float beta = acos(cosBeta);
if (beta < radians(Spot_cutoff)){
	float attenuation = pow(cos(beta),Spot_exponent);
	fColor = ambient + (diffuse + specular)*attenuation;
	}
else
    fColor = ambient;



fColor.a = 1.0;

}



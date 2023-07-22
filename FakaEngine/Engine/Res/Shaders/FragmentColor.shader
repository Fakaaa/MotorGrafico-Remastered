
#version 330 core

in vec2 texCoord;
uniform sampler2D ourTexture;
uniform float isModel = 0; //1 = true, 0 = false.

uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_diffuse2;
//uniform sampler2D texture_diffuse3;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_specular2;

in vec4 color;
in vec3 Normal;
in vec3 FragPos;

uniform int nr_of_directional_light;
uniform int nr_of_point_lights;
uniform int nr_of_spot_light;

struct DireLight
{
	vec3 direction;
	vec3 colour;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define SIZE_DIRECTIONAL_LIGHTS 100
uniform DireLight dirLight[SIZE_DIRECTIONAL_LIGHTS];

struct PointLight
{
	vec3 posLight;
	vec3 colour;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define SIZE_POINT_LIGHTS 100
uniform PointLight pointLight[SIZE_POINT_LIGHTS];

struct SpotLight
{
	vec3 posLight;
	vec3 colour;

	float cutOff;
	float outerCutOff;
	
	float linear;
	float constant;
	float quadratic;

	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define SIZE_SPOT_LIGHTS 100
uniform SpotLight spotLight[SIZE_SPOT_LIGHTS];

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

uniform vec3 cameraPos;
out vec4 FragColor;

vec3 CalcDirLight(DireLight directionalLight, vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight pointLigh, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLigh, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 outPutDirectional = vec3(0.0);
	vec3 outPutPoint = vec3(0.0);
	vec3 outPutSpot = vec3(0.0);

	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 norm = normalize(Normal);

	int aux = nr_of_directional_light;
	if(aux >= SIZE_DIRECTIONAL_LIGHTS)
		aux = SIZE_DIRECTIONAL_LIGHTS;

	for (int i = 0; i < aux; i++)
	{
		outPutDirectional += CalcDirLight(dirLight[i], norm,viewDir);
	}

	aux = nr_of_point_lights;
	if(aux >= SIZE_POINT_LIGHTS)
		aux = SIZE_POINT_LIGHTS;

	for (int i = 0; i < aux; i++)
	{
		outPutPoint += CalcPointLight(pointLight[i], norm, FragPos, viewDir);
	}

	aux = nr_of_spot_light;
	if(aux >= SIZE_SPOT_LIGHTS)
		aux = SIZE_SPOT_LIGHTS;

	for (int i = 0; i < aux; i++)
	{
		outPutSpot += CalcSpotLight(spotLight[i], norm, FragPos, viewDir);
	}
	vec4 outPutShader = vec4((outPutDirectional + outPutPoint + outPutSpot), 1.0);
	vec4 result = (outPutShader + texture(ourTexture, texCoord));

	FragColor = result;
}

vec3 CalcDirLight(DireLight directionalLight, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-directionalLight.direction);

	vec3 ambient = directionalLight.ambient * material.ambient;

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 diffuse = directionalLight.diffuse * (diff * material.diffuse);

	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = directionalLight.specular * (spec * material.specular);

	return (ambient + diffuse + specular) * directionalLight.colour;
}

vec3 CalcPointLight(PointLight pointLigh, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(pointLigh.posLight - fragPos);

	vec3 ambient = pointLigh.ambient * material.ambient;

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 diffuse = pointLigh.diffuse * (diff * material.diffuse);

	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = pointLigh.specular * (spec * material.specular);

	float distance = length(pointLigh.posLight - fragPos);
	float attenuation = 1.0 / (pointLigh.constant + pointLigh.linear * distance +
		pointLigh.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular) * pointLigh.colour;
}

vec3 CalcSpotLight(SpotLight spotLigh, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 ambient= vec3(0.0);
	vec3 diffuse= vec3(0.0);
	vec3 specular= vec3(0.0);

	vec3 lightDir = normalize(spotLigh.posLight - fragPos);

	float theta = dot(lightDir, normalize(-spotLigh.direction));
	float epsilon = spotLigh.cutOff - spotLigh.outerCutOff;
	float intensity = clamp((theta - spotLigh.outerCutOff) / epsilon, 0.0, 1.0);

	if (theta > spotLigh.cutOff)
	{

		ambient = spotLigh.ambient * material.ambient;

		float diff = max(dot(normal, lightDir), 0.0);

		diffuse = spotLigh.diffuse * (diff * material.diffuse);

		vec3 reflectDir = reflect(-lightDir, normal);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

		specular = spotLigh.specular * (spec * material.specular);

		diffuse *= intensity;
		specular *= intensity;

		float distance = length(spotLigh.posLight - fragPos);
		float attenuation = 1.0 / (spotLigh.constant + spotLigh.linear * distance + spotLigh.quadratic * (distance * distance));

		diffuse *= attenuation;
		specular *= attenuation;
	}

	return (ambient + diffuse + specular) * spotLigh.colour;
}
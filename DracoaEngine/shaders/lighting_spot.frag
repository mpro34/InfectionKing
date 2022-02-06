#version 330 core

struct Material
{
    vec3 ambient;
    sampler2D diffuseMap;
    vec3 specular;
    float shininess;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float exponent;

    float cosInnerCone;
    float cosOuterCone;
    bool on;
};

in vec2 TexCoordinate;
in vec3 Normal;
in vec3 FragPos;

out vec4 frag_color;

uniform SpotLight light;
uniform Material material;
uniform vec3 viewPos;

vec3 calcSpotLight();

void main()
{
    // Blinn-Phong lighting model = Ambient + Diffuse + Specular
    // Ambient
    vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoordinate));
    
    vec3 spotColor = vec3(0.0f);

    if (light.on)
    {
        spotColor = calcSpotLight();
    }


    frag_color = vec4(ambient + spotColor, 1.0f);
};

vec3 calcSpotLight()
{
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 spotDir = normalize(light.direction);

    float cosDir = dot(-lightDir, spotDir);
    float spotIntensity = smoothstep(light.cosOuterCone, light.cosInnerCone, cosDir);

    // Diffuse - light depth
    vec3 normal = normalize(Normal);
    float NDotL = max(dot(normal, lightDir), 0.0f); // clamp to > 0
    vec3 diffuse = light.diffuse * NDotL * vec3(texture(material.diffuseMap, TexCoordinate));

    // Specular - shininess (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float NDotH = max(dot(normal, halfDir), 0.0f); // clamp to > 0
    vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

    float d = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d));

    diffuse *= attenuation * spotIntensity;
    specular *= attenuation * spotIntensity;

    return (diffuse + specular);
}
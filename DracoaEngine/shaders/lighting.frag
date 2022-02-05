#version 330 core

in vec2 TexCoordinate;
in vec3 Normal;
in vec3 FragPos;

out vec4 frag_color;

uniform sampler2D myTexture1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // Phong lighting model = Ambient + Diffuse + Specular
    // Ambient
    float ambientFactor = 0.1f;
    vec3 ambient = lightColor * ambientFactor;
    
    // Diffuse - light depth
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float NDotL = max(dot(normal, lightDir), 0.0f); // clamp to > 0
    vec3 diffuse = lightColor * NDotL;

    // Specular - shininess (Phong)
    float specularFactor = 0.8f; // artificial intensity of light
    float shininess = 100.0f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float RDotV = max(dot(reflectDir, viewDir), 0.0f); // clamp to > 0
    vec3 specular = lightColor * specularFactor * pow(RDotV, shininess);

    vec4 texel = texture(myTexture1, TexCoordinate); // texture sampling

    frag_color = vec4(ambient + diffuse + specular, 1.0f) * texel;
};
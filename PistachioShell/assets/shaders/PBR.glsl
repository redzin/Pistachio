
////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_VERTEX_SHADER

layout(std140, binding = 0) uniform CameraData
{
  mat4 view;
  mat4 projection;
  mat4 projectionView;
  vec3 cameraPosition;
};

layout(std140, binding = 1) uniform ModelUniforms
{
  mat4 modelTransform;
  mat4 normalMatrix;
};

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

layout(location = 0) out vec3 out_WorldPosition;
layout(location = 1) out vec3 out_Normal;
layout(location = 2) out vec2 out_Texcoord;

void main()
{
    out_Normal = mat3(normalMatrix) * a_Normal;
    out_Texcoord = a_Texcoord;
    out_WorldPosition = (modelTransform * vec4(a_Position, 1.0)).xyz;
    gl_Position = projectionView * vec4(out_WorldPosition, 1.0);
}
#endif


////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_FRAGMENT_SHADER

const float PI = 3.14159265359;

layout(location = 0) in vec3 in_WorldPosition;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_Texcoord;

layout(std140, binding = 0) uniform CameraData
{
  mat4 view;
  mat4 projection;
  mat4 projectionView;
  vec3 cameraPosition;
};

layout(std140, binding = 2) uniform MaterialUniform
{
  vec4 colorFactor;
  vec2 metallicRoughnessFactor;
};

layout(binding = 0) uniform sampler2D u_ColorSampler;
layout(binding = 1) uniform sampler2D u_MetallicRoughnessSampler;

out vec4 out_color;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
 
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

//uniform vec3 lightPositions[4];
//uniform vec3 lightColors[4];

void main()
{
  
  vec3 lightPositions[4];
  vec3 lightColors[4];


  vec3 up = vec3(0.0f, 1.0f, 0.0f);
  vec3 right = normalize(cross(cameraPosition, up));
  
  //lightPositions[0] = cameraPosition + -5.0f * up +  0.0f * right; // to be input from CPU
  //lightPositions[1] = cameraPosition +  5.0f * up +  0.0f * right; // to be input from CPU
  //lightPositions[3] = cameraPosition +  0.0f * up + -5.0f * right; // to be input from CPU
  //lightPositions[2] = cameraPosition +  0.0f * up +  5.0f * right; // to be input from CPU
  
  lightPositions[0] = vec3( 5.0f,   0.0f, 10.0f); // to be input from CPU
  lightPositions[1] = vec3(-5.0f,   0.0f, 10.0f); // to be input from CPU
  lightPositions[3] = vec3(  0.0f,  5.0f, 10.0f); // to be input from CPU
  lightPositions[2] = vec3(  0.0f, -5.0f, 10.0f); // to be input from CPU

  lightColors[0] = vec3(1.0f, 1.0f, 1.0f) * 200.0f; // to be input from CPU
  lightColors[1] = vec3(1.0f, 1.0f, 1.0f) * 200.0f; // to be input from CPU
  lightColors[2] = vec3(1.0f, 1.0f, 1.0f) * 200.0f; // to be input from CPU
  lightColors[3] = vec3(1.0f, 1.0f, 1.0f) * 200.0f; // to be input from CPU


  //Albedo from color texture
  vec3 color = colorFactor.xyz * texture(u_ColorSampler, in_Texcoord).xyz;
  color.r = pow(color.r, 2.2f);
  color.g = pow(color.g, 2.2f);
  color.b = pow(color.b, 2.2f);

  float metallic = metallicRoughnessFactor[0] * texture(u_MetallicRoughnessSampler, in_Texcoord).b;
  float roughness = max(metallicRoughnessFactor[1] * texture(u_MetallicRoughnessSampler, in_Texcoord).g, 0.05f);
  
  vec3 N = normalize(in_Normal);
  vec3 V = normalize(cameraPosition - in_WorldPosition);
  
  // Surface reflection at zero incidence
  vec3 F0 = vec3(0.04f);
  F0 = mix(F0, color, metallic);

  // Result of the reflectance equation (i.e. total light result)
  vec3 Lo = vec3(0.0);

  for (int i = 0; i < 4; i++)
  {
    
    vec3 L = normalize(lightPositions[i] - in_WorldPosition);
    vec3 H = normalize(V + L);
    float lightDist = length(lightPositions[i] - in_WorldPosition);
    
    // Radiance
    float attenuation = 1.0 / (lightDist * lightDist);
    vec3 radiance = lightColors[i] * attenuation; 
    
    // Fresnel
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    
    // Normal distribution function which approximates how much surface area is aligned directly with the halfway vector, i.e. how much specular light to expect
    float NDF = DistributionGGX(N, H, roughness);
    
    // Geometry function which estimates how much self-shadowing occurs as a result of light getting reflected back by "overhangs" on the surface
    float G = GeometrySmith(N, V, L, roughness);
    
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)  + 0.0001;
    vec3 specular = numerator / denominator;  
    
    vec3 kS = F; // fraction of specular reflection
    vec3 kD = vec3(1.0) - kS; // fraction of refraction
    kD *= 1.0 - metallic;
    
    float NdotL = max(dot(N, L), 0.0);        
    Lo += (kD * color / PI + specular) * radiance * NdotL;
    
  }

  vec3 ambient = vec3(0.03f) * color; // improvised, temporary
  color = ambient + Lo;

  // Gamme correction and tone mapping
  color = color / (color + vec3(1.0));
  color = pow(color, vec3(1.0/2.2));

  out_color = vec4(color, colorFactor.w);
  
  //out_color = vec4(N, 1.0f);
  //out_color = vec4(texture(u_MetallicRoughnessSampler, in_Texcoord).rgb, 1.0f);
  //out_color = vec4(in_Texcoord.st, 0.0f, 1.0f);
  
  if (out_color.w == 0.0f)
    discard;

}

#endif


////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_VERTEX_SHADER

// Uniforms
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

// Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

#ifdef _ENABLE_TEXCOORD_0
layout(location = 2) in vec2 a_Texcoord_0;
#endif

#ifdef _ENABLE_COLOR_BUFFER_3
layout(location = 3) in vec3 a_Color;
#endif

#ifdef _ENABLE_COLOR_BUFFER_4
layout(location = 4) in vec4 a_Color;
#endif

#ifdef _ENABLE_NORMAL_MAPPING
layout(location = 5) in vec4 a_Tangent;
#endif

// Out-variables
layout(location = 0) out vec3 out_WorldPosition;
layout(location = 1) out vec3 out_Normal;

#ifdef _ENABLE_TEXCOORD_0
layout(location = 2) out vec2 out_Texcoord_0;
#endif

#ifdef _ENABLE_COLOR_BUFFER_3
layout(location = 3) out vec3 out_Color;
#endif

#ifdef _ENABLE_COLOR_BUFFER_4
layout(location = 4) out vec4 out_Color;
#endif

#ifdef _ENABLE_NORMAL_MAPPING
//layout(location = 5) out vec4 out_Tangent;
layout(location = 5) out mat3 out_TBN;
#endif

void main()
{
  out_Normal = mat3(normalMatrix) * a_Normal;
    
#ifdef _ENABLE_TEXCOORD_0
  out_Texcoord_0 = a_Texcoord_0;
#endif

#ifdef _ENABLE_COLOR_BUFFER_4
  out_Color = a_Color;
#endif

#ifdef _ENABLE_COLOR_BUFFER_3
  out_Color = a_Color;
#endif

#ifdef _ENABLE_NORMAL_MAPPING
  //out_Tangent = a_Tangent;
  vec3 Bitangent = cross(a_Normal, a_Tangent.xyz * a_Tangent.w);
  vec3 T = normalize(vec3(modelTransform * vec4(a_Tangent.xyz * a_Tangent.w, 0.0)));
  vec3 B = normalize(vec3(modelTransform * vec4(Bitangent, 0.0)));
  vec3 N = normalize(vec3(modelTransform * vec4(a_Normal, 0.0)));
  out_TBN = mat3(T, B, N);
#endif

  out_WorldPosition = (modelTransform * vec4(a_Position, 1.0)).xyz;
  gl_Position = projectionView * vec4(out_WorldPosition, 1.0);
}
#endif


////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_FRAGMENT_SHADER

const float PI = 3.14159265359;

// Uniforms
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

// Samplers
layout(binding = 0) uniform sampler2D u_ColorSampler;

#ifdef _ENABLE_METALLIC_ROUGHNESS_TEXTURE
layout(binding = 1) uniform sampler2D u_MetallicRoughnessSampler;
#endif

#ifdef _ENABLE_NORMAL_MAPPING
layout(binding = 2) uniform sampler2D u_NormalSampler;
#endif

// In-variables
layout(location = 0) in vec3 in_WorldPosition;
layout(location = 1) in vec3 in_Normal;

#ifdef _ENABLE_TEXCOORD_0
layout(location = 2) in vec2 in_Texcoord_0;
#endif

#ifdef _ENABLE_COLOR_BUFFER_3
layout(location = 3) in vec3 in_Color;
#endif

#ifdef _ENABLE_COLOR_BUFFER_4
layout(location = 4) in vec4 in_Color;
#endif

#ifdef _ENABLE_NORMAL_MAPPING
layout(location = 5) in mat3 in_TBN;
#endif

// Out-variables
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
  
  lightPositions[0] = vec3(-0.0f,  10.0f,  20.0f); // to be input from CPU
  lightPositions[1] = vec3( 0.0f, -10.0f,  20.0f); // to be input from CPU
  lightPositions[3] = vec3(-0.0f,  10.0f, -20.0f); // to be input from CPU
  lightPositions[2] = vec3( 0.0f, -10.0f, -20.0f); // to be input from CPU

  lightColors[0] = vec3(1.0f, 1.0f, 1.0f) * 1000.0f; // to be input from CPU
  lightColors[1] = vec3(1.0f, 1.0f, 1.0f) * 1000.0f; // to be input from CPU
  lightColors[2] = vec3(1.0f, 1.0f, 1.0f) * 1000.0f; // to be input from CPU
  lightColors[3] = vec3(1.0f, 1.0f, 1.0f) * 1000.0f; // to be input from CPU


#if defined _ENABLE_TEXCOORD_0 && defined _ENABLE_COLOR_TEXTURE
  //Albedo from color texture
  vec3 color = texture(u_ColorSampler, in_Texcoord_0).xyz; // todo: move colorfactor after sRGB decoding / color correcting
  color.r = pow(color.r, 2.2f);
  color.g = pow(color.g, 2.2f);
  color.b = pow(color.b, 2.2f);
  color *= colorFactor.xyz;
#else
  vec3 color = colorFactor.xyz;
#endif

#if defined _ENABLE_TEXCOORD_0 && defined _ENABLE_METALLIC_ROUGHNESS_TEXTURE
  float metallic = metallicRoughnessFactor.x * texture(u_MetallicRoughnessSampler, in_Texcoord_0).b;
  float roughness = max(metallicRoughnessFactor.y * texture(u_MetallicRoughnessSampler, in_Texcoord_0).g, 0.05f);
  float ao = texture(u_MetallicRoughnessSampler, in_Texcoord_0).r;
#else
  float metallic = metallicRoughnessFactor.x;
  float roughness = max(metallicRoughnessFactor.y, 0.05f);
  float ao = 1.0f;
#endif

#ifdef _ENABLE_COLOR_BUFFER_3
  color *= in_Color;
#endif

#ifdef _ENABLE_COLOR_BUFFER_4
  color *= in_Color.xyz;
#endif

#ifdef _ENABLE_NORMAL_MAPPING
  vec3 N = texture(u_NormalSampler, in_Texcoord_0).rgb;
  N = N * 2.0f - 1.0f;
  N = normalize(in_TBN * N);
#else
  vec3 N = normalize(in_Normal);
#endif

  vec3 V = normalize(cameraPosition - in_WorldPosition);
  
  // Surface reflection at zero incidence
  vec3 F0 = vec3(0.04);
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

  vec3 ambient = vec3(0.03f) * color * ao;
  color = ambient + Lo;

  // Gamma correction and tone mapping
  color = color / (color + vec3(1.0));
  color = pow(color, vec3(1.0/2.2));

  out_color = vec4(color, colorFactor.w);
  
  //out_color = vec4(N, 1.0f);
  //out_color = vec4(texture(u_MetallicRoughnessSampler, in_Texcoord_0).rgb, 1.0f);
  //out_color = vec4(texture(u_NormalSampler, in_Texcoord_0).rgb, 1.0f);
  //out_color = vec4(in_Texcoord_0.st, 0.0f, 1.0f);
  //out_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
  
  if (out_color.w == 0.0f)
    discard;

}

#endif

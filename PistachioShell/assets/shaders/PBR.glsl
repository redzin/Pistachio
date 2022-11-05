
////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_VERTEX_SHADER

layout(std140, binding = 0) uniform CameraData
{
  mat4 view;
  mat4 projection;
  mat4 projectionView;
};
layout(std140, binding = 1) uniform ModelUniforms
{
  mat4 modelTransform;
};

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 1) in vec2 a_Texcoord;

layout(location = 0) out vec3 out_Color;
layout(location = 1) out vec3 out_Normal;
layout(location = 2) out vec2 out_Texcoord;

void main()
{
    out_Color = vec3(1.0f, 0.0f, 0.0f);
    out_Normal = a_Normal;
    out_Texcoord = a_Texcoord;
    gl_Position = projectionView * modelTransform * vec4(a_Position, 1.0);
}
#endif


////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_FRAGMENT_SHADER

layout(location = 0) in vec3 in_Color;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_Texcoord;

out vec4 out_color;

void main()
{
  
  //out_color = vec4(normalize(in_Normal), 1.0f);
  out_color = vec4(in_Texcoord, 0.0f, 1.0f);

  if (out_color.w == 0.0f)
    discard;

}

#endif

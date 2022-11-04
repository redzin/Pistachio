
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

layout(location = 0) out vec3 out_Color;
layout(location = 1) out vec3 out_Normal;

void main()
{
    out_Color = vec3(1.0f, 0.0f, 0.0f);
    out_Normal = a_Normal;
    gl_Position = projectionView * modelTransform * vec4(a_Position, 1.0);
}
#endif


////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_FRAGMENT_SHADER

layout(location = 0) in vec3 in_Color;
layout(location = 1) in vec3 in_Normal;

out vec4 out_color;

void main()
{
  
  out_color = vec4(normalize(in_Normal), 1.0f);

  if (out_color.w == 0.0f)
    discard;

}

#endif

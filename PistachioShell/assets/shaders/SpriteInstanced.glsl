
////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_VERTEX_SHADER

layout(std140, binding = 0) uniform CameraData
{
  mat4 view;
  mat4 projection;
  mat4 projectionView;
};

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_TexIndex;
layout(location = 3) in mat4 a_Transform;

layout(location = 0) out vec3 out_TexCoord;
layout(location = 1) out vec3 out_Color;

void main()
{
    out_TexCoord = vec3(a_TexCoord, a_TexIndex);
    gl_Position = projectionView * a_Transform * vec4(a_Position, 1.0);
}
#endif


////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_FRAGMENT_SHADER

layout(location = 0) in vec3 in_TexCoord;

layout(binding = 0) uniform sampler2DArray u_TextureSampler;

out vec4 out_color;

void main()
{
  
  out_color = texture(u_TextureSampler, in_TexCoord);
  //out_color = vec4(in_TexCoord.xy, 0.0f, 1.0f);

  if (out_color.w == 0.0f)
    discard;

}

#endif

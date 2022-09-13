
////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_VERTEX_SHADER

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(location = 0) out vec2 out_TexCoord;

void main()
{
    out_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position, 1.0);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_FRAGMENT_SHADER

layout(location = 0) in vec2 in_TexCoord;

layout(binding = 0) uniform sampler2D u_TextureSampler;

out vec4 out_color;

void main()
{
  //out_color = vec4(in_TexCoord, 0.0, 1.0);
  out_color = texture(u_TextureSampler, in_TexCoord);
}

#endif

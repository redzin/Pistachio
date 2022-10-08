
////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_VERTEX_SHADER

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

layout(location = 0) out vec3 out_Color;

void main()
{
    out_Color = a_Color;
    gl_Position = vec4(a_Position, 1.0);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SHADER_TYPE_FRAGMENT_SHADER

layout(location = 0) in vec3 in_Color;

out vec4 out_color;

void main()
{
  out_color = vec4(in_Color, 1.0);
}

#endif

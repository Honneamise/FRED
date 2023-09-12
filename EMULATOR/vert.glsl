#version 450

layout (location = 0) in vec2 Pos;
layout (location = 1) in vec2 Uv;

out vec2 Tex;

void main()
{
    gl_Position = vec4(Pos.x, Pos.y, 0.0f, 1.0f);

    Tex = Uv;
}
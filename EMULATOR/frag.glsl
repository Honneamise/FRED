#version 450

uniform sampler2D Texture;

in vec2 Tex;

out vec4 Color;

const vec3 Palette[16] = vec3[] 
(
    vec3(0.0,  0.0,  0.0),  // #000000
    vec3(0.5,  0.0,  0.0),  // #800000
    vec3(0.0,  0.5,  0.0),  // #008000
    vec3(0.5,  0.5,  0.0),  // #808000
    vec3(0.0,  0.0,  0.5),  // #000080
    vec3(0.5,  0.0,  0.5),  // #800080
    vec3(0.0,  0.5,  0.5),  // #008080
    vec3(0.75, 0.75, 0.75), // #C0C0C0
    vec3(0.5,  0.5,  0.5),  // #808080
    vec3(1.0,  0.0,  0.0),  // #FF0000
    vec3(0.0,  1.0,  0.0),  // #00FF00
    vec3(1.0,  1.0,  0.0),  // #FFFF00
    vec3(0.0,  0.0,  1.0),  // #0000FF
    vec3(1.0,  0.0,  1.0),  // #FF00FF
    vec3(0.0,  1.0,  1.0),  // #00FFFF
    vec3(1.0,  1.0,  1.0)   // #FFFFFF
);


void main()
{
    int index = int( texture2D(Texture, Tex).r * 255.0);

    Color = vec4( Palette[index], 1.0);
}
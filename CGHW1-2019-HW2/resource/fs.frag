#version 330
layout(location=0) out vec4 color;

uniform sampler2D text;
uniform vec3 object_color;
uniform vec3 light_pos;
uniform vec3 eye_pos;
uniform int Blinn_Phong;

in vec3 g_position;
in vec2 g_uv;
in vec3 g_normal;

void main()
{
    // color = vec4(1.0);
    vec3 l = normalize ( light_pos - g_position);
    vec3 n = normalize(g_normal);
    float cosine = max(dot(l, n), 0);
    // vec3 r = reflect(l, n); //Phong
    vec3 e = normalize(eye_pos - g_position);
    // vec3 h = normalize(l + e); //Blinn-Phong
    // float spec = cosine * pow(dot(r,e), 30); //Phong ,alpha = 光反射的集中程度
    // float spec = cosine * pow(dot(n,h), 30); //Blinn-Phong ,alpha = 光反射的集中程度
    vec3 h;
    vec3 r;
    float spec;

    if(Blinn_Phong==1){
        h = normalize(l + e);
        spec = cosine * pow(dot(n,h), 30); 
    }else{
        r = reflect(l, n);
        spec = cosine * pow(dot(r,e), 30);
    }

    color = vec4(object_color * texture(text, g_uv).rgb * cosine + spec, 1.0);
}
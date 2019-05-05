#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 vp;
uniform vec3 light_pos;
uniform vec3 eye_pos;
uniform int Blinn_Phong;

out vec3 v_position;
out vec2 v_uv;
out vec3 v_normal;
out float spec;
out float v_cosine;

void main()
{
    v_position = (model*vec4(position, 1.0)).xyz; //乘view就變成view space
    v_uv = texcoord;
    v_normal = (model*vec4(normal, 0.0)).xyz;
    gl_Position = vp*model*vec4(position, 1.0);


    vec3 l = normalize ( light_pos - v_position);
    vec3 n = normalize(v_normal);
    float cosine = max(dot(l, n), 0);
    vec3 e = normalize(eye_pos - v_position);
    vec3 h;
    vec3 r;

    if(Blinn_Phong==1){
        h = normalize(l + e);
        spec = cosine * pow(dot(n,h), 30); 
    }else{
        r = reflect(l, n);
        spec = cosine * pow(dot(r,e), 30);
    }
    v_cosine = cosine;
}
// 灯光顶点着色器 Vertex Shader

#version 330 core

// 顶点数据
layout (location = 0) in vec3 position; // 数据位0 位置

// 全局数据
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 输出数据

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); 
}

// 灯光片段着色器 Fragment Shader

#version 330 core

// 输入数据

// 输出数据
out vec4 color;

void main()
{
	// 灯光发射白光
	color = vec4(1.0f); // 设置四维向量的所有元素为1.0f
}

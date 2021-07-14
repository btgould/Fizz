#shader vertex
#version 330 core

layout(location = 0) in vec4 a_position;

uniform mat4 u_VP;
uniform mat4 u_ModelTRS;

void main() {
	gl_Position = u_VP * u_ModelTRS * a_position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
	color = vec4(0.2f,  0.2f, 0.8f, 1.0f);
};

#shader vertex
#version 330 core

layout(location = 0) in vec4 a_position;

uniform mat4 u_VP;
uniform mat4 u_ModelTRS;

// position of current pixel in world coords
out vec4 v_FragWorldPos;

void main() {
    v_FragWorldPos = u_ModelTRS * a_position;
	gl_Position = u_VP * v_FragWorldPos;
};

#shader fragment
#version 330 core

in vec4 v_FragWorldPos;

// center of circle in world coords
uniform vec2 u_Position;
// radius of circle in world coords
uniform float u_Radius;

float circle(vec2 worldFragPos, vec2 worldCirclePos, float worldCircleRadius) {
    float dist = distance(worldFragPos, worldCirclePos);
    return float(dist < worldCircleRadius);
}

layout(location = 0) out vec4 color;

void main() {
    float alpha = circle(v_FragWorldPos.xy, u_Position, u_Radius);

    if (alpha == 0.0f) 
        discard;

	color = vec4(0.2f,  0.2f, 0.8f, alpha);
};

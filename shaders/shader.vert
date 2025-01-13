#version 450

const int maxNumberOfAttacks = 15;

layout(binding = 0) uniform VertexUniformBufferObject {
	mat4 view;
	mat4 proj;
	mat4 models[maxNumberOfAttacks];
} vubo;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec2 inTexCoord;

layout (location = 0) out vec3 fragPos;
layout (location = 1) out vec3 fragNormal;
layout (location = 2) out vec3 fragColor;
layout (location = 3) out vec2 fragTexCoord;

void main() {
	mat4 model = vubo.models[gl_InstanceIndex];

	fragPos = (model * vec4(inPosition, 0.0)).xyz;
	fragNormal = normalize((model * vec4(inNormal, 0.0)).xyz);
	fragColor = inColor;
	fragTexCoord = inTexCoord;

	gl_Position = vubo.proj * vubo.view * model * vec4(inPosition, 1.0);
}
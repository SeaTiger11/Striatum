#version 450

layout(binding = 1) uniform FragmentUniformBufferObject {
	vec3 camPos;
} fubo;
layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragColor;
layout(location = 3) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

const float ambient = 0.3;

const vec4 sunColor = vec4(1.0, 1.0, 1.0, 1.0);
const vec3 sunDirection = normalize(vec3(1.0, 1.0, -1.0));

void main() {
	outColor = (max(0.0, dot(fragNormal, -sunDirection)) + ambient) * texture(texSampler, fragTexCoord) * vec4(fragColor, 1.0f);
}
#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, color);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && normal == other.normal && color == other.color && texCoord == other.texCoord;
    }
};

struct modelData {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	VkDeviceSize verticesOffset;
	VkDeviceSize indicesOffset;
	VkDeviceSize maxSize;
	glm::vec3 position;
};

struct rayCastHit {
    bool hit;
    float* voxel = nullptr;
    float ironContent;

    rayCastHit(bool inHit, float* inVoxel, float inIron) {
        hit = inHit;
        voxel = inVoxel;
        ironContent = inIron;
    }
};

const float voxelScale = 2.0f;

class voxelSphere {
	public:
		glm::uvec3 scale;
		float realRadius, currentRadius;

		voxelSphere(glm::vec3 inPosition, float inRadius, VkDeviceSize inOffset);
		modelData generateSphere();
		modelData updateMarchingCubes();
		modelData getData();
		bool shouldUpdate();

        rayCastHit rayCast(glm::vec3 position, glm::vec3 orientation);

	private:
		const float updatePoint = 0.25f;

		modelData model;
		glm::vec3 center;
		VkDeviceSize offset;

        const float ironSize = 0.3f, ironCutOff = 0.7f;

		float* grid;

        rayCastHit rayCastMiss = rayCastHit(false, nullptr, 0);

		float& getVoxel(glm::uvec3 pos);
		void addVertex(glm::vec3 pos, glm::vec3 normal, glm::vec3 color, glm::vec2 texCoord);
		void processCube(glm::uvec3 pos);

		glm::vec3 computeNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);

		std::optional<float> rayAABB(glm::vec3 gridMin, glm::vec3 gridMax, glm::vec3 origin, glm::vec3 direction);
};

#endif
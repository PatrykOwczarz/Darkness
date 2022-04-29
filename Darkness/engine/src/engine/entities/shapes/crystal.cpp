#include "pch.h"
#include "crystal.h"
#include <engine.h> 

engine::crystal::crystal(std::vector<glm::vec3> vertices) : m_vertices(vertices) {
	std::vector<glm::vec3> normals;
	// up back normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) -
		vertices.at(2)));
	// up left normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) -
		vertices.at(3)));
	// up front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) -
		vertices.at(4)));
	// up right normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(4), vertices.at(0) -
		vertices.at(1)));
	// down back normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(2), vertices.at(5) -
		vertices.at(1)));
	// down left normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(3), vertices.at(5) -
		vertices.at(2)));
	// down front normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(4), vertices.at(5) -
		vertices.at(3)));
	// down right normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(1), vertices.at(5) -
		vertices.at(4)));

	std::vector<mesh::vertex> crystal_vertices
	{
		//up back
		// position					normal					tex coord
		{ vertices.at(0),			normals.at(0),		{ 0.f, 0.f } },
		{ vertices.at(1),			normals.at(0),		{ 1.f, 0.f } },
		{ vertices.at(2),			normals.at(0),		{ 0.5f, 1.f } },

		//up left
		// position					normal					tex coord
		{ vertices.at(0),			normals.at(1),		{ 0.f, 0.f } },
		{ vertices.at(2),			normals.at(1),		{ 1.f, 0.f } },
		{ vertices.at(3),			normals.at(1),		{ 0.5f, 1.f } },

		//up front
		// position					normal					tex coord
		{ vertices.at(0),			normals.at(2),		{ 0.f, 0.f } },
		{ vertices.at(3),			normals.at(2),		{ 1.f, 0.f } },
		{ vertices.at(4),			normals.at(2),		{ 0.5f, 1.f } },

		//up right
		// position					normal					tex coord
		{ vertices.at(0),			normals.at(3),		{ 0.f, 0.f } },
		{ vertices.at(4),			normals.at(3),		{ 1.f, 0.f } },
		{ vertices.at(1),			normals.at(3),		{ 0.5f, 1.f } },

		//down back
		// position					normal					tex coord
		{ vertices.at(5),			normals.at(4),		{ 0.f, 0.f } },
		{ vertices.at(2),			normals.at(4),		{ 1.f, 0.f } },
		{ vertices.at(1),			normals.at(4),		{ 0.5f, 1.f } },

		//down left
		// position					normal					tex coord
		{ vertices.at(5),			normals.at(5),		{ 0.f, 0.f } },
		{ vertices.at(3),			normals.at(5),		{ 1.f, 0.f } },
		{ vertices.at(2),			normals.at(5),		{ 0.5f, 1.f } },

		//down front
		// position					normal					tex coord
		{ vertices.at(5),			normals.at(6),		{ 0.f, 0.f } },
		{ vertices.at(4),			normals.at(6),		{ 1.f, 0.f } },
		{ vertices.at(3),			normals.at(6),		{ 0.5f, 1.f } },

		//down right
		// position					normal					tex coord
		{ vertices.at(5),			normals.at(7),		{ 0.f, 0.f } },
		{ vertices.at(1),			normals.at(7),		{ 1.f, 0.f } },
		{ vertices.at(4),			normals.at(7),		{ 0.5f, 1.f } },

	};

	const std::vector<uint32_t> crystal_indices
	{
			0, 1, 2,	//up back
			3, 4, 5,	//up left
			6, 7, 8,	//up front
			9, 10, 11,	//up right
			12, 13, 14,	//down back
			15, 16, 17,	//down left
			18, 19, 20,	//down front
			21, 22, 23,	//down right
	};

	m_mesh = engine::mesh::create(crystal_vertices, crystal_indices);
}

engine::crystal::~crystal() {}

engine::ref<engine::crystal> engine::crystal::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::crystal>(vertices);
}

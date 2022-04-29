#include "pch.h"
#include "lamp.h"
#include <engine.h> 

engine::lamp::lamp(std::vector<glm::vec3> vertices) : m_vertices(vertices) {
	std::vector<glm::vec3> normals;
	// low front normal
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(0), vertices.at(4) -
		vertices.at(1)));
	// low left normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(1), vertices.at(5) -
		vertices.at(2)));
	// low back normal
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(2), vertices.at(6) -
		vertices.at(3)));
	// low right normal
	normals.push_back(glm::cross(vertices.at(7) - vertices.at(3), vertices.at(7) -
		vertices.at(0)));
	// high front normal
	normals.push_back(glm::cross(vertices.at(8) - vertices.at(4), vertices.at(8) -
		vertices.at(5)));
	// high left normal
	normals.push_back(glm::cross(vertices.at(9) - vertices.at(5), vertices.at(9) -
		vertices.at(6)));
	// high back normal
	normals.push_back(glm::cross(vertices.at(10) - vertices.at(6), vertices.at(10) -
		vertices.at(7)));
	// high right normal
	normals.push_back(glm::cross(vertices.at(11) - vertices.at(7), vertices.at(11) -
		vertices.at(4)));

	std::vector<mesh::vertex> lamp_vertices
	{
		//bottom
		// position					normal					tex coord
		{ vertices.at(0),			{ 0.f, -1.f, 0.f },		{ 0.f, 0.f } },
		{ vertices.at(1),			{ 0.f, -1.f, 0.f },		{ 1.f, 0.f } },
		{ vertices.at(2),			{ 0.f, -1.f, 0.f },		{ 0.f, 1.f } },
		{ vertices.at(3),			{ 0.f, -1.f, 0.f },		{ 1.f, 1.f } },

		//low front
		// position					normal					tex coord
		{ vertices.at(0),			normals.at(0),		{ 0.f, 0.f } },
		{ vertices.at(1),			normals.at(0),		{ 1.f, 0.f } },
		{ vertices.at(5),			normals.at(0),		{ 0.f, 1.f } },
		{ vertices.at(4),			normals.at(0),		{ 1.f, 1.f } },

		//low left
		// position					normal					tex coord
		{ vertices.at(1),			normals.at(1),		{ 0.f, 0.f } },
		{ vertices.at(2),			normals.at(1),		{ 1.f, 0.f } },
		{ vertices.at(6),			normals.at(1),		{ 0.f, 1.f } },
		{ vertices.at(5),			normals.at(1),		{ 1.f, 1.f } },

		//low back
		// position					normal					tex coord
		{ vertices.at(2),			normals.at(2),		{ 0.f, 0.f } },
		{ vertices.at(3),			normals.at(2),		{ 1.f, 0.f } },
		{ vertices.at(7),			normals.at(2),		{ 0.f, 1.f } },
		{ vertices.at(6),			normals.at(2),		{ 1.f, 1.f } },

		//low right
		// position					normal					tex coord
		{ vertices.at(3),			normals.at(3),		{ 0.f, 0.f } },
		{ vertices.at(0),			normals.at(3),		{ 1.f, 0.f } },
		{ vertices.at(4),			normals.at(3),		{ 0.f, 1.f } },
		{ vertices.at(7),			normals.at(3),		{ 1.f, 1.f } },

		//high front
		// position					normal					tex coord
		{ vertices.at(4),			normals.at(4),		{ 0.f, 0.f } },
		{ vertices.at(5),			normals.at(4),		{ 1.f, 0.f } },
		{ vertices.at(9),			normals.at(4),		{ 0.f, 1.f } },
		{ vertices.at(8),			normals.at(4),		{ 1.f, 1.f } },

		//high left
		// position					normal					tex coord
		{ vertices.at(5),			normals.at(5),		{ 0.f, 0.f } },
		{ vertices.at(6),			normals.at(5),		{ 1.f, 0.f } },
		{ vertices.at(10),			normals.at(5),		{ 0.f, 1.f } },
		{ vertices.at(9),			normals.at(5),		{ 1.f, 1.f } },

		//low back
		// position					normal					tex coord
		{ vertices.at(6),			normals.at(6),		{ 0.f, 0.f } },
		{ vertices.at(7),			normals.at(6),		{ 1.f, 0.f } },
		{ vertices.at(11),			normals.at(6),		{ 0.f, 1.f } },
		{ vertices.at(10),			normals.at(6),		{ 1.f, 1.f } },

		//low right
		// position					normal					tex coord
		{ vertices.at(7),			normals.at(7),		{ 0.f, 0.f } },
		{ vertices.at(4),			normals.at(7),		{ 1.f, 0.f } },
		{ vertices.at(8),			normals.at(7),		{ 0.f, 1.f } },
		{ vertices.at(11),			normals.at(7),		{ 1.f, 1.f } },

		//top
		{ vertices.at(8),			{ 0.f, 1.f, 0.f },		{ 0.f, 0.f } },
		 {vertices.at(9),			{ 0.f, 1.f, 0.f },		{ 1.f, 0.f } },
		 {vertices.at(10),			{ 0.f, 1.f, 0.f },		{ 0.f, 1.f } },
		 {vertices.at(11),			{ 0.f, 1.f, 0.f },		{ 1.f, 1.f } },

	};

	const std::vector<uint32_t> lamp_indices
	{
			0, 1, 2,		0, 2, 3, //bottom
			4, 5, 6,		4, 6, 7, //low front
			8, 9, 10,		8, 10, 11, //low left
			12, 13, 14,		12, 14, 15, //low back
			16, 17, 18,		16, 18, 19, //low right
			20, 21, 22,		20, 22, 23, //high front
			24, 25 ,26,		24, 26, 27, //high left
			28, 29, 30,		28, 30, 31, //high back
			32, 33, 34,		32, 34, 35, //high right
			36, 37, 38,		36, 38, 39 //top
	};

	m_mesh = engine::mesh::create(lamp_vertices, lamp_indices);
}

engine::lamp::~lamp() {}

engine::ref<engine::lamp> engine::lamp::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::lamp>(vertices);
}

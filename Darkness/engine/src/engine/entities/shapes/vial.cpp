#include "pch.h"
#include "vial.h"
#include <engine.h> 

engine::vial::vial(std::vector<glm::vec3> vertices) : m_vertices(vertices) {
	std::vector<glm::vec3> normals;
	// front normal
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(0), vertices.at(4) -
		vertices.at(1)));
	// left normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(1), vertices.at(5) -
		vertices.at(2)));
	// right normal
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(2), vertices.at(6) -
		vertices.at(3)));
	// bottom normal
	normals.push_back(glm::cross(vertices.at(7) - vertices.at(3), vertices.at(7) -
		vertices.at(0)));

	std::vector<mesh::vertex> vial_vertices
	{
		//bottom
		// position					normal					tex coord
		{ vertices.at(0),			{ 0.f, -1.f, 0.f },		{ 0.f, 0.f } },
		{ vertices.at(1),			{ 0.f, -1.f, 0.f },		{ 1.f, 0.f } },
		{ vertices.at(2),			{ 0.f, -1.f, 0.f },		{ 0.f, 1.f } },
		{ vertices.at(3),			{ 0.f, -1.f, 0.f },		{ 1.f, 1.f } },

		//front
		// position					normal					tex coord
		{ vertices.at(0),			normals.at(0),		{ 0.f, 0.f } },
		{ vertices.at(1),			normals.at(0),		{ 1.f, 0.f } },
		{ vertices.at(5),			normals.at(0),		{ 0.f, 1.f } },
		{ vertices.at(4),			normals.at(0),		{ 1.f, 1.f } },

		//left
		// position					normal					tex coord
		{ vertices.at(1),			normals.at(1),		{ 0.f, 0.f } },
		{ vertices.at(2),			normals.at(1),		{ 1.f, 0.f } },
		{ vertices.at(6),			normals.at(1),		{ 0.f, 1.f } },
		{ vertices.at(5),			normals.at(1),		{ 1.f, 1.f } },

		//back
		// position					normal					tex coord
		{ vertices.at(2),			normals.at(2),		{ 0.f, 0.f } },
		{ vertices.at(3),			normals.at(2),		{ 1.f, 0.f } },
		{ vertices.at(7),			normals.at(2),		{ 0.f, 1.f } },
		{ vertices.at(6),			normals.at(2),		{ 1.f, 1.f } },

		//right
		// position					normal					tex coord
		{ vertices.at(3),			normals.at(3),		{ 0.f, 0.f } },
		{ vertices.at(0),			normals.at(3),		{ 1.f, 0.f } },
		{ vertices.at(4),			normals.at(3),		{ 0.f, 1.f } },
		{ vertices.at(7),			normals.at(3),		{ 1.f, 1.f } },

	   //top
	   { vertices.at(4),			{ 0.f, 1.f, 0.f },		{ 0.f, 0.f } },
		{vertices.at(5),			{ 0.f, 1.f, 0.f },		{ 1.f, 0.f } },
		{vertices.at(6),			{ 0.f, 1.f, 0.f },		{ 0.f, 1.f } },
		{vertices.at(7),			{ 0.f, 1.f, 0.f },		{ 1.f, 1.f } },

	};

	const std::vector<uint32_t> vial_indices
	{
			0, 1, 2,		0, 2, 3, //bottom
			4, 5, 6,		4, 6, 7, //front
			8, 9, 10,		8, 10, 11, //left
			12, 13, 14,		12, 14, 15, //back
			16, 17, 18,		16, 18, 19, //right
			20, 21, 22,		20, 22, 23, //back
	};

	m_mesh = engine::mesh::create(vial_vertices, vial_indices);
}

engine::vial::~vial() {}

engine::ref<engine::vial> engine::vial::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::vial>(vertices);
}

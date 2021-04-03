#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

struct VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	void add(glm::vec3 value);
	void add(glm::vec4 value);
	GLuint getId();
	int getComponents();
private:
	GLuint id;
	std::vector<GLfloat> data;
	bool dirty;
	int components;

};
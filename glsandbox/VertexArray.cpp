#include "bugl.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include <exception>


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &id);

	if (!id)
	{
		throw std::exception();

	}

	buffers.resize(20);
	dirty = true;
	vertCount = 0;
}

VertexArray::VertexArray(std::string path)
{
	id = buLoadModel(path, &vertCount);
	dirty = false;
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &id);
}

size_t VertexArray::getVertCount()
{
	return vertCount;
}

void VertexArray::setBuffer(int position, std::shared_ptr<VertexBuffer> buffer)
{
	buffers.at(position) = buffer;
	dirty = true;

}

GLuint VertexArray::getId()
{
	if (dirty)
	{
		glBindVertexArray(id);

		for (size_t i = 0; i < buffers.size(); i++)
		{
			if (!buffers.at(i)) continue;

			// Bind the position VBO, assign it to position 0 on the bound VAO
			// and flag it to be used
			glBindBuffer(GL_ARRAY_BUFFER, buffers.at(i)->getId());

			glVertexAttribPointer(i, buffers.at(i)->getComponents(), GL_FLOAT, GL_FALSE,
				3 * sizeof(GLfloat), (void*)0);

			glEnableVertexAttribArray(i);

		}

		glBindVertexArray(0);
	}

	return id;
}
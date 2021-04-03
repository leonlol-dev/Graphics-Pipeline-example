#pragma once
#include <GL/glew.h>

#include <memory>
#include <vector>
#include <string>

struct VertexBuffer;


struct VertexArray
{
public:
	VertexArray();
	VertexArray(std::string path);
	~VertexArray();
	

	void setBuffer(int position, std::shared_ptr<VertexBuffer> buffers);
	GLuint getId();
	size_t getVertCount();

private:
	GLuint id;
	std::vector<std::shared_ptr<VertexBuffer>> buffers;
	bool dirty;
	size_t vertCount;

};
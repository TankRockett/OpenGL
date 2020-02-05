#include "Vertex.h"
#include "Render.h"

Vertex::Vertex(const void* Data, unsigned int Size)
{
	//Write vertex data into the buffer
	GLCALL(glGenBuffers(1, &ID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, ID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER,Size, Data, GL_STATIC_DRAW));
}

Vertex::~Vertex()
{
	GLCALL(glDeleteBuffers(1, &ID));
}

void Vertex::Bind()const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, ID));
}

void Vertex::Unbind()const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

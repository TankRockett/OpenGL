#include "Vertex.h"
#include "Render.h"
#include "Index.h"

Index::Index(const unsigned int* Data, unsigned int Cnt)
	:Cnt(Cnt)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCALL(glGenBuffers(1, &ID));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,Cnt*sizeof(unsigned int), Data, GL_STATIC_DRAW));
}

Index::~Index()
{
	GLCALL(glDeleteBuffers(1, &ID));
}

void Index::Bind()const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
}

void Index::Unbind()const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

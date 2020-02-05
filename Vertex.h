#pragma once
class Vertex
{
private:
	unsigned int ID;
public:
	//Data::Verteces	Size::in bytes
	Vertex(const void* Data, unsigned int Size);
	~Vertex();

	void Bind()const;
	void Unbind()const;
};


#pragma once
class Index
{
private:
	unsigned int ID;
	unsigned int Cnt;
public:
	//Data::Array of verteces	Cnt::Number of elements
	Index(const unsigned int* Data, unsigned int Cnt);
	~Index();

	void Bind()const;
	void Unbind()const;
	inline unsigned int GetCnt()const { return Cnt; }
};
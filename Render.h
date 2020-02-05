#pragma once

#include <GL/glew.h>

//Make a breakpoint if an error occurs
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x)	GLClearErr(); x; ASSERT(GLLogCall(#x,__FILE__,__LINE__))

void GLClearErr();
bool GLLogCall(const char* Function, const char* File, int Line);

class Render
{

};


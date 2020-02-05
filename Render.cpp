#include "Render.h"
#include <iostream>

void GLClearErr() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* Function, const char* File, int Line) {
	while (GLenum Err = glGetError()) {
		std::cout << "Error: " << Err << " " << Function << " " << File << " " << Line << std::endl;
		return false;
	}
	return true;
}

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Render.h"
#include "Vertex.h"
#include "Index.h"

struct Source {
	std::string Vertex;
	std::string Fragment;
};

static Source ParseShader(const std::string& Path) {
	std::ifstream Stream(Path);

	enum class Type {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string Line;
	std::stringstream SS[2];//Divides the stringstream into 2 pieces: Vertex and Fragment
	Type T = Type::NONE;
	while (getline(Stream, Line)) {
		if (Line.find("#shader") != std::string::npos) {
			if (Line.find("vertex") != std::string::npos)
				T = Type::VERTEX;
			else if (Line.find("fragment") != std::string::npos)
				T = Type::FRAGMENT;
		}
		else {
			//Pushes other lines into the stringstream
			SS[(int)T] << Line << '\n';
		}
	}
	return { SS[0].str(),SS[1].str() };
}

//Get shader's source code and compile it
static unsigned int CompileShader(unsigned int Type, const std::string& Source) {
	unsigned int ID = glCreateShader(Type);
	const char* Src = Source.c_str();//The source must be present
	glShaderSource(ID, 1, &Src, nullptr);
	glCompileShader(ID);

	//Error handling
	int Result;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &Result);//Interger vector
	if (Result == GL_FALSE) {
		int L;
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &L);
		char* Msg = (char*)alloca(L * sizeof(char));
		glGetShaderInfoLog(ID, L, &L, Msg);
		std::cout << "Failed to compile "<<
			(Type == GL_VERTEX_SHADER ? "Vertex":"Fragment")<< " shader!" << std::endl;
		std::cout << Msg << std::endl;
		glDeleteShader(ID);
		return 0;
	}

	return ID;
}

//Use the compiled code to attach to a program and link it
static unsigned int CreateShader(const std::string& Vertex, const std::string& Fragment) {
	unsigned int Program = glCreateProgram();
	unsigned int Vs = CompileShader(GL_VERTEX_SHADER, Vertex);
	unsigned int Fs = CompileShader(GL_FRAGMENT_SHADER, Fragment);

	glAttachShader(Program, Vs);
	glAttachShader(Program, Fs);
	glLinkProgram(Program);
	glValidateProgram(Program);

	glDeleteShader(Vs);
	glDeleteShader(Fs);
	return Program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//Specify current opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//Init glew after glfw context window is initialized
	if (glewInit() != GLEW_OK) 
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float Pos[] = {
			-0.5f,-0.5f,
			0.5f,-0.5f,
			0.5f,0.5f,
			-0.5f,0.5f,
		};

		unsigned int Indices[] = { 0,1,2,2,3,0 };
		unsigned int VAO;
		GLCALL(glGenVertexArrays(1, &VAO));
		GLCALL(glBindVertexArray(VAO));

		//Specify and bind a buffer of verteces to an array layout
		Vertex VB(Pos, 4 * 2 * sizeof(float));

		//Bind the vertex array with the buffer
		GLCALL(glEnableVertexAttribArray(0));
		GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

		//Bind indices from the buffer into an elements array	
		Index IB(Indices, 6);

		Source Src = ParseShader("Shaders/Base.Shader");
		unsigned int Shader = CreateShader(Src.Vertex, Src.Fragment);
		GLCALL(glUseProgram(Shader));

		//Retrieve color variable location and insert its data into the shader
		GLCALL(int Location = glGetUniformLocation(Shader, "u_Color"));
		ASSERT(Location != -1);//If uniform location doesn't exist
		GLCALL(glUniform4f(Location, 0.2f, 0.3f, 0.8f, 1.0f));

		//Unbind buffer
		GLCALL(glBindVertexArray(0));
		GLCALL(glUseProgram(0));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		float R = 0.0f;
		float Incr = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			GLCALL(glUseProgram(Shader));
			GLCALL(glUniform4f(Location, R, 0.3f, 0.8f, 1.0f));

			GLCALL(glBindVertexArray(VAO));
			IB.Bind();

			GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (R > 1.0f)
				Incr = -0.05f;
			else if (R < 0.0f)
				Incr = 0.05f;

			R += Incr;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		glDeleteProgram(Shader);
	}
	glfwTerminate();
	return 0;
}

/*class Vector {
public:
	Vector() {

	}
	Vector(float x, float y) {
		
	}
	float x, y, z;

	float DotProc(const Vector&a,const Vector& b);
	Vector Length()const;
	Vector operator+(const Vector& V)const;
	Vector operator-(const Vector& V)const;
	Vector Normal()const;
};

float Vector::DotProc(const Vector&a,const Vector& b){
	return a.x*b.x + a.y*b.y;
}

Vector Vector::Length()const{
	Vector L;
	return L;
}

Vector Vector::Normal()const {
	Vector Nlized;
	//Points to the vector that will be normalized
	return Nlized;
}

Vector Vector::operator+(const Vector& V)const {
	Vector R;
	R.x = x + V.x;
	R.y = y + V.y;
	return R;
} 

Vector Vector::operator-(const Vector& V)const {
	return Vector(x - V.x , y - V.y);
}

float Appr(float G, float C, float dt) {
	float D = G - C;
	if (D > dt) {
		return C + dt;
	}
	else if (D < -dt)
		return C - dt;
	return G;
}

class E {
public:
	E() {
		p = y = r = 0;
	}
	E(float P, float Y, float R) {
		p = P;
		y = Y;
		r = R;
	}
	float p;
	float y;
	float r;
	};
	V V::Cross(const V& v)const {
	V c;
	c.x = y * v.z - z * v.y;
	c.y = z * v.x - x * v.z;
	c.z = x * v.y - y * v.x;
	return c;
	}
}
*/
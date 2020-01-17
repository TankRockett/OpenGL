#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

	float Pos[] = {
		-0.5f,-0.5f,
		0.0f,0.5f,
		0.5f,-0.5f
	};

	//Generate and bind a buffer to an array
	unsigned int Buffer;
	glGenBuffers(1, &Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float),Pos,GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2,0);

	std::string Vertex =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"  gl_Position = position;\n"
		"}\n";
	std::string Fragment = 
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"  color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";
	unsigned int Shader = CreateShader(Vertex,Fragment);
	glUseProgram(Shader);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDrawArrays(GL_TRIANGLES,0,3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
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
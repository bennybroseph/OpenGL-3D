#include "Graphics.h"


struct MyVertex
{
	float x, y, z;        //Vertex
	float nx, ny, nz;     //Normal
};

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Graphics
{
	unsigned int m_VAO, m_VBO, m_IBO;
	unsigned int m_Shader;

	GLFWwindow* window;

	glm::mat4 m_Viewport;
	glm::mat4 m_Projection;
	glm::mat4 m_ProjectionViewMatrix;

	std::vector<MyVertex> m_Vertex;
	std::vector<unsigned int> m_VertexIndex;

	int Init()
	{
		if (glfwInit() == false)
			return -1;

		window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

		if (window == nullptr)
		{
			glfwTerminate();
			return -2;
		}

		glfwMakeContextCurrent(window);

		//the rest of our code goes here!
		if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
		{
			glfwDestroyWindow(window);
			glfwTerminate();
			return -3;
		}

		glClearColor(0.25f, 0.25f, 0.25f, 1);
		glEnable(GL_DEPTH_TEST);

		//testing what version of OpenGL we are running
		auto major = ogl_GetMajorVersion();
		auto minor = ogl_GetMinorVersion();
		printf_s("GL: %i.%i\n", major, minor);

		const char* vsSource = "#version 150\n \
					 in vec4 Position; \
					 in vec4 Colour; \
					 out vec4 vColour; \
					 uniform mat4 ProjectionView; \
					 void main() { vColour = Colour; gl_Position = ProjectionView * Position; }";

		const char* fsSource = "#version 150\n \
					 in vec4 vColour; \
                     out vec4 FragColor; \
					 void main() { FragColor = vColour; }";

		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vs, 1, (const char**)&vsSource, 0);
		glCompileShader(vs);

		glShaderSource(fs, 1, (const char**)&fsSource, 0);
		glCompileShader(fs);

		m_Shader = glCreateProgram();
		glAttachShader(m_Shader, vs);
		glAttachShader(m_Shader, fs);
		glBindAttribLocation(m_Shader, 0, "Position");
		glBindAttribLocation(m_Shader, 1, "Colour");
		glLinkProgram(m_Shader);

		int success = GL_FALSE;
		glGetProgramiv(m_Shader, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			int infoLogLength = 0;
			glGetShaderiv(m_Shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];

			glGetShaderInfoLog(m_Shader, infoLogLength, 0, infoLog);
			printf("Error: Failed to link Gizmo shader program!\n%s\n", infoLog);
			delete[] infoLog;
		}

		SetView(glm::vec3(0, 0, 10), glm::vec3(0), glm::vec3(0, 1, 0));

		return 0;
	}

	void DrawSquare(const float a_fPosX, const float a_fPosY, const float a_fWidth, const float a_fHeight)
	{
		m_Vertex.push_back({ a_fPosX - a_fWidth, a_fPosY - a_fHeight, 0.0f });
		m_Vertex.push_back({ a_fPosX + a_fWidth, a_fPosY - a_fHeight, 0.0f });
		m_Vertex.push_back({ a_fPosX - a_fWidth, a_fPosY + a_fHeight, 0.0f });
		m_Vertex.push_back({ a_fPosX + a_fWidth, a_fPosY + a_fHeight, 0.0f });

		m_VertexIndex.push_back(0);
		m_VertexIndex.push_back(1);
		m_VertexIndex.push_back(2);
		m_VertexIndex.push_back(2);
		m_VertexIndex.push_back(1);
		m_VertexIndex.push_back(3);
	}

	GLFWwindow * GetWindow() { return window; }

	void SetView(glm::vec3 a_v3Pos, glm::vec3 a_v3LookAt, glm::vec3 a_v3Positive)
	{
		m_Viewport = glm::lookAt(a_v3Pos, a_v3LookAt, a_v3Positive);
		m_Projection = glm::perspective(glm::pi<float>()*0.25f, 16 / 9.f, 0.1f, 1000.f);

		m_ProjectionViewMatrix = m_Projection * m_Viewport;

		glUseProgram(m_Shader);
		unsigned int projectionViewUniform = glGetUniformLocation(m_Shader, "ProjectionView");
		glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_ProjectionViewMatrix));
	}

	void SwapBuffers()
	{
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		if (m_Vertex.size() > 0)
		{
			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			// (??, sizeof(YOUR_VECTOR_NAME) * NUMBER_OF_VERTICIES, "Reference to the place in memory to start", ??)
			glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * m_Vertex.size() * 3, &m_Vertex[0].x, GL_STATIC_DRAW);

			glGenBuffers(1, &m_IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_VertexIndex.size(), m_VertexIndex.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(0));
			glEnableVertexAttribArray(0);
			glBindVertexArray(0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glBindVertexArray(m_VAO);
			glDrawElements(GL_TRIANGLES, m_VertexIndex.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
			glBindVertexArray(0);

			m_Vertex.clear();			
			m_VertexIndex.clear();
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
	}


	int Quit()
	{
		glfwDestroyWindow(window);
		glfwTerminate();

		return 0;
	}
}

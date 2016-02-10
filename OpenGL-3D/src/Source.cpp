#include "Graphics.h"

int main()
{
	bool Run = true;

	Graphics::Init();
	while (Run)
	{		
		Graphics::DrawSquare(0, 0, 1, 1);

		glfwPollEvents();

		if (glfwGetKey(Graphics::GetWindow(), GLFW_KEY_UP) == GLFW_PRESS)
			Graphics::SetView({ 0, 0, 0 }, { 0, 0, 0 }, { 0, 1, 0 });

		Run = glfwGetKey(Graphics::GetWindow(), GLFW_KEY_ESCAPE) == GLFW_RELEASE;

		Graphics::SwapBuffers();
	}
	Graphics::Quit();
	
	return 0;
}


/*
void generateGrid(unsigned int rows, unsigned int cols)
{
	Vertex* aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			aoVertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);

			//create some colour based off something...
			vec3 colour = vec3(sinf((c / (float)(cols - 1)) * (r / (float)(rows - 1))));
			aoVertices[r * cols + c].colour = vec4(colour, 1);
		}
	}

	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int index = 0;

	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			// triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			//triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	}

	///vbo generation
	glGenBuffers(1, &m_VBO);
	///ibo generation
	glGenBuffers(1, &m_IBO);

	//generate a vertex array object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//begin: bind and fill vbo + ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
	//end: bind and fill vbo + ibo

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	delete[] aoVertices;
}

void createShader()
{
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Colour; \
							out vec4 vColour; \
							uniform mat4 ProjectionView; \
							void main() { vColour = Colour; gl_Position = ProjectionView * Position;}";

	const char* fsSource = "#version 410\n \
							in vec4 vColour; \
							out vec4 FragColor; \
							void main() { FragColor = vColour; }";

	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glBindAttribLocation(m_programID, 0, "Position");
	glBindAttribLocation(m_programID, 1, "Colour");

	glLinkProgram(m_programID);


	glUseProgram(m_programID);

}

*/

/*namespace tinyobj
//{
//
//	typedef struct mesh_t
//	{
//		std::vector<float>			positions;
//		std::vector<float>			normals;
//		std::vector<float>			texcoords;
//		std::vector<unsigned int>	indices;
//		std::vector<int>			material_ids;
//	};
//
//	typedef struct shape_t
//	{
//		std::string			name;
//		mesh_t				mesh;
//	};
//
//
//}
//
//struct OpenGLInfo
//{
//	unsigned int m_VAO;
//	unsigned int m_VBO;
//	unsigned int m_IBO;
//	unsigned int m_index_count;
//};
//
//std::vector<OpenGLInfo> m_gl_info;
//
//void createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes)
//{
//	m_gl_info.resize(shapes.size());
//	for (unsigned int mesh_index = 0; mesh_index < shapes.size(); ++mesh_index)
//	{
//		glGenVertexArrays(1, &m_gl_info[mesh_index].m_VAO);
//		glGenBuffers(1, &m_gl_info[mesh_index].m_VBO);
//		glGenBuffers(1, &m_gl_info[mesh_index].m_IBO);
//		glBindVertexArray(m_gl_info[mesh_index].m_VAO);
//
//		unsigned int float_count = shapes[mesh_index].mesh.positions.size();
//		float_count += shapes[mesh_index].mesh.normals.size();
//		float_count += shapes[mesh_index].mesh.texcoords.size();
//
//		std::vector<float> vertex_data;
//		vertex_data.reserve(float_count);
//
//		vertex_data.insert(vertex_data.end(), shapes[mesh_index].mesh.positions.begin(), shapes[mesh_index].mesh.positions.end());
//
//		vertex_data.insert(vertex_data.end(), shapes[mesh_index].mesh.normals.begin(), shapes[mesh_index].mesh.normals.end());
//
//		m_gl_info[mesh_index].m_index_count = shapes[mesh_index].mesh.indices.size();
//
//		glBindBuffer(GL_ARRAY_BUFFER, m_gl_info[mesh_index].m_VBO);
//		glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_info[mesh_index].m_IBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[mesh_index].mesh.indices.size() * sizeof(unsigned int), shapes[mesh_index].mesh.indices.data(), GL_STATIC_DRAW);
//
//		glEnableVertexAttribArray(0); //position
//		glEnableVertexAttribArray(1); //normal data
//
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0,
//			(void*)(sizeof(float)*shapes[mesh_index].mesh.positions.size()));
//
//		glBindVertexArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	}
//
//}
*/

/*std::vector<tinyobj::shape_t> shapes;

Application *theApp = new Application();

if(theApp->startup() == true)
{
while (theApp->update() == true)
{
theApp->draw();
}
theApp->shutdown();
}

delete theApp;*/
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <vector>
#include "stdio.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h> 
#define GLM_FORCE_PURE

#include <glm/fwd.hpp>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <iostream>

namespace Graphics
{
	int Init();

	void DrawSquare(const float a_fPosX, const float a_fPosY, const float a_fWidth, const float a_fHeight);

	GLFWwindow * GetWindow();

	void SetView(glm::vec3 a_v3Pos, glm::vec3 a_v3LookAt, glm::vec3 a_v3Positive);

	void SwapBuffers();

	int Quit();
}

#endif // !_GRAPHICS_H_

#include <iostream>
#include <gl_core_4_4.h>
#include <glfw3.h>
#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

int main()
{
	if (glfwInit() == false)
		return -1;

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Graphics Window", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	Gizmos::create();
	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	mat4 projection = glm::perspective(glm::pi<float>() * .25f, 16 / 9.f, .1f /*doesnt change anything but if number is too large then code breaks*/
		, 1000.f /*doesnt change anything*/);

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	while (glfwWindowShouldClose(window) == false &&
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();

		Gizmos::addTransform(glm::mat4(1));
		
		//Code starts here
		vec4 yellow = vec4(255, 255, 0, .5); //Red, green, blue, transparency
		vec4 blue = vec4(0, 0, 255, .8);
		vec4 white = vec4(255, 255, 255, 1);

		float time = glfwGetTime();

		//Rotate Sun
		mat4 Sun = glm::rotate(time * 1 * .25f , vec3(0,1,0));
		mat4 Earth = Sun * glm::translate(vec3(0, 0, 6)) * glm::rotate(time * 1 * 5, vec3(0, 1, 0));
		mat4 Moon = Earth * glm::translate(vec3(1.75, 0, 0));

		Gizmos::addSphere(vec3(Sun[3]), 3, 50, 50, yellow, &Sun); //Sun
		Gizmos::addSphere(vec3(Earth[3]), 1, 50, 50, blue, &Earth); // Earth
		Gizmos::addSphere(vec3(Moon[3]), .35, 50, 50, white, &Moon); // Moon

		//code end here
		vec4 black(0, 0, 0, 1);

		for (int i = 0; i < 21; ++i)
		{
			Gizmos::addLine(vec3(-10 + i, 0, 10),
				vec3(-10 + i, 0, -10),
				i == 10 ? white : black);

			Gizmos::addLine(vec3(10, 0, -10 + i),
				vec3(-10, 0, -10 + i),
				i == 10 ? white : black);
		}
		Gizmos::draw(projection * view);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


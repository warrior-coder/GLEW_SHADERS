#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

/*
GLEW (OpenGL Extension Wrangler) provides modern OpenGL functionality (shaders, vertex objects)
GLFW (OpenGL FrameWork) provides creation and management multi-platform windows with OpenGL context and gives access to input 
 */

int main(void)
{
    // initialize GLFW
    if (!glfwInit())
    {
        return 1;
    }

    // create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(400, 400, "GLEW Start Window", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return 2;
    }

    // make the window's context current
    glfwMakeContextCurrent(window);

    // initialize GLEW only after we created a valid OpenGL rendering context
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return 3;
    }

    // loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // render image
        glClearColor(0.5f, 0.5f, 0.5f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();

        glBegin(GL_TRIANGLES);
	        glColor3f(1, 0, 0); glVertex2f(0, 0.8f);
	        glColor3f(0, 1, 0); glVertex2f(0.87f, -0.7f);
	        glColor3f(0, 0, 1); glVertex2f(-0.87f, -0.7f);
        glEnd();

        glPopMatrix();

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    // free memory
    glfwTerminate();

    return 0;
}
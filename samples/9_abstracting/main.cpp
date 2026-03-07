#include "render.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Set core */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Interval of swap buffers */
    glfwSwapInterval(1);

    /* Initialize glew (after made context) */
    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    /* Display the version of OpenGL */
    GLCALL(const GLubyte *gl_version = glGetString(GL_VERSION));
    std::cout << gl_version << std::endl;

    {
        // clang-format off
        float positions[] = {
            -0.5f, -0.5f, // 0
             0.5f, -0.5f, // 1
             0.5f,  0.5f, // 2
            -0.5f,  0.5f  // 3
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        // clang-format on

        /* Create vertex array */
        VertexArrary va;

        /* Create a buffer and bind data */
        VertexBuffer vb(positions, sizeof(positions));

        /* How to parse the configuration data */
        VertexBufferLayout layout;
        layout.push<float>(2);
        va.addBuffer(vb, layout);

        /* Create an index buffer and bind data */
        IndexBuffer ib(indices, 6);

        /* Create shader */
        std::string shader_path = "resource/shaders/";
        Shader shader({
            {  GL_VERTEX_SHADER,     shader_path + "basic_vertex.glsl"},
            {GL_FRAGMENT_SHADER, shader_path + "uniform_fragment.glsl"}
        });
        shader.bind();

        /* Uniform */
        float r = 0.0f;
        shader.setUniform4f("u_Color", r, 0.5f, 0.8f, 1.0f);

        /* Unlink */
        va.unbind();
        shader.unbind();
        vb.unbind();
        ib.unbind();

        /* Loop until the user closes the window */
        float increment = 0.05f;
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCALL(glClear(GL_COLOR_BUFFER_BIT));

            shader.bind();
            shader.setUniform4f("u_Color", r, 0.5f, 0.8f, 1.0f);

            /* Use vao instead of buffer and ibo */
            va.bind();

            GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f || r < 0.0f)
            {
                increment *= -1;
            }
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();

    return 0;
}
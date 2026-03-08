#include "error.h"
#include "vertex_buffer.h"
#include "renderer.h"
#include "texture.h"

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
    window = glfwCreateWindow(480, 480, "Hello World", NULL, NULL);
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
        /* XYUV */
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
             0.5f, -0.5f, 1.0f, 0.0f, // 1
             0.5f,  0.5f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, 0.0f, 1.0f  // 3
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        // clang-format on

        /* Use blend*/
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Create vertex array */
        VertexArrary va;

        /* Create a buffer and bind data */
        VertexBuffer vb(positions, sizeof(positions));

        /* How to parse the configuration data */
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        va.addBuffer(vb, layout);

        /* Create an index buffer and bind data */
        IndexBuffer ib(indices, 6);

        /* Create shader */
        std::string shader_path = "resource/shaders/";
        Shader shader({
            {  GL_VERTEX_SHADER,   shader_path + "texture_vertex.glsl"},
            {GL_FRAGMENT_SHADER, shader_path + "texture_fragment.glsl"}
        });
        shader.bind();

        /* Create texture */
        std::string texture_path = "resource/textures/";
        Texture texture(texture_path + "thinking_face.png");
        texture.bind(0);

        /* Uniform */
        shader.setUniform1i("u_Texture", 0);

        /* Unlink */
        va.unbind();
        shader.unbind();
        vb.unbind();
        ib.unbind();

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.clear();
            shader.bind();
            renderer.draw(va, ib, shader);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();

    return 0;
}
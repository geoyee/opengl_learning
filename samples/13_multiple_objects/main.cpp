#include "error.h"
#include "vertex_buffer.h"
#include "renderer.h"
#include "texture.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <vector>

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
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
             50.0f, -50.0f, 1.0f, 0.0f, // 1
             50.0f,  50.0f, 1.0f, 1.0f, // 2
            -50.0f,  50.0f, 0.0f, 1.0f  // 3
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        // clang-format on

        /* Use blend */
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Create vertex array */
        VertexArrary va;

        /* Create a buffer and bind data */
        VertexBuffer vb(positions, sizeof(positions));

        /* How to parse the configuration data */
        VertexBufferLayout layout;
        layout.push<float>(2); // XY
        layout.push<float>(2); // UV
        va.addBuffer(vb, layout);

        /* Create an index buffer and bind data */
        IndexBuffer ib(indices, 6);

        /* Create shader */
        std::string shader_path = "resource/shaders/";
        Shader shader({
            {  GL_VERTEX_SHADER,      shader_path + "proj_vertex.glsl"},
            {GL_FRAGMENT_SHADER, shader_path + "texture_fragment.glsl"}
        });
        shader.bind();

        /* Create texture */
        std::string texture_path = "resource/textures/";
        Texture texture(texture_path + "thinking_face.png");
        texture.bind(0);

        /* Math */
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        /* Uniform */
        shader.setUniform1i("u_Texture", 0);

        /* Unlink */
        va.unbind();
        shader.unbind();
        vb.unbind();
        ib.unbind();

        Renderer renderer;

        /* Init ImGui */
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();

        {
            /* Setup scaling (This is not necessary) */
            float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
            ImGuiStyle& style = ImGui::GetStyle();
            style.ScaleAllSizes(main_scale);
            style.FontScaleDpi = main_scale;
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
            io.ConfigDpiScaleFonts = true;
            io.ConfigDpiScaleViewports = true;
#endif
        }

        constexpr size_t OBJ_NUM = 2;
        std::vector<glm::vec3> translations;
        translations.reserve(OBJ_NUM);
        for (size_t i = 0; i < OBJ_NUM; ++i)
        {
            translations.emplace_back(200.0f + 200.0f * i, 200.0f, 0);
        };

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.clear();

            /* Start the Dear ImGui frame */
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            shader.bind();
            for (size_t i = 0; i < OBJ_NUM; ++i)
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translations[i]);
                glm::mat4 mvp = proj * view * model;
                shader.setUniformMat4f("u_MVP", mvp);
                renderer.draw(va, ib, shader);
            }

            /* UI of ImGui */
            ImGui::Begin("Debug");
            for (size_t i = 0; i < OBJ_NUM; ++i)
            {
                ImGui::SliderFloat3(
                    ("Translations [" + std::to_string(i) + "]").c_str(), &translations[i].x, 0.0f, 960.0f);
            }
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

            /* Rendering */
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    /* Cleanup */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
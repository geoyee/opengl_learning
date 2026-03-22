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

#include <array>
#include <iostream>
#include <vector>

/* Struct of Vertex */
struct Vertex
{
    glm::vec3 position;  // XYZ
    glm::vec4 color;     // RGBA
    glm::vec2 texCoords; // UV
    float texID;         // TexID
};

static std::array<Vertex, 4> CreateQuad(
    float x, float y, float texID, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
{
    float size = 100.0f;
    Vertex v0;
    v0.position = {x, y, 0.0f};
    v0.color = {r, g, b, a};
    v0.texCoords = {0.0f, 0.0f};
    v0.texID = texID;
    Vertex v1;
    v1.position = {x + size, y, 0.0f};
    v1.color = {r, g, b, a};
    v1.texCoords = {1.0f, 0.0f};
    v1.texID = texID;
    Vertex v2;
    v2.position = {x + size, y + size, 0.0f};
    v2.color = {r, g, b, a};
    v2.texCoords = {1.0f, 1.0f};
    v2.texID = texID;
    Vertex v3;
    v3.position = {x, y + size, 0.0f};
    v3.color = {r, g, b, a};
    v3.texCoords = {0.0f, 1.0f};
    v3.texID = texID;
    return {v0, v1, v2, v3};
};

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
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4,
        };
        // clang-format on

        /* Use blend */
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Create vertex array */
        VertexArrary va;

        /* Create a buffer and bind data */
        constexpr int VERTEX_NUM = 1000;
        VertexBuffer vb(sizeof(Vertex) * VERTEX_NUM);

        /* How to parse the configuration data */
        VertexBufferLayout layout;
        layout.push<float>(3); // XYZ
        layout.push<float>(4); // RGBA
        layout.push<float>(2); // UV
        layout.push<float>(1); // TexID
        va.addBuffer(vb, layout);

        /* Create an index buffer and bind data */
        IndexBuffer ib(indices, 12);

        /* Create shader */
        std::string shader_path = "resource/shaders/";
        Shader shader({
            {  GL_VERTEX_SHADER,   shader_path + "batch_vertex.glsl"},
            {GL_FRAGMENT_SHADER, shader_path + "batch_fragment.glsl"}
        });
        shader.bind();

        /* Create texture */
        std::string texture_path = "resource/textures/";
        std::vector<std::string> texture_names = {"thinking_face.png", "face_with_raised_eyebrow.png"};
        size_t texture_num = texture_names.size();
        std::vector<Texture> textures;
        textures.reserve(texture_num);
        for (unsigned int i = 0; i < texture_num; ++i)
        {
            textures.emplace_back(texture_path + texture_names[i]);
            textures.back().bind(i);
        }

        /* Math */
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        /* Uniform */
        int samplers[2] = {0, 1};
        shader.setUniform1iv("u_Textures", 2, samplers);

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

        glm::vec3 translation(200.0f, 200.0f, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.clear();

            /* Set dynamic vertex buffer */
            auto q0 = CreateQuad(-50.0f, -50.0f, 0.0f);
            auto q1 = CreateQuad(50.0f, -50.0f, 1.0f);
            Vertex vertices[8];
            memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
            memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));
            vb.bind(vertices, sizeof(vertices));

            /* Start the Dear ImGui frame */
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            /* Rebinding is required, ImGui may modify the state of the texture unit */
            for (unsigned int i = 0; i < texture_num; ++i)
            {
                textures[i].bind(i);
            }

            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = proj * view * model;

            shader.bind();
            shader.setUniformMat4f("u_MVP", mvp);
            renderer.draw(va, ib, shader);

            /* UI of ImGui */
            ImGui::Begin("Debug");
            ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
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
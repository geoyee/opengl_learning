#include "camera.h"
#include "error.h"
#include "vertex_buffer.h"
#include "renderer.h"
#include "texture.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// Settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // ---------- GLFW Initialization ----------
    glfwSetErrorCallback([](int error, const char *description)
                         { std::cerr << "GLFW Error " << error << ": " << description << std::endl; });
    if (glfwInit() != GL_TRUE)
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // ---------- GLEW Initialization ----------
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // ---------- ImGui Setup ----------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    // DPI scaling
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    io.ConfigDpiScaleFonts = true;
    io.ConfigDpiScaleViewports = true;
#endif // GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3

    // Backend initialization
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    {
        // ******************************************************************************
        /* Setting */
        GLCALL(glEnable(GL_DEPTH_TEST));
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Create VBO, VIO, VAO */
        // clang-format off
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 0
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 3

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // 4
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // 5
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // 6
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // 7

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // 8
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 9
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 10
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // 11

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // 12
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 13
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 14
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // 15

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // 16
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // 17
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // 18
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // 19

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 20 
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 21 
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // 22 
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f  // 23
    };
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,   0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f,  -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f,  -3.5f),
        glm::vec3(-1.7f,  3.0f,  -7.5f),
        glm::vec3( 1.3f, -2.0f,  -2.5f),
        glm::vec3( 1.5f,  2.0f,  -2.5f),
        glm::vec3( 1.5f,  0.2f,  -1.5f),
        glm::vec3(-1.3f,  1.0f,  -1.5f)
    };
        // clang-format on

        VertexArrary va;
        VertexBuffer vb(vertices, sizeof(vertices));
        VertexBufferLayout layout;
        layout.push<float>(3); // XYZ
        layout.push<float>(2); // UV
        va.addBuffer(vb, layout);

        /* Generate index */
        unsigned int indices[36];
        unsigned int offset = 0;
        for (unsigned int i = 0; i < 36; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;
            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;
            offset += 4;
        }
        IndexBuffer ib(indices, sizeof(indices));

        /* Create shader */
        std::string shader_path = "resource/shaders/";
        Shader shader({
            {  GL_VERTEX_SHADER,   shader_path + "merge_vertex.glsl"},
            {GL_FRAGMENT_SHADER, shader_path + "merge_fragment.glsl"}
        });
        shader.bind();

        /* Create texture */
        std::string texture_path = "resource/textures/";
        std::vector<std::string> texture_names = {"thinking_face.png", "southeast.png"};
        size_t texture_num = texture_names.size();
        std::vector<Texture> textures;
        textures.reserve(texture_num);
        for (unsigned int i = 0; i < texture_num; ++i)
        {
            textures.emplace_back(texture_path + texture_names[i]);
            textures.back().bind(i);
        }

        /* Uniform */
        int samplers[2] = {0, 1};
        shader.setUniform1iv("u_Textures", 2, samplers);

        vb.unbind();
        va.unbind();
        shader.unbind();
        // ******************************************************************************

        // ---------- Main Loop ----------
        Renderer renderer;
        GLCALL(const GLubyte *gl_version = glGetString(GL_VERSION));

        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            // Render here
            renderer.clear();

            // Start ImGui frame
            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();

            shader.bind();

            // ******************************************************************************
            /* Rebinding is required, ImGui may modify the state of the texture unit */
            for (unsigned int i = 0; i < texture_num; ++i)
            {
                textures[i].bind(i);
            }

            glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
            transform = glm::rotate(transform, currentFrame, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 proj =
                glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();

            for (unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                glm::mat4 mvp = proj * view * model * transform;
                shader.setUniformMat4f("u_MVP", mvp);
                renderer.draw(va, ib, shader);
            }
            // ******************************************************************************

            // UI
            ImGui::Begin("Debug");
            ImGui::Text("GL version: %s", gl_version);
            ImGui::Text("Zoom: %.0f", camera.Zoom);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

            // Render ImGui
            ImGui::Render();

            // OpenGL rendering
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } // The purpose of this block is to ensure that resources are released in the correct order and that the programme exits correctly when the window is closed

    // ---------- Cleanup ----------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera::Movement::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera::Movement::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera::Movement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera::Movement::RIGHT, deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    GLCALL(glViewport(0, 0, width, height));
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback([[maybe_unused]] GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback([[maybe_unused]] GLFWwindow *window, [[maybe_unused]] double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
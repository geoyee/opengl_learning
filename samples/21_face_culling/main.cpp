#include "camera.h"
#include "error.h"
#include "model.h"
#include "shader.h"

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
constexpr unsigned int SCR_WIDTH = 1200;
constexpr unsigned int SCR_HEIGHT = 900;
std::string shader_path = "resource/shaders/";
std::string texture_path = "resource/textures/";
bool mouse_ctr = true;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
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
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Backend initialization
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    {
        // ******************************************************************************
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

        /* Create shader and texture */
        Shader shader({
            {  GL_VERTEX_SHADER, shader_path + "simple.vs"},
            {GL_FRAGMENT_SHADER, shader_path + "simple.fs"}
        });
        Texture tex(texture_path + "container2.png");
        // ******************************************************************************

        // ---------- Main Loop ----------
        Renderer renderer;

        GLCALL(const GLubyte *gl_version = glGetString(GL_VERSION));
        GLCALL(glEnable(GL_CULL_FACE));
        GLCALL(glCullFace(GL_BACK));
        GLCALL(glFrontFace(GL_CCW));

        bool cull_back = true;

        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            GLCALL(glCullFace(cull_back ? GL_BACK : GL_FRONT));

            // Render here
            renderer.clear();

            // Start ImGui frame
            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();

            // ******************************************************************************
            tex.bind();
            shader.bind();
            glm::mat4 proj =
                glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(-1.0f, 1.0f, 0.5f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            shader.setUniformMat4f("u_Model", model);
            shader.setUniformMat4f("u_View", view);
            shader.setUniformMat4f("u_Proj", proj);
            shader.setUniform1i("u_Texture", 0);
            renderer.draw(va, ib, shader);
            // ******************************************************************************

            // UI
            ImGui::Begin("Debug");
            ImGui::Text("GL version: %s", gl_version);
            ImGui::Text("Zoom: %.0f", camera.Zoom);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

            ImGui::Begin("Controller");
            ImGui::Checkbox("glCullFace_GL_BACK", &cull_back);
            ImGui::End();

            // Render ImGui
            ImGui::Render();

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow *backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

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
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera::Movement::UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera::Movement::DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        mouse_ctr = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        mouse_ctr = false;
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
    if (!mouse_ctr)
    {
        return;
    }
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
    if (!mouse_ctr)
    {
        return;
    }
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
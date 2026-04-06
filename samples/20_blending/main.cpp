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
std::string model_path = "resource/models/";
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
        // clang-format off
        float win_vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 1
             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // 3
            
        };
        unsigned int win_indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        // clang-format on
        VertexArrary win_vao;
        VertexBuffer win_vbo(win_vertices, sizeof(win_vertices));
        VertexBufferLayout layout;
        layout.push<float>(3); // XYZ
        layout.push<float>(3); // Normal
        layout.push<float>(2); // UV
        win_vao.addBuffer(win_vbo, layout);
        IndexBuffer win_veo(win_indices, sizeof(win_indices));

        /* Load model and shader */
        Shader win_shader({
            {  GL_VERTEX_SHADER,   shader_path + "model_vertex.glsl"},
            {GL_FRAGMENT_SHADER, shader_path + "blend_fragment.glsl"}
        });
        Texture win_tex(texture_path + "blending_transparent_window.png");

        Model obj(model_path + "backpack/backpack.obj", 1); // 0 for window
        Shader shader({
            {  GL_VERTEX_SHADER,   shader_path + "model_vertex.glsl"},
            {GL_FRAGMENT_SHADER, shader_path + "model_fragment.glsl"}
        });

        /* Position of window */
        std::vector<glm::vec3> pos_wins{glm::vec3(-0.25f, 0.0f, 0.0f), glm::vec3(0.25f, 0.0f, -1.0f)};
        std::sort(pos_wins.begin(),
                  pos_wins.end(),
                  [](const glm::vec3& a, const glm::vec3& b)
                  { return glm::length(camera.Position - a) > glm::length(camera.Position - b); });
        // ******************************************************************************

        // ---------- Main Loop ----------
        Renderer renderer(true, false, true);

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

            // ******************************************************************************
            shader.bind();
            glm::mat4 proj =
                glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -15.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            shader.setUniformMat4f("u_Model", model);
            shader.setUniformMat4f("u_View", view);
            shader.setUniformMat4f("u_Proj", proj);
            obj.draw(renderer, shader);

            win_tex.bind(0);
            win_shader.bind();
            for (const auto& win_pos : pos_wins)
            {
                glm::mat4 win_model = glm::mat4(1.0f);
                win_model = glm::translate(win_model, win_pos);
                win_model = glm::scale(win_model, glm::vec3(1.0f, 1.0f, 1.0f));
                win_shader.setUniformMat4f("u_Model", win_model);
                win_shader.setUniformMat4f("u_View", view);
                win_shader.setUniformMat4f("u_Proj", proj);
                win_shader.setUniform1i("u_Window", 0);
                renderer.draw(win_vao, win_veo, win_shader);
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
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#define ASSERT(x)                                                                                                      \
    if (!(x))                                                                                                          \
    {                                                                                                                  \
        __debugbreak();                                                                                                \
    }

#define GLCALL(func)                                                                                                   \
    GLClearError();                                                                                                    \
    func;                                                                                                              \
    ASSERT(GLLogError(#func, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

static bool GLLogError(const char *func_name, const char *file, int line)
{
    while (GLenum code = glGetError())
    {
        std::cerr << "[OpenGL ERROR] " << file << " " << func_name << " -> (" << line << "): " << code << std::endl;
        return false;
    }
    return true;
}

static std::string ParseShader(const std::string& file)
{
    std::ifstream fs(file);
    std::ostringstream oss;
    if (!fs.is_open())
    {
        return "";
    }
    oss << fs.rdbuf();
    return oss.str();
}

static unsigned int CompilerShader(unsigned int type, const std::string& source)
{
    GLCALL(unsigned int id = glCreateShader(type));
    const char *src = source.data();
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));
    /* Error handing */
    int res;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &res));
    if (res == GL_FALSE)
    {
        int len;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
        std::vector<char> msg(len);
        GLCALL(glGetShaderInfoLog(id, len, &len, msg.data()));
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader: " << reinterpret_cast<char const *>(msg.data()) << std::endl;
        GLCALL(glDeleteShader(id));
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCALL(unsigned int program = glCreateProgram());
    unsigned int vs = CompilerShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompilerShader(GL_FRAGMENT_SHADER, fragmentShader);
    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));
    /* Generally not deleted, for ease of debugging and suchlike */
    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));
    return program;
}

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

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

    /* Create a buffer and bind data */
    unsigned int buffer;
    GLCALL(glGenBuffers(1, &buffer));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    /* How to parse the configuration data */
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr));

    /* Create an index buffer and bind data */
    unsigned int ibo;
    GLCALL(glGenBuffers(1, &ibo));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    /* Create shader */
    std::string shader_path = "resource/shaders/";
    std::string vertex_shader_src = ParseShader(shader_path + "basic_vertex.glsl");
    std::string fragment_shader_src = ParseShader(shader_path + "uniform_fragment.glsl");
    unsigned int shader = CreateShader(vertex_shader_src, fragment_shader_src);
    GLCALL(glUseProgram(shader));

    /* Uniform */
    float r = 0.0f;
    GLCALL(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCALL(glUniform4f(location, r, 0.5f, 0.8f, 1.0f));

    /* Loop until the user closes the window */
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));

        GLCALL(glUniform4f(location, r, 0.5f, 0.8f, 1.0f));
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

    glfwTerminate();
    GLCALL(glDeleteProgram(shader));
    return 0;
}
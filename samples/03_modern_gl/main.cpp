#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

static unsigned int CompilerShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.data();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    /* Error handing */
    int res;
    glGetShaderiv(id, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> msg(len);
        glGetShaderInfoLog(id, len, &len, msg.data());
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader: " << reinterpret_cast<char const *>(msg.data()) << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompilerShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompilerShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    /* Generally not deleted, for ease of debugging and suchlike */
    glDeleteShader(vs);
    glDeleteShader(fs);
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

    /* Initialize glew (after made context) */
    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    /* Display the version of OpenGL */
    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = {-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f};

    /* Create a buffer and bind data */
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    /* How to parse the configuration data */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    /* Create shader */
    std::string vertex_shader_src = R"(
        #version 330 core

        layout(location = 0) in vec4 position;

        void main()
        {
            gl_Position = position;
        }
    )";
    std::string fragment_shader_src = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        void main()
        {
            color = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";
    unsigned int shader = CreateShader(vertex_shader_src, fragment_shader_src);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    glDeleteProgram(shader);
    return 0;
}
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

static GLuint compile_shader(unsigned int type, const std::string &source)
{
    auto id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char msg[length];
        glGetShaderInfoLog(id, length, &length, msg);
        auto type_str = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
        std::cout << "Failed to compile " << type_str << " shader\n";
        std::cout << msg << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int create_shader(const std::string &vert, const std::string &frag)
{
    GLuint program = glCreateProgram();
    GLuint vs = compile_shader(GL_VERTEX_SHADER, vert);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, frag);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void handle_err() {
    auto code = glGetError();
    if (code != GL_NO_ERROR) {
        auto str = gluErrorString(code); 
        std::cout << str << std::endl;
    }
}

int main(void)
{
    {
    auto result = glfwInit();
    if (result == GLFW_FALSE) {
        std::cout << "Failed to init GLFW" << std::endl;
        return -1;
    }
    }

    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    auto *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (window == nullptr)
    {
        const char* description;
        int code = glfwGetError(&description);

        if (description) {
            std::cout << description << std::endl;
            std::cout << code << std::endl;
        }
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    {
    auto result = glewInit();
    if (result != GLEW_OK)
        throw std::runtime_error("glew init error");
    }


    std::cout << glGetString(GL_VERSION) << std::endl;

    GLfloat positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f,  0.0f
    };


    // GLuint buffer;
    // glGenBuffers(1, &buffer);
    // glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    // glEnableVertexAttribArray(0);
    handle_err();

    std::string vert_src =
        "#version 410 core\n"
        "\n"
        "layout (location = 0) in vec3 pos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
        "}\n";
    std::string frag_src =
        "#version 330 core\n"
        "\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    const char *pvert = vert_src.c_str();
    const char *pfrag = frag_src.c_str();

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &pvert, NULL);
    glCompileShader(vert);

    GLuint frag = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(frag, 1, &pfrag, NULL);
    glCompileShader(frag);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vert);
    glAttachShader(shader_program, frag);

    glLinkProgram(shader_program);
    glDeleteShader(vert);
    glDeleteShader(frag);

    GLuint vbo;
    GLuint vao;

    // vao and vbo, order matters
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // unsigned int shader = create_shader(vert, frag);
    // glUseProgram(shader);

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        /* Render here */
        // glClear(GL_COLOR_BUFFER_BIT);

        // glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */

        /* Poll for and process events */
        glfwPollEvents();
    }

    // glDeleteProgram(shader);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

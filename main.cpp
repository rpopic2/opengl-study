// #include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static std::string vert_src =
"#version 330 core\n"
"\n"
"layout (location = 0) in vec3 pos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
"}\n";
static std::string frag_src =
"#version 330 core\n"
"\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   color = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n";


int main(void)
{
    {
    auto result = glfwInit();
    if (result == GLFW_FALSE) {
        std::cout << "Failed to init GLFW" << std::endl;
        return -1;
    }
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto *window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
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

    glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, 800, 800);

    std::cout << glGetString(GL_VERSION) << std::endl;


    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    const char *pvert = vert_src.c_str();
    glShaderSource(vert_shader, 1, &pvert, NULL);
    glCompileShader(vert_shader);

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *pfrag = frag_src.c_str();
    glShaderSource(frag_shader, 1, &pfrag, NULL);
    glCompileShader(frag_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);

    glLinkProgram(shader_program);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);


    GLfloat positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    GLuint vbo;
    GLuint vao;

    // vao and then vbo. order matters
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window))
    {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


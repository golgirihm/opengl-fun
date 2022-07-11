#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int compileShader(unsigned int type, const std::string& source)
{
    unsigned int id         = glCreateShader(type);
    const char* source_cstr = source.c_str();

    glShaderSource(id, 1, &source_cstr, nullptr);
    glCompileShader(id);

    int compileStatus;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program    = glCreateProgram();
    unsigned int vs         = compileShader(GL_VERTEX_SHADER,   vertexShader);
    unsigned int fs         = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // print the OpenGL version
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        // glewInit failed, something is seriously wrong.
        std::cerr << "glewInit() failed: " << glewGetErrorString(err) << std::endl;
    }

    constexpr uint16_t NUM_POSITIONS = 3;
    constexpr uint16_t NUM_DIMS = 2;
    float positions[NUM_DIMS * NUM_POSITIONS]
    {
        -0.5f,  0.5f,
         0.5f,  0.5f,
         0.0f, -0.5f,
    };

    unsigned int bufferID;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);

    glBufferData(GL_ARRAY_BUFFER, (NUM_DIMS * NUM_POSITIONS) * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, NUM_DIMS, GL_FLOAT, GL_FALSE, NUM_DIMS *sizeof(float), static_cast<void *>(0));

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location=0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n"
        ;

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location=0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}\n"
        ;

    unsigned int shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // draw a triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

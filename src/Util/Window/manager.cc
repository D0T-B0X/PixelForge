#include "Util/Window/manager.hh"

namespace chip8
{

namespace util
{

namespace window
{

void
Manager::framebuffer_resize_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

void
Manager::drop_callback(GLFWwindow* window, int count, const char** paths)
{
    // add safeguards to protect program from window manager shenanigans
    if (count > 0 && paths != nullptr && paths[0] != nullptr) {
        
        std::string payload(paths[0]);
        if (!payload.empty()) {
            gameROM = payload;
            pendingROM = true;
        }
    }
}

Manager::Manager()
 : window(nullptr), WINDOW_NAME("Chip-8"), 
   WINDOW_SCALE(15)
{
}

Manager::Manager(const char* name)
 : window(nullptr), WINDOW_NAME(name),
   WINDOW_SCALE(15)
{
}

void
Manager::initWindow()
{

    // Managerilize glfw with OpenGL 4.6 Core Profile
    if ( !glfwInit() ) {
        EPRINTF("FATAL: GLFW failed to initialize.\n")
        std::exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// compatibility for apple devices
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Window managers have a default minimum size limit
    // so we scale the window to a safe intial level
    int windowHeight = 32 * WINDOW_SCALE;
    int windowWidth = 64* WINDOW_SCALE;

    // Create a new glfw window
    window = glfwCreateWindow(windowWidth, windowHeight, WINDOW_NAME, NULL, NULL);
    if ( !window ) {
        glfwTerminate();
        EPRINTF("FATAL: Window creation failed.\n")
        std::exit(-1);
    }

    // set window pointer to "this"
    glfwSetWindowUserPointer(window, this);
    glfwMakeContextCurrent(window);

    // VSync
    glfwSwapInterval(1);

    // Load GLAD (Essential to use OpenGL helper functions)
    if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
        EPRINTF("FATAL: GLAD initialization failed.\n")
        std::exit(-1);
    }

    // Set an initial viewport
    glViewport(0, 0, windowWidth, windowHeight);

    // change the view port on user window size changes
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    // set drop callback for rom loads
    glfwSetDropCallback(window, drop_callback);

    // initliaze rendering resources
    initRendering();
}

void
Manager::cleanup()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool
Manager::shouldWindowClose() const
{
    return glfwWindowShouldClose(window);
}

void
Manager::swapWindowBuffers()
{
    glfwSwapBuffers(window);
}

void 
Manager::render(const Pixel* display_state, Byte width, Byte height, bool high_res)
{

    uint32_t* pixel_buffer;
    if (high_res) {
        pixel_buffer = high_res_pixel_buffer;
    } else {
        pixel_buffer = low_res_pixel_buffer;
    }

    // convert monochrome pixel data into RGBA pixels
    for (int i = 0; i < (width*height); ++i) {
        pixel_buffer[i] = display_state[i] ? FOREGROUND_COLOR : BACKGROUND_COLOR;
    }

    // clear background every frame
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // stream the generated texture to VRAM
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Reallocate and push the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel_buffer);

    // bufferless draw call for final frame render
    glUseProgram(shader_program);
    glBindVertexArray(vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
}

void
Manager::initRendering()
{
    shader_program = compileShader();

    // set the sampler uniform to texture unit 0
    glUseProgram(shader_program);
    GLint loc = glGetUniformLocation(shader_program, "screenTexture");
    if (loc != -1) {
        glUniform1i(loc, 0);
    }

    // generate vao for rendering
    glGenVertexArrays(1, &vertex_array_object);

    // create a texture buffer
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // strict nearest-neighbor filtering for pixelated edges
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint 
Manager::compileShader()
{
    // shaders have been hardcoded due to their simplicity and immutability
    const char* vertex_shader_source = R"(
        #version 460 core
        out vec2 TexCoords;
        void main() 
        {
            float x = -1.0 + float((gl_VertexID & 1) << 2);
            float y = -1.0 + float((gl_VertexID & 2) << 1);
            TexCoords.x = (x + 1.0) * 0.5;
            TexCoords.y = 1.0 - ((y + 1.0) * 0.5); 
            gl_Position = vec4(x, y, 0.0, 1.0);
        }
    )";

    const char* fragment_shader_source = R"(
        #version 460 core
        out vec4 FragColor;
        in vec2 TexCoords;
        uniform sampler2D screenTexture;
        void main()
        {
            FragColor = texture(screenTexture, TexCoords);
        }
    )";

    int success;
    char info_log[512];

    // compile vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        EPRINTF("FATAL: Vertex shader compilation failed.\n");
        EPRINTF(info_log);
        std::exit(-1);
    }

    // compile fragment shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        EPRINTF("FATAL: Fragment shader compilation failed.\n");
        EPRINTF(info_log);
        std::exit(-1);
    }

    // link shaders into a program
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);
    
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        EPRINTF("FATAL: Shader program linking failed.\n");
        EPRINTF(info_log);
        std::exit(-1);
    }

    // free shader memory
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program_id;
}

void
Manager::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

bool
Manager::isKeyPressed(int glfw_key_code) const
{
    return (glfwGetKey(window, glfw_key_code) == GLFW_PRESS);
}

const char*
Manager::getROMPath() const
{
    return gameROM.c_str();
}

} // namespace window
} // namespace util
} // namespace chip8

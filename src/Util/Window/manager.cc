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

    // Load GLAD (Essential to use OpenGL helper functions)
    if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
        EPRINTF("FATAL: GLAD initialization failed.\n")
        std::exit(-1);
    }

    // Set an initial viewport
    glViewport(0, 0, windowWidth, windowHeight);

    // change the view port on user window size changes
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
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

} // namespace window

} // namespace util

} // namespace chip8
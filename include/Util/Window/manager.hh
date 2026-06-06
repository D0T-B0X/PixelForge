#ifndef _UTIL_WINDOW_MANAGER_HH_
#define _UTIL_WINDOW_MANAGER_HH_

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "types.hh"

namespace chip8
{

namespace util
{

namespace window
{

class Manager
{
  private:
    const char*         WINDOW_NAME;
    const int           WINDOW_SCALE;   
    GLFWwindow*         window;

    /**
     * @brief Resize viewport on window resize by the user
     */
    static void framebuffer_resize_callback(GLFWwindow* window, int width, int height);

  public:
    Manager();
    Manager(const char* name);
    
    /**
     * @brief initialize the emulator window
     */
    void initWindow();   
    
    /**
     * @brief Cleanup memory on termination
     */
    void cleanup();

    /**
     * @brief Window runtime state information
     */
    bool shouldWindowClose() const;

    void swapWindowBuffers();

    /**
     * @brief Process emulator window specific input
     */
    void processInput();

    /**
     * @brief Process all keypad input
     * 
     * Keyboard to keypad mapping:
     *  
     * | 1 | 2 | 3 | 4 | -> | 1 | 2 | 3 | C |
     * |---|---|---|---| -> |---|---|---|---|
     * | Q | W | E | R | -> | 4 | 5 | 6 | D |
     * | A | S | D | F | -> | 7 | 8 | 9 | E |
     * | Z | X | C | V | -> | A | 0 | B | F |
     */
    bool isKeyPressed(int glfw_key_code) const;
};

} // namespace window
} // namespace util 
} // namespace chip8

#endif // _UTIL_WINDOW_MANAGER_HH_

#ifndef _UTIL_WINDOW_MANAGER_HH_
#define _UTIL_WINDOW_MANAGER_HH_

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "types.hh"

constexpr inline Byte LOW_RES_SCREEN_WIDTH = 64;
constexpr inline Byte LOW_RES_SCREEN_HEIGHT = 32;

constexpr inline Byte HIGH_RES_SCREEN_WIDTH = 128;
constexpr inline Byte HIGH_RES_SCREEN_HEIGHT = 64;

#define FOREGROUND_COLOR 0xFFFFFFFFu
#define BACKGROUND_COLOR 0x00000000u

namespace chip8
{

namespace util
{

namespace window
{

class Manager
{
  private:
    const char*                WINDOW_NAME;
    const int                  WINDOW_SCALE;   
    GLFWwindow*                window;

    static inline std::string  gameROM;

    // Two buffers to hold high res and low res textures
    uint32_t                   low_res_pixel_buffer[LOW_RES_SCREEN_WIDTH * LOW_RES_SCREEN_HEIGHT]; 
    uint32_t                   high_res_pixel_buffer[HIGH_RES_SCREEN_WIDTH * HIGH_RES_SCREEN_HEIGHT];
    GLuint                     texture_id;
    GLuint                     vertex_array_object;
    GLuint                     shader_program;

    /**
     * @brief Resize viewport on window resize by the user
     */
    static void framebuffer_resize_callback(GLFWwindow* window, int width, int height);

    /**
     * @brief Callback invoked on a rom drop
     * 
     * @param window Current window context
     * @param count The number of files dropped
     * @param paths Path to the dropped files
     */
    static void drop_callback(GLFWwindow* window, int count, const char** paths);

  public:
    Manager();
    Manager(const char* name);

    static inline bool pendingROM = false; 
    
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

    void render(const Pixel* display_state, Byte width, Byte height, bool high_res);

    void initRendering();

    GLuint compileShader();

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

    /**
     * @brief Returns the path of the ROM file dropped in the window by the user 
     * 
     * @return The path of the dropped ram
     */
    const char* getROMPath() const;
};

} // namespace window
} // namespace util 
} // namespace chip8

#endif // _UTIL_WINDOW_MANAGER_HH_

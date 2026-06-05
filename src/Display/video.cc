#include "Display/video.hh"

namespace chip8
{

namespace display
{

Video::Video()
{
    // Set all pixels in the display to 0 (OFF)
    for(Pixel& pixel : display) { pixel = 0; }

    windowManager.initWindow();
}

Video::Video(const DisplayColor& secondaryColor)
 : secondaryColor(secondaryColor)
{
    // Set all pixels in the display to 0 (OFF)
    for(Pixel& pixel : display) { pixel = 0; }

    windowManager.initWindow();
}

// ** TEST RUN ONLY: Main run loop will exist inside an application layer **
// Run the main loop 
void
Video::run()
{
    while( windowManager.shouldWindowClose() == false ) {
        windowManager.swapWindowBuffers();
        glfwPollEvents();
    }
}

} // namespace display

} // namespace chip8
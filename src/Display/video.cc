#include "Display/video.hh"

namespace chip8
{

namespace display
{

Video::Video()
{
    // Set all pixels in the display to 0 (OFF)
    for(Pixel& pixel : display) { pixel = 0; }
}

Video::Video(const DisplayColor& secondaryColor)
 : secondaryColor(secondaryColor)
{
    // Set all pixels in the display to 0 (OFF)
    for(Pixel& pixel : display) { pixel = 0; }
}

bool
Video::setPixel(Byte x_coord, Byte y_coord)
{
    // 1-D index of the pixel
    uint16_t display_cell = x_coord + (y_coord * 64);

    // original state of the pixel
    Pixel pixel_state = this->display[display_cell];

    // Switch the pixel's state
    this->display[display_cell] ^= 1;

    // if the pixel was originally ON, collision = true, else false
    return pixel_state; 
}

// Set all pixels to OFF
void
Video::clear()
{
    for(Pixel& pixel : display) { pixel = 0; }
}

} // namespace display

} // namespace chip8
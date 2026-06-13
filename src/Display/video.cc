#include "Display/video.hh"

namespace chip8
{

namespace display
{

Video::Video()
   // Initliaze in low res mode 
 : highResMode(false), display_width(LOW_RES_WIDTH), 
   display_height(LOW_RES_HEIGHT)
{
    // Set all pixels in the display to 0 (OFF)
    for(Pixel& pixel : display) { pixel = 0; }
}

Video::Video(const DisplayColor& secondaryColor)
 : secondaryColor(secondaryColor), highResMode(false),
   display_width(LOW_RES_WIDTH), display_height(LOW_RES_HEIGHT)
{
    // Set all pixels in the display to 0 (OFF)
    for(Pixel& pixel : display) { pixel = 0; }
}

bool
Video::setPixel(Byte x_coord, Byte y_coord)
{
    // Wrap coordinates to current logical bounds
    x_coord %= display_width;
    y_coord %= display_height;

    // 1-D index using dynamic pitch
    uint16_t display_cell = x_coord + (y_coord * display_width);

    // original state of the pixel
    Pixel pixel_state = this->display[display_cell];

    // Switch the pixel's state
    this->display[display_cell] ^= 1;

    // if the pixel was originally ON, collision = true, else false
    return pixel_state; 
}

Pixel*
Video::getDisplayState()
{
    return display;
}

// Set all pixels to OFF
void
Video::clear()
{
    for(Pixel& pixel : display) { pixel = 0; }
}

// Scroll the display down by N pixels
void
Video::scrollDown(Byte N)
{
    // Shift rows down. Iterate backwards to prevent overwriting data.
    for (int y = display_height - 1; y >= N; --y) {
        for (int x = 0; x < display_width; ++x) {
            display[y * display_width + x] = display[(y - N) * display_width + x];
        }
    }

    // blank out top new rows
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < display_width; ++x) {
            display[y * display_width + x] = 0;
        }
    }
}

// Scroll the screen by 4 pixels
void 
Video::scrollLeft()
{
    for (int y = 0; y < display_height; ++y) {
        // Shift pixels left by 4 columns
        for (int x = 0; x < display_width - 4; ++x) {
            display[y * display_width + x] = display[y * display_width + (x + 4)];
        }
        
        // Zero out the rightmost 4 columns
        for (int x = display_width - 4; x < display_width; ++x) {
            display[y * display_width + x] = 0;
        }
    }
}

void 
Video::scrollRight()
{
    for (int y = 0; y < display_height; ++y) {
        // Shift pixels right by 4 columns. Iterate backwards to prevent overwriting.
        for (int x = display_width - 1; x >= 4; --x) {
            display[y * display_width + x] = display[y * display_width + (x - 4)];
        }
        
        // Zero out the leftmost 4 columns
        for (int x = 0; x < 4; ++x) {
            display[y * display_width + x] = 0;
        }
    }
}

void 
Video::setHighRes()
{
    display_width = HIGH_RES_WIDTH;
    display_height = HIGH_RES_HEIGHT;
    highResMode = true;

    // clear the display on resolution change
    clear();
}

void 
Video::setLowRes()
{
    display_width = LOW_RES_WIDTH;
    display_height = LOW_RES_HEIGHT;
    highResMode = false;

    // clear the display on resolution change
    clear();
}

Byte
Video::getWidth()
{
    return display_width;
}

Byte
Video::getHeight()
{
    return display_height;
}

bool
Video::isHighRes() const
{
    return highResMode;
}

} // namespace display
} // namespace chip8
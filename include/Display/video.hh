#ifndef _DISPLAY_VIDEO_HH_
#define _DISPLAY_VIDEO_HH_

#include <cstdint>

#include "Util/Window/manager.hh"
#include "Display/fontset.hh"
#include "types.hh"

namespace chip8
{

namespace display
{

// Use this struct to define secondary color of the display
struct DisplayColor {
    Color red;
    Color green;
    Color blue;

    DisplayColor() 
     : red(85), green(255), blue(85)
    {
    }

    DisplayColor(Color r, Color g, Color b) 
     : red(r), green(g), blue(b)
    {
    }

};

class Video
{
  private:
    // Chip-8 has a 64x32 monochrome display
    Pixel                   display[64 * 32];
    DisplayColor            secondaryColor;

  public:
    Video();

    // Pass in the second color (first is always white)
    Video(const DisplayColor& secondaryColor);

    /**
     * Set the pixel to ON
     */
    bool setPixel(Byte x_coord, Byte y_coord);

    /**
     * Clear the display by setting all pixels to OFF (0)
     */
    void clear();
};

} // namespace display

} // namespace chip8

#endif // _DISPLAY_VIDEO_HH_
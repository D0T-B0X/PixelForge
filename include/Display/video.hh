#ifndef _DISPLAY_VIDEO_HH_
#define _DISPLAY_VIDEO_HH_

#include <cstdint>

#include "Display/fontset.hh"
#include "types.hh"

constexpr inline Byte HIGH_RES_WIDTH = 128;
constexpr inline Byte HIGH_RES_HEIGHT = 64;

constexpr inline Byte LOW_RES_WIDTH = 64;
constexpr inline Byte LOW_RES_HEIGHT = 32;

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
    Byte                    display_width;
    Byte                    display_height;

    // the emulator display must have memory to support high res mode
    Pixel                   display[HIGH_RES_WIDTH * HIGH_RES_HEIGHT];
    DisplayColor            secondaryColor;

    bool                    highResMode;

  public:
    Video();

    // Pass in the second color (first is always white)
    Video(const DisplayColor& secondaryColor);

    /**
     * Set the pixel to ON
     */
    bool setPixel(Byte x_coord, Byte y_coord);

    /**
     * Get the current vm screen pixel state
     */
    Pixel* getDisplayState();

    /**
     * Clear the display by setting all pixels to OFF (0)
     */
    void clear();

    /**
     * @brief Scroll the display down
     * 
     * @param N the number of pixels to scroll by
     */
    void scrollDown(Byte N);

    /**
     * @brief Scroll the display left by 4 pixels
     * 
     */
    void scrollLeft();

    /**
     * @brief Scroll the display right by 4 pixels
     * 
     */
    void scrollRight();

    /**
     * @brief Set the display to high resolution mode (128x64)
     * 
     */
    void setHighRes();

    /**
     * @brief Set the display to low resolution mode (64x32)
     * 
     */
    void setLowRes();

    /**
     * @brief Get the current display width
     * 
     */
    Byte getWidth();

    /**
     * @brief Get the current display height
     * 
     */
    Byte getHeight();

};

} // namespace display
} // namespace chip8

#endif // _DISPLAY_VIDEO_HH_

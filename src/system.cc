#include "system.hh"
#include <cstdlib>
#include <ctime>

namespace chip8
{

System::System()
 : cpu(
    this->ram, 
    this->display, 
    this->delay, 
    this->sound
   )
{
    // seed the RNG for instruction I_CXNN
    std::srand(std::time(nullptr));
    windowManager.initWindow();
}

void
System::run()
{
    while(!windowManager.shouldWindowClose()) {
        windowManager.swapWindowBuffers();
        glfwPollEvents();
    }
}

}
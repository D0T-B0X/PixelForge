#ifndef _CONFIG_HH_
#define _CONFIG_HH_

#include "types.hh"

namespace chip8{

struct EmulatorQuirks {
    bool shift_modifies_vx_in_place;    // 8XY6 / 8XYE
    bool load_store_leaves_i_unchanged; // FX55 / FX65
    bool jump_with_offset_vx;           // BNNN or BXNN
    bool clip_sprites_at_edges;         // DXYN
    
    EmulatorQuirks()
     : shift_modifies_vx_in_place(true), load_store_leaves_i_unchanged(true),
       jump_with_offset_vx(true), clip_sprites_at_edges(true) 
    { }
};

} // namespace chip8

#endif // _CONFIG_HH_

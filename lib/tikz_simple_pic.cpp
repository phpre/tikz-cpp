#include "tikz_simple_pic.h"

namespace TIKZ {
    std::string OUT_DIR, TEX_DIR;
    std::string FONT_PATH, COLOR_PATH, MACRO_PATH;
    std::string PROGRAM_NAME;

    std::deque<std::function<void( )>> SIMPLE_PICS{ };

    u64 add_simple_picture( std::function<void( )> p_f ) {
        SIMPLE_PICS.push_back( p_f );
        return SIMPLE_PICS.size( );
    }
} // namespace TIKZ

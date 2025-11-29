#include "tikz_default_pictures.h"
#include "tikz_string.h"

namespace TIKZ {
    void add_occurrences_pictures( document& p_out, const stylized_string& p_pname,
                                   const stylized_string&             p_tname,
                                   const std::deque<breakpoint_repn>& p_occs,
                                   occ_style_t                        p_style ) {
        for( u64 y = 0; y < p_occs.size( ); ++y ) {
            picture pic{ };
            place_highlighted_occurrence( pic, p_pname, tikz_point{ 0.0, -2.5 * CHAR_HEIGHT },
                                          p_tname, tikz_point{ 0.0, 0.0 }, p_occs, y, p_style );
            pic.place_text( math_mode( VSIZE_CORRECTION + p_pname.m_name ),
                            tikz_point{ -1 * CHAR_WIDTH, -3 * CHAR_HEIGHT } );
            pic.place_text( math_mode( VSIZE_CORRECTION + p_tname.m_name ),
                            tikz_point{ -1 * CHAR_WIDTH, -CHAR_HEIGHT / 2 } );
            p_out.add_picture( pic );
        }
    }
} // namespace TIKZ

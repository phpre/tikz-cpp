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

    void add_alignment_picture( document& p_out, const std::string& p_P, const std::string& p_T,
                                const cost_table& p_cost, alignment_style_t p_style ) {

        auto            tn = stylized_string{ p_T, "T",
                                   str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
        auto            pn = stylized_string{ p_P, "P",
                                   str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
        breakpoint_repn bp = compute_breakpoints( p_P, p_T, p_cost );

        add_alignment_picture( p_out, bp, tn, pn, p_style );
    }

    void add_alignment_picture( document& p_out, breakpoint_repn p_alignment,
                                stylized_string p_Tname, stylized_string p_Pname,
                                alignment_style_t p_style ) {
        picture pic{ };
        place_alignment( pic, p_Pname, tikz_point{ 0.0, 0.0 }, p_Tname, tikz_point{ 0.0, 1.25 },
                         p_alignment, p_style );
        p_out.add_picture( pic );
    }

} // namespace TIKZ

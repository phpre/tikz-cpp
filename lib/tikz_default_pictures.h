#pragma once
#include "tikz_document.h"
#include "tikz_string.h"

namespace TIKZ {
    void add_occurrences_pictures( document& p_out, const stylized_string& p_pname,
                                   const stylized_string&             p_tname,
                                   const std::deque<breakpoint_repn>& p_occs,
                                   occ_style_t p_style = occ_style_t::NO_ANNOTATION );

    void add_alignment_picture( document& p_out, const std::string& p_P, const std::string& p_T,
                                const cost_table& p_cost = { },
                                alignment_style_t p_style
                                = AT_COMPRESS | AT_SHOW_MATCHED_CHARACTERS );

    void add_alignment_picture( document& p_out, breakpoint_repn p_alignment,
                                stylized_string p_Tname, stylized_string p_Pname,
                                alignment_style_t p_style
                                = AT_COMPRESS | AT_SHOW_MATCHED_CHARACTERS );
} // namespace TIKZ

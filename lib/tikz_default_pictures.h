#pragma once
#include "tikz_document.h"
#include "tikz_string.h"

namespace TIKZ {
    void add_occurrences_pictures( document& p_out, const stylized_string& p_pname,
                                   const stylized_string&             p_tname,
                                   const std::deque<breakpoint_repn>& p_occs,
                                   occ_style_t p_style = occ_style_t::NO_ANNOTATION );
} // namespace TIKZ

#pragma once
#include <string>

#include "tikz_document.h"
#include "tikz_string.h"

namespace TIKZ {
    void add_string_picture( document& p_out, const std::string& p_string );

    void add_occurrences_pictures( document& p_out, const stylized_string& p_pname,
                                   const stylized_string&             p_tname,
                                   const std::deque<breakpoint_repn>& p_occs,
                                   occ_style_t p_style = occ_style_t::NO_ANNOTATION );

    void add_alignment_picture( document& p_out, const std::string& p_P, const std::string& p_T,
                                const cost_table& p_cost = { },
                                alignment_style_t p_style
                                = AT_COMPRESS | AT_SHOW_MATCHED_CHARACTERS );

    void add_alignment_picture( document& p_out, const std::string& p_P, const std::string& p_T,
                                breakpoint_repn   p_alignment,
                                alignment_style_t p_style
                                = AT_COMPRESS | AT_SHOW_MATCHED_CHARACTERS );

    void add_alignment_picture( document& p_out, breakpoint_repn p_alignment,
                                stylized_string p_Tname, stylized_string p_Pname,
                                alignment_style_t p_style
                                = AT_COMPRESS | AT_SHOW_MATCHED_CHARACTERS );

    void add_alignment_graph_picture( document& p_doc, const std::string& p_T,
                                      stylized_string p_Tname, const std::string& p_P,
                                      stylized_string p_Pname, fragmentco p_fragment,
                                      breakpoint_repn p_alignment, bool p_highlightMD = false );

    void add_alignment_graph_explanation_pictures( document&          p_doc,
                                                   const std::string& p_Tname    = "T",
                                                   const std::string& p_Pname    = "P",
                                                   const std::string& p_Tposname = "t",
                                                   const std::string& p_Pposname = "p" );

    void add_slices_pictures( document& p_doc, stylized_string p_Pname, stylized_string p_Tname,
                              fragmentco p_fragment, u64 p_d, breakpoint_repn p_alignment,
                              const std::string& p_P, const std::string& p_T,
                              bool p_simplePlabel = true );

    void add_trie_construction_pictures( document& p_doc, const std::string& p_alphabet,
                                         const std::deque<std::string>& p_str );
} // namespace TIKZ

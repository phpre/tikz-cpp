#pragma once
#include <string>

#include "alg_ag_slice.h"
#include "alg_breakpoint.h"
#include "alg_trie.h"
#include "defines.h"
#include "tikz_flags.h"
#include "tikz_graph.h"
#include "tikz_picture.h"
#include "tikz_stylized_string.h"

namespace TIKZ {
    color color_for_bp( const breakpoint& p_bp );

    void place_separator( picture& p_pic, tikz_point p_PtopLeft, tikz_point p_TtopLeft,
                          color p_color );

    void place_character_outline( picture& p_pic, tikz_point p_StopLeft, color p_color,
                                  const kv_store& p_options = { }, double p_corLeft = 0.0,
                                  double p_corTop = 0.0, double p_corRight = 0.0,
                                  double p_corBot = 0.0 );

    void place_character_sep_dots( picture& p_pic, tikz_point p_StopLeft, u64 p_length,
                                   color p_color, const kv_store& p_options = { } );
    void place_character_sep_dots_vertical( picture& p_pic, tikz_point p_StopLeft, u64 p_length,
                                            color p_color, const kv_store& p_options = { } );

    inline void place_character_highlight( picture& p_pic, tikz_point p_StopLeft, color p_color,
                                           color p_bgColor, const kv_store& p_options = { },
                                           u64 p_length = 1, u64 p_height = 1 ) {
        if( p_length > 1 ) {
            place_character_sep_dots( p_pic, p_StopLeft, p_length, p_bgColor,
                                      OPT::DRAW( p_bgColor ) | OPT::LINE_WIDTH( "1pt" )
                                          | p_options );
        } else if( p_height > 1 ) {
            place_character_sep_dots_vertical( p_pic, p_StopLeft, p_height, p_bgColor,
                                               OPT::DRAW( p_bgColor ) | OPT::LINE_WIDTH( "1pt" )
                                                   | p_options );
        }
        place_character_outline( p_pic, p_StopLeft, p_bgColor,
                                 OPT::DOUBLE( p_color ) | OPT::DOUBLE_DISTANCE( ".75pt" )
                                     | OPT::LINE_WIDTH( ".75pt" ) | p_options,
                                 0.0, 0.0, ( p_length - 1 ) * CHAR_WIDTH,
                                 ( p_height - 1 ) * -CHAR_HEIGHT );
        if( p_length > 1 ) {
            place_character_sep_dots( p_pic, p_StopLeft, p_length, p_color, p_options );
        } else if( p_height > 1 ) {
            place_character_sep_dots_vertical( p_pic, p_StopLeft, p_height, p_color, p_options );
        }
    }

    // Prints a string S
    void place_string( picture& p_pic, const stylized_string& p_S, tikz_point p_StopLeft,
                       const kv_store& p_options = { } );

    inline void place_string( picture& p_pic, const std::string& p_string, tikz_point p_StopLeft,
                              const kv_store& p_options = { } ) {
        auto tn = stylized_string{ p_string, EMPTY_STR,
                                   str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
        place_string( p_pic, tn, p_StopLeft, p_options );
    }

    void place_string_vertical( picture& p_pic, const stylized_string& p_S, tikz_point p_StopLeft,
                                const kv_store& p_options = { } );

    inline void place_string_vertical( picture& p_pic, const std::string& p_string,
                                       tikz_point p_StopLeft, const kv_store& p_options = { } ) {
        auto tn = stylized_string{ p_string, EMPTY_STR,
                                   str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
        place_string_vertical( p_pic, tn, p_StopLeft, p_options );
    }

    void place_string_sequence( picture& p_pic, const std::deque<stylized_string>& p_S,
                                tikz_point p_StopLeft, bool p_redrawFrame = false,
                                const kv_store& p_options = { } );

    void place_string_sequence_vertical( picture& p_pic, const std::deque<stylized_string>& p_S,
                                         tikz_point p_StopLeft, bool p_redrawFrame = false,
                                         const kv_store& p_options = { } );

    // assumes T is on top, P on bottom
    void place_matched_string_pair( picture& p_pic, const stylized_string& p_P,
                                    tikz_point p_PtopLeft, const stylized_string& p_T,
                                    tikz_point p_TtopLeft, color p_color, bool p_compress = false );

    void place_edit_cost( picture& p_pic, u64 p_cost, tikz_position p_position, color p_color,
                          color p_bgColor );

    typedef u8                  alignment_style_t;
    constexpr alignment_style_t AT_NONE                     = 0;
    constexpr alignment_style_t AT_PRINT_BREAKPOINTS        = ( 1 << 0 );
    constexpr alignment_style_t AT_PRINT_EXTRA_STRING_PARTS = ( 1 << 1 );
    constexpr alignment_style_t AT_COMPRESS                 = ( 1 << 2 );
    constexpr alignment_style_t AT_SHOW_MATCHED_CHARACTERS  = ( 1 << 4 );
    constexpr alignment_style_t AT_SHOW_EDIT_COST           = ( 1 << 5 );

    constexpr alignment_style_t AT_DEFAULT = AT_PRINT_BREAKPOINTS;
    constexpr alignment_style_t AT_OCCS_DEFAULT
        = AT_PRINT_EXTRA_STRING_PARTS | AT_COMPRESS | AT_SHOW_MATCHED_CHARACTERS;

    // Prints alignment P to T
    std::pair<tikz_point, tikz_point>
    place_alignment( picture& p_pic, const stylized_string& p_P, tikz_point p_PtopLeft,
                     const stylized_string& p_T, tikz_point p_TtopLeft,
                     const breakpoint_repn& p_brpnt, alignment_style_t p_style = AT_DEFAULT );

    enum class occ_style_t { NO_ANNOTATION, STARTING_POS, ALL_POS, OCC_FULL };
    void place_highlighted_occurrence( picture& p_pic, const stylized_string& p_P,
                                       tikz_point p_PtopLeft, const stylized_string& p_T,
                                       tikz_point                         p_TtopLeft,
                                       const std::deque<breakpoint_repn>& p_occs, u64 p_selectedOcc,
                                       occ_style_t p_occstyle = occ_style_t::NO_ANNOTATION );

    void place_alignment_graph_label( picture& p_pic, const stylized_string& p_Pname,
                                      const stylized_string& p_Tname,
                                      tikz_point p_gridTopLeft = tikz_point{ 0.0, 0.0 } );

    vertex_grid place_alignment_graph_simple( picture& p_pic, fragmentco p_fragP,
                                              fragmentco p_fragT,
                                              tikz_point p_gridCellSize
                                              = tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT },
                                              tikz_point p_gridTopLeft = tikz_point{ 0.0, 0.0 },
                                              color      p_color = COLOR_BLACK.deemphasize( ) );

    vertex_grid place_alignment_graph( picture& p_pic, const std::string& p_P, fragmentco p_fragP,
                                       const std::string& p_T, fragmentco p_fragT,
                                       tikz_point p_gridCellSize
                                       = tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT },
                                       tikz_point p_gridTopLeft = tikz_point{ 0.0, 0.0 },
                                       color      p_vertexColor = COLOR_BLACK.deemphasize( ) );

    void place_alignment_on_coordinates( picture& p_pic, const vertex_grid& p_vg,
                                         const breakpoint_repn& p_brpnt, bool p_singleStep = true,
                                         bool p_showCost = false );

    inline void place_weighted_alignment_on_coordinates( picture& p_pic, const vertex_grid& p_vg,
                                                         const breakpoint_repn& p_brpnt,
                                                         bool p_singleStep = true ) {
        place_alignment_on_coordinates( p_pic, p_vg, p_brpnt, p_singleStep, true );
    }

    void place_graph_slice_on_coordinates( picture& p_pic, const vertex_grid& p_vg,
                                           const graph_slice& p_gs, color p_color,
                                           color p_portalColor      = COLOR_NONE,
                                           color p_innerPortalColor = COLOR_NONE,
                                           color p_topColor         = COLOR_NONE,
                                           color p_bottomColor      = COLOR_NONE );

    void place_graph_slices_stylized( picture& p_pic, const std::deque<graph_slice>& p_gs,
                                      breakpoint_repn p_alignment,
                                      color  p_portalVertexColor      = COLOR_BLACK.deemphasize( ),
                                      color  p_innerPortalVertexColor = COLOR_NONE,
                                      color  p_topVertexColor         = SEP_COL.deemphasize( ),
                                      color  p_bottomVertexColor      = SEP_COL.deemphasize( ),
                                      color  p_pureColor              = MAT_COL.deemphasize( ),
                                      color  p_impureColor            = SUB_COL.deemphasize( ),
                                      color  p_leftColor    = COLOR_TEXT.deemphasize_strong( ),
                                      color  p_rightColor   = COLOR_TEXT.deemphasize_strong( ),
                                      double p_sliceSpacing = DEFAULT_SLICE_SPACING );

    void place_graph_slices( picture& p_pic, const std::deque<graph_slice>& p_gs,
                             breakpoint_repn p_alignment, const std::string& p_P,
                             const std::string& p_T,
                             color              p_portalVertexColor = COLOR_BLACK.deemphasize( ),
                             color  p_internalPortalVertexColor     = COLOR_BLACK.deemphasize( ),
                             color  p_topVertexColor                = SEP_COL.deemphasize( ),
                             color  p_bottomVertexColor             = SEP_COL.deemphasize( ),
                             double p_sliceSpacing                  = DEFAULT_SLICE_SPACING );

    void place_graph_slices_t_labels( picture& p_pic, const std::deque<graph_slice>& p_gs,
                                      const stylized_string& p_Tname,
                                      const stylized_string& p_Pname,
                                      bool                   p_labelEqualParts = false,
                                      double p_labelDistance = DEFAULT_SLICE_SPACING,
                                      double p_sliceSpacing  = DEFAULT_SLICE_SPACING );

    void place_graph_slices_p_labels( picture& p_pic, const std::deque<graph_slice>& p_gs, u64 p_d,
                                      bool   p_guideLines    = true,
                                      double p_labelDistance = DEFAULT_SLICE_SPACING,
                                      double p_sliceSpacing  = DEFAULT_SLICE_SPACING );
} // namespace TIKZ

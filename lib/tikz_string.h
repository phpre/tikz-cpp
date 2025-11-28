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

    // Prints a string S
    void place_string( picture& p_pic, const stylized_string& p_S, tikz_point p_StopLeft,
                       const kv_store& p_options = { } );

    void place_string_vertical( picture& p_pic, const stylized_string& p_S, tikz_point p_StopLeft,
                                const kv_store& p_options = { } );

    // assumes T is on top, P on bottom
    void place_matched_string_pair( picture& p_pic, const stylized_string& p_P,
                                    tikz_point p_PtopLeft, const stylized_string& p_T,
                                    tikz_point p_TtopLeft, color p_color, bool p_compress = false );

    // Prints alignment P to T
    std::pair<tikz_point, tikz_point>
    place_alignment( picture& p_pic, const stylized_string& p_P, tikz_point p_PtopLeft,
                     const stylized_string& p_T, tikz_point p_TtopLeft,
                     const breakpoint_repn& p_brpnt, bool p_printBreakpoints = true,
                     bool p_printExtraStringParts = false, bool p_compress = false,
                     bool p_showMatchedCharacters = false );

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
                                         const breakpoint_repn& p_brpnt, bool p_singleStep = true );

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

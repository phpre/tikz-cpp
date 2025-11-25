#pragma once
#include <string>

#include "alg_ag_slice.h"
#include "alg_breakpoint.h"
#include "alg_trie.h"
#include "defines.h"
#include "tikz_graph.h"
#include "tikz_picture.h"
#include "tikz_stylized_string.h"

namespace TIKZ {
    constexpr double CHAR_WIDTH  = .5;
    constexpr double CHAR_HEIGHT = .5;
    constexpr double CHAR_GLOW   = .075;

    constexpr color MAT_COL = COLOR_C1;
    constexpr color DEL_COL = COLOR_C3;
    constexpr color INS_COL = COLOR_C4;
    constexpr color SUB_COL = COLOR_C5;
    constexpr color SEP_COL = COLOR_C2;

    color color_for_bp( const breakpoint& p_bp );

    void place_separator( picture& p_pic, tikz_point p_PtopLeft, tikz_point p_TtopLeft,
                          color p_color );

    // Prints a string S
    void place_string( picture& p_pic, const stylized_string& p_S, tikz_point p_StopLeft );

    void place_string_vertical( picture& p_pic, const stylized_string& p_S, tikz_point p_StopLeft );

    // assumes T is on top, P on bottom
    void place_matched_string_pair( picture& p_pic, const stylized_string& p_P,
                                    tikz_point p_PtopLeft, const stylized_string& p_T,
                                    tikz_point p_TtopLeft, color p_color, bool p_compress = false );

    // Prints alignment P to T
    std::pair<tikz_point, tikz_point>
    place_alignment( picture& p_pic, const stylized_string& p_P, tikz_point p_PtopLeft,
                     const stylized_string& p_T, tikz_point p_TtopLeft,
                     const breakpoint_repn& p_brpnt, bool p_printBreakpoints = true,
                     bool p_printExtraStringParts = false, bool p_compress = false );

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

    constexpr double DEFAULT_SLICE_SPACING = 0.25;
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

    typedef std::pair<u64, u64> trie_point;
    constexpr trie_point        TRIE_ROOT = { 0, 0 };

    struct placed_trie_vertex {
        bool                m_marked;
        u64                 m_height;
        tikz_point          m_pos{ 0.0, 0.0 };
        std::string         m_name;
        std::map<char, u64> m_next;
        trie_point          m_parent;
    };

    class placed_trie {
      public:
        typedef std::map<trie_point, placed_trie_vertex> placed_trie_vertices_t;

      private:
        placed_trie_vertices_t _vertices;
        tikz_point             _topLeft;
        double                 _distX;
        double                 _distY;
        std::string            _name;

      public:
        inline placed_trie( const trie& p_trie, tikz_point p_topLeft = { .0, .0 },
                            double p_distX = 2.5 * CHAR_WIDTH, double p_distY = -1.5 * CHAR_HEIGHT,
                            const std::string& p_name = EMPTY_STR );

        inline tikz_point vertex_position( u64 p_depth, u64 p_height ) const {
            return _topLeft + tikz_point{ p_depth * _distX, p_height * _distY };
        }

        inline placed_trie_vertex operator[]( trie_point p_point ) const {
            return _vertices.at( p_point );
        }

        inline trie_point next( trie_point p_point, char p_char ) const {
            const auto& vtx = ( *this )[ p_point ];
            auto        j   = vtx.m_next.at( p_char );
            return { p_point.first + 1, j };
        }

        inline std::deque<trie_point> in_order( trie_point p_start = TRIE_ROOT ) {
            std::deque<trie_point> res{ p_start };
            for( const auto& [ c, n ] : ( *this )[ p_start ].m_next ) {
                res.append_range( in_order( { p_start.first + 1, n } ) );
            }
            return res;
        }
    };

    placed_trie place_trie_coordinates( picture& p_pic, const trie& p_trie,
                                        tikz_point         p_topLeft = { .0, .0 },
                                        double             p_distX   = 2.5 * CHAR_WIDTH,
                                        double             p_distY   = -1.5 * CHAR_HEIGHT,
                                        const std::string& p_name    = EMPTY_STR );

    placed_trie place_trie_vertices( picture& p_pic, const trie& p_trie,
                                     tikz_point         p_topLeft = { .0, .0 },
                                     double             p_distX   = 2.5 * CHAR_WIDTH,
                                     double             p_distY   = -1.5 * CHAR_HEIGHT,
                                     const std::string& p_name    = EMPTY_STR );

    void place_trie_string_on_coordinates( picture& p_pic, const trie& p_trie,
                                           const std::string& p_string,
                                           const std::string& p_name    = EMPTY_STR,
                                           const kv_store&    p_options = { } );

    placed_trie place_trie( picture& p_pic, const trie& p_trie, tikz_point p_topLeft = { .0, .0 },
                            double p_distX = 2.5 * CHAR_WIDTH, double p_distY = -1.5 * CHAR_HEIGHT,
                            const std::string& p_name    = EMPTY_STR,
                            const kv_store&    p_options = { } );
} // namespace TIKZ

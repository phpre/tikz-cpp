#include <deque>
#include <map>

#include "tikz_command.h"
#include "tikz_graph.h"
#include "tikz_option.h"
#include "tikz_picture.h"
#include "tikz_point.h"
#include "tikz_string.h"
#include "tikz_stylized_string.h"

namespace TIKZ {

    void place_trie_vertex( picture& p_pic, tikz_position p_pos, const std::string& p_name,
                            bool p_marked ) {
        auto selected_inner
            = vertex::unselected_vertex( COLOR_FILL_WHITE, .875, .5, vertex::ST_CROSS );
        auto vertex = p_marked ? vertex::marked_vertex( COLOR_TEXT, 2, .75, vertex::ST_CIRCLE,
                                                        selected_inner )
                               : vertex::unselected_vertex( );
        place_vertex( p_pic, p_pos, vertex, p_name );
    }

    void place_trie_edge( picture& p_pic, tikz_position p_start, tikz_position p_end,
                          bool p_horizontal, const stylized_string& p_label,
                          tikz_point p_labelPos ) {
        if( p_horizontal ) {
            p_pic.place_line( p_start, p_end,
                              OPT::DRAW( COLOR_FILL_WHITE ) | OPT::DOUBLE( COLOR_TEXT )
                                  | OPT::DOUBLE_DISTANCE );

        } else {
            p_pic.place_vh_line( p_start, p_end,
                                 OPT::DRAW( COLOR_FILL_WHITE ) | OPT::DOUBLE( COLOR_TEXT )
                                     | OPT::DOUBLE_DISTANCE | OPT::ROUNDED_CORNERS );
        }
        place_string( p_pic, p_label, p_labelPos );
    }

    void place_trie( picture& p_pic, const trie& p_trie, tikz_point p_topLeft, double p_distX,
                     double p_distY, const std::string& p_name ) {
        // ensure to place strings in lexicographically sorted order
        u64             i = 0;
        std::deque<u64> cur{ 0 };

        std::map<std::pair<u64, u64>, std::string> names{ { { 0, 0 }, p_name + "_" } };
        std::map<std::pair<u64, u64>, u64>         heights{ { { 0, 0 }, 0 } };

        place_trie_vertex( p_pic, p_topLeft, names[ { 0, 0 } ],
                           p_trie.m_vertices[ 0 ][ 0 ].m_marked );
        for( ; i < p_trie.m_vertices.size( ); ++i ) {
            std::deque<u64> next{ };
            for( u64 j : cur ) {
                const auto& vtx  = p_trie.m_vertices[ i ][ j ];
                u64         hcur = 0;
                for( const auto& [ c, t ] : vtx.m_next ) {
                    next.push_back( t );
                    names[ { i + 1, t } ]   = names[ { i, j } ] + c;
                    heights[ { i + 1, t } ] = heights[ { i, j } ] + hcur;

                    // place vertex
                    place_trie_vertex(
                        p_pic,
                        p_topLeft
                            + tikz_point{ ( i + 1 ) * p_distX, heights[ { i + 1, t } ] * p_distY },
                        names[ { i + 1, t } ], p_trie.m_vertices[ i + 1 ][ t ].m_marked );

                    // draw edge
                    place_trie_edge(
                        p_pic, names[ { i, j } ], names[ { i + 1, t } ], !hcur,
                        stylized_string{ std::string{ c } },
                        p_topLeft
                            + tikz_point{ ( i + .5 ) * p_distX - CHAR_WIDTH / 2,
                                          heights[ { i + 1, t } ] * p_distY + CHAR_HEIGHT / 2 } );
                    hcur += p_trie.m_vertices[ i + 1 ][ t ].m_size;
                }
            }
            cur = next;
        }
    }
} // namespace TIKZ

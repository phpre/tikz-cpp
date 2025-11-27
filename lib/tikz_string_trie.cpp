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
    placed_trie::placed_trie( const trie& p_trie, tikz_point p_topLeft, double p_distX,
                              double p_distY, const std::string& p_name )
        : _topLeft{ p_topLeft }, _distX{ p_distX }, _distY{ p_distY }, _name{ p_name } {
        auto names   = p_trie.names( p_name );
        auto heights = p_trie.heights( );

        _vertices = placed_trie_vertices_t{ };

        for( const auto& [ p, h ] : heights ) {
            auto        pos = vertex_position( p.first, h );
            const auto& vtx = p_trie.m_vertices[ p.first ][ p.second ];
            _vertices[ p ]  = { vtx.m_marked, h, pos, names[ p ], vtx.m_next, TRIE_ROOT };
            _depth          = std::max( _depth, p.first );
        }

        for( const auto& [ p, h ] : heights ) {
            for( const auto& [ c, n ] : ( *this )[ p ].m_next ) {
                _vertices[ { p.first + 1, n } ].m_parent = p;
            }
        }
    }

    void place_trie_vertex( picture& p_pic, const placed_trie_vertex& p_vertex,
                            const kv_store& p_options = { } ) {
        auto selected_inner
            = vertex::unselected_vertex( COLOR_FILL_WHITE, .875, .5, vertex::ST_CROSS );
        auto vertex = p_vertex.m_marked ? vertex::marked_vertex( COLOR_TEXT, 2, .75,
                                                                 vertex::ST_CIRCLE, selected_inner )
                                        : vertex::unselected_vertex( );
        place_vertex( p_pic, p_vertex.m_pos, vertex, p_vertex.m_name, p_options );
    }

    void place_trie_edge( picture& p_pic, placed_trie_vertex p_start, placed_trie_vertex p_end,
                          const stylized_string& p_label, const kv_store& p_options ) {
        if( p_start.m_pos.m_y == p_end.m_pos.m_y ) {
            p_pic.place_line( p_start.m_name, p_end.m_name,
                              OPT::double_arrow( OPT::DRAW( COLOR_TEXT ) | p_options )
                                  | OPT::DRAW( COLOR_FILL_WHITE ) );

        } else {
            p_pic.place_vh_line( p_start.m_name, p_end.m_name,
                                 OPT::double_arrow( OPT::DRAW( COLOR_TEXT ) | p_options )
                                     | OPT::DRAW( COLOR_FILL_WHITE ) | OPT::ROUNDED_CORNERS );
        }
        auto labelpos = tikz_point{ ( p_start.m_pos.m_x + p_end.m_pos.m_x ) / 2, p_end.m_pos.m_y }
                        + tikz_point{ -CHAR_WIDTH * .65, CHAR_HEIGHT / 2 };
        place_string( p_pic, p_label, labelpos, p_options );
    }

    placed_trie place_trie_coordinates( picture& p_pic, const trie& p_trie, tikz_point p_topLeft,
                                        double p_distX, double p_distY,
                                        const std::string& p_name ) {
        placed_trie res{ p_trie, p_topLeft, p_distX, p_distY, p_name };
        for( const auto& p : res.in_order( ) ) {
            p_pic.place_coordinate( res[ p ].m_pos, res[ p ].m_name );
        }
        return res;
    }

    placed_trie place_trie_vertices( picture& p_pic, const trie& p_trie, tikz_point p_topLeft,
                                     double p_distX, double p_distY, const std::string& p_name ) {
        placed_trie res{ p_trie, p_topLeft, p_distX, p_distY, p_name };
        for( const auto& p : res.in_order( ) ) { place_trie_vertex( p_pic, res[ p ] ); }
        return res;
    }

    void place_trie_string_on_coordinates( picture& p_pic, const placed_trie& p_trie,
                                           const std::string& p_string,
                                           const kv_store&    p_options ) {
        trie_point pos = TRIE_ROOT;
        for( u64 i = 0; i < p_string.size( ); ++i ) {
            place_trie_vertex( p_pic, p_trie[ pos ], p_options );
            auto n     = p_trie.next( pos, p_string[ i ] );
            auto label = stylized_string{ std::string{ p_string[ i ] } };
            place_trie_edge( p_pic, p_trie[ pos ], p_trie[ n ], label, p_options );
            pos = n;
        }
        place_trie_vertex( p_pic, p_trie[ pos ], p_options );
    }

    void place_trie_depth_labels( picture& p_pic, const placed_trie& p_trie, u64 p_max,
                                  tikz_point p_labelTopLeft, const kv_store& p_options ) {
        for( u64 d = 0; d <= p_max; ++d ) {
            auto        pos   = p_labelTopLeft + p_trie.vertex_position( d, 0 );
            std::string label = textsize_footnotesize( VSIZE_CORRECTION + std::to_string( d ) );
            p_pic.place_text( label, pos, p_options );
        }
    }

    void place_trie_depth_labels( picture& p_pic, const placed_trie& p_trie,
                                  tikz_point p_labelTopLeft, const kv_store& p_options ) {
        place_trie_depth_labels( p_pic, p_trie, p_trie.depth( ), p_labelTopLeft, p_options );
    }

    placed_trie place_trie( picture& p_pic, const trie& p_trie, tikz_point p_topLeft,
                            const std::string& p_name, double p_distX, double p_distY,
                            const kv_store& p_options ) {
        auto trie = place_trie_vertices( p_pic, p_trie, p_topLeft, p_distX, p_distY, p_name );
        auto vtcs = trie.in_order( );

        for( auto p = vtcs.rbegin( ); *p != TRIE_ROOT; ++p ) {
            auto label = stylized_string{ std::string{ *trie[ *p ].m_name.rbegin( ) } };
            place_trie_edge( p_pic, trie[ trie[ *p ].m_parent ], trie[ *p ], label, p_options );
        }
        return trie;
    }
} // namespace TIKZ

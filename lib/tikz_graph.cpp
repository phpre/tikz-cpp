#include <format>
#include <string>

#include "tikz_command.h"
#include "tikz_graph.h"
#include "tikz_option.h"

namespace TIKZ {
    kv_store vertex::compile( ) const {
        auto res = vertex::shape_string( m_shape )
                   | OPT::INNER_SEP( std::format( "{:5.3f}pt", m_innerSep ) )
                   | OPT::OUTER_SEP( std::format( "{:5.3f}pt", m_outerSep ) );

        if( m_vertexType & vertex::VT_FILL ) { res = res | OPT::FILL( m_fillColor ); }
        if( m_vertexType & vertex::VT_DRAW ) {
            res = res | OPT::DRAW( m_drawColor )
                  | OPT::LINE_WIDTH( std::format( "{:5.3f}pt", m_lineWidth ) );
        }
        if( !m_inner.empty( ) ) {
            res = res
                  | tikz_option{ "path picture", EMPTY_STR,
                                 "{ \\node[" + m_inner.to_string( ) + "] {}; }",
                                 m_inner.libraries( ), m_inner.packages( ) };
        }

        return res;
    }

    void vertex_grid::place_coordinates( picture& p_pic, u64 p_sizeX, u64 p_sizeY, s32 p_startX,
                                         s32 p_startY, const kv_store& p_options ) const {
        for( u64 x = 0; x < p_sizeX; ++x ) {
            for( u64 y = 0; y < p_sizeY; ++y ) {
                p_pic.place_coordinate( this->point_for_pos( p_startX + x, p_startY + y ),
                                        this->label_for_pos( p_startX + x, p_startY + y ),
                                        p_options );
            }
        }
    }

    void vertex_grid::place_vertices_on_coordinates( picture& p_pic, const vertex& p_vertex,
                                                     u64 p_sizeX, u64 p_sizeY, s32 p_startX,
                                                     s32             p_startY,
                                                     const kv_store& p_options ) const {
        for( u64 x = 0; x < p_sizeX; ++x ) {
            for( u64 y = 0; y < p_sizeY; ++y ) {
                place_vertex( p_pic, this->label_for_pos( p_startX + x, p_startY + y ), p_vertex,
                              EMPTY_STR, p_options );
            }
        }
    }

    void vertex_grid::place_vertex_on_coordinates( picture& p_pic, const vertex& p_vertex,
                                                   s32 p_posX, s32 p_posY,
                                                   const kv_store& p_options ) const {
        place_vertex( p_pic, this->label_for_pos( p_posX, p_posY ), p_vertex, EMPTY_STR,
                      p_options );
    }

    void vertex_grid::place_diagonal_on_coordinates( picture& p_pic, const vertex& p_vertex,
                                                     u64 p_length, s32 p_startX, s32 p_startY,
                                                     const kv_store& p_options ) const {
        for( u64 i = 0; i < p_length; ++i ) {
            place_vertex( p_pic, this->label_for_pos( p_startX + i, p_startY + i ), p_vertex,
                          EMPTY_STR, p_options );
        }
    }

    void vertex_grid::place_new_vertex( picture& p_pic, const vertex& p_vertex, s32 p_posX,
                                        s32 p_posY, const kv_store& p_options ) const {
        place_vertex( p_pic, this->point_for_pos( p_posX, p_posY ), p_vertex,
                      this->label_for_pos( p_posX, p_posY ), p_options );
    }

    void vertex_grid::place_vertices( picture& p_pic, const vertex& p_vertex, u64 p_sizeX,
                                      u64 p_sizeY, s32 p_startX, s32 p_startY,
                                      const kv_store& p_options ) const {
        for( u64 x = 0; x < p_sizeX; ++x ) {
            for( u64 y = 0; y < p_sizeY; ++y ) {
                place_vertex( p_pic, this->point_for_pos( p_startX + x, p_startY + y ), p_vertex,
                              this->label_for_pos( p_startX + x, p_startY + y ), p_options );
            }
        }
    }

    void vertex_grid::place_diagonal( picture& p_pic, const vertex& p_vertex, u64 p_length,
                                      s32 p_startX, s32 p_startY,
                                      const kv_store& p_options ) const {
        for( u64 i = 0; i < p_length; ++i ) {
            place_vertex( p_pic, this->point_for_pos( p_startX + i, p_startY + i ), p_vertex,
                          this->label_for_pos( p_startX + i, p_startY + i ), p_options );
        }
    }

    void place_vertex( picture& p_pic, tikz_position p_position, const vertex& p_vertex,
                       const std::string& p_name, const kv_store& p_options ) {
        p_pic.place_node( p_position, EMPTY_STR, p_vertex.compile( ) | p_options, p_name );
    }
} // namespace TIKZ

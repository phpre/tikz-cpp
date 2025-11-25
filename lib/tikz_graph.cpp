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

    void place_selected_arrow( picture& p_pic, tikz_position p_startPos, tikz_position p_endPos,
                               color p_lineColor, color p_fillColor, double p_angle,
                               const kv_store& p_options ) {
        auto opt = OPT::ROTATE( std::format( "{:5.3f}", p_angle ) ) | OPT::DRAW( p_fillColor )
                   | OPT::DOUBLE( p_lineColor ) | OPT::DOUBLE_DISTANCE( ".75pt" )
                   | OPT::FILL( p_fillColor ) | OPT::ROUNDED_CORNERS( ".2pt" ) | p_options;
        p_pic.add_library( "calc" );

        std::deque<std::shared_ptr<path_operation>> path{ };
        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_endPos.to_string( ) + ") + (-6.5pt, 3.5pt)$" ) );
        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_endPos.to_string( ) + ") + (-6.5pt, 1.25pt)$" ) );
        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_startPos.to_string( ) + ") + (2.75pt, 1.25pt)$" ) );

        path.emplace_back( std::make_shared<arc_to_operation>( 25.0, 333.5, 3.0 ) );

        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_startPos.to_string( ) + ") + (2.75pt, -1.25pt)$" ) );
        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_endPos.to_string( ) + ") + (-6.5pt, -1.25pt)$" ) );
        path.emplace_back( std::make_shared<line_to_operation>(
            std::string{ "$(" } + p_endPos.to_string( ) + ") + (-6.5pt, -3.5pt)$" ) );
        path.emplace_back( std::make_shared<line_to_operation>( tikz_position{ } ) );

        p_pic.add_command( std::make_shared<path_command>(
            std::string{ "$(" } + p_endPos.to_string( ) + ") + (-.5pt, 0pt)$", path, opt ) );
    }

    void place_arrow( picture& p_pic, tikz_position p_startPos, tikz_position p_endPos,
                      const kv_store& p_options, const kv_store& p_basicOptions ) {
        p_pic.place_line( p_startPos, p_endPos, p_basicOptions | p_options );
    }
} // namespace TIKZ

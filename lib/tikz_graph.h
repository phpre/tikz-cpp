#pragma once
#include <string>

#include "tikz_option.h"
#include "tikz_picture.h"

namespace TIKZ {
    struct vertex {
        enum vertex_t {
            VT_NONE = 0,
            VT_DRAW = ( 1 << 1 ),
            VT_FILL = ( 1 << 2 ),
        } m_vertexType
            = VT_NONE;

        enum shape_t {
            ST_CIRCLE = 0,
            ST_RECTANGLE,
            ST_TRIANGLE_UP,
            ST_TRIANGLE_DOWN,
        } m_shape;

        color m_drawColor = COLOR_BLACK;
        color m_fillColor = COLOR_BLACK;

        double m_lineWidth = 0.0;
        double m_innerSep  = 0.0;
        double m_outerSep  = 0.0;

        kv_store m_inner = { };

        static inline kv_store shape_string( shape_t p_shape ) {
            switch( p_shape ) {
            default:
            case ST_CIRCLE: return OPT::CIRCLE;
            case ST_RECTANGLE: return OPT::RECTANGLE | OPT::ROUNDED_CORNERS( ".85pt" );
            case ST_TRIANGLE_DOWN:
                return OPT::REGULAR_TRIANGLE | OPT::ROUNDED_CORNERS( ".85pt" )
                       | OPT::SHAPE_BORDER_ROTATE( "180" );
            case ST_TRIANGLE_UP: return OPT::REGULAR_TRIANGLE | OPT::ROUNDED_CORNERS( ".85pt" );
            }
        }

        static constexpr vertex unselected_vertex( color  p_color  = COLOR_TEXT,
                                                   double p_radius = 1.0, double p_margin = .75,
                                                   shape_t p_shape = vertex::ST_CIRCLE ) {
            vertex res{ };

            res.m_vertexType = vertex::VT_FILL;
            res.m_shape      = p_shape;
            res.m_fillColor  = p_color;
            res.m_innerSep   = p_radius;
            res.m_outerSep   = p_margin;

            return res;
        }

        static constexpr vertex
        marked_vertex( color p_color = COLOR_TEXT, double p_radius = 1.25, double p_margin = .75,
                       shape_t       p_shape = vertex::ST_CIRCLE,
                       const vertex& p_inner = unselected_vertex( COLOR_FILL_WHITE, .55 ) ) {
            vertex res{ };

            res.m_vertexType = vertex::VT_FILL;
            res.m_shape      = p_shape;
            res.m_fillColor  = p_color;
            res.m_innerSep   = p_radius;
            res.m_outerSep   = p_margin;
            res.m_inner      = p_inner.compile( );

            return res;
        }

        kv_store compile( ) const;
    };

    struct vertex_grid {
      private:
        tikz_point  m_topLeft;
        tikz_point  m_distance;
        std::string m_labelPrefix;

      public:
        vertex_grid( tikz_point p_topLeft = { 0.0, 0.0 }, tikz_point p_distance = { 1.0, 1.0 },
                     const std::string& p_labelPrefix = EMPTY_STR )
            : m_topLeft{ p_topLeft }, m_distance{ p_distance }, m_labelPrefix{ p_labelPrefix } {
        }

        inline std::string label_for_pos( s32 p_posX, s32 p_posY ) const {
            if( m_labelPrefix != EMPTY_STR ) {
                return m_labelPrefix + "_" + std::to_string( p_posX ) + "_"
                       + std::to_string( p_posY );
            } else {
                return std::to_string( p_posX ) + "_" + std::to_string( p_posY );
            }
        }

        inline tikz_point point_for_pos( s32 p_posX, s32 p_posY ) const {
            return m_topLeft + ( tikz_point{ m_distance.m_x, 0 } * p_posX )
                   + ( tikz_point{ 0, m_distance.m_y } * p_posY );
        }

        void place_coordinates( picture& p_pic, u64 p_sizeX, u64 p_sizeY, s32 p_startX = 0,
                                s32 p_startY = 0, const kv_store& p_options = { } ) const;

        void place_vertex_on_coordinates( picture& p_pic, const vertex& p_vertex, s32 p_posX = 0,
                                          s32 p_posY = 0, const kv_store& p_options = { } ) const;

        void place_vertices_on_coordinates( picture& p_pic, const vertex& p_vertex, u64 p_sizeX,
                                            u64 p_sizeY, s32 p_startX = 0, s32 p_startY = 0,
                                            const kv_store& p_options = { } ) const;

        void place_diagonal_on_coordinates( picture& p_pic, const vertex& p_vertex, u64 p_length,
                                            s32 p_startX = 0, s32 p_startY = 0,
                                            const kv_store& p_options = { } ) const;

        void place_new_vertex( picture& p_pic, const vertex& p_vertex, s32 p_posX = 0,
                               s32 p_posY = 0, const kv_store& p_options = { } ) const;

        void place_vertices( picture& p_pic, const vertex& p_vertex, u64 p_sizeX, u64 p_sizeY,
                             s32 p_startX = 0, s32 p_startY = 0,
                             const kv_store& p_options = { } ) const;

        void place_diagonal( picture& p_pic, const vertex& p_vertex, u64 p_length, s32 p_startX = 0,
                             s32 p_startY = 0, const kv_store& p_options = { } ) const;
    };

    void place_vertex( picture& p_pic, tikz_position p_position, const vertex& p_vertex,
                       const std::string& p_name = EMPTY_STR, const kv_store& p_options = { } );

    void place_arrow( picture& p_pic, tikz_position p_startPos, tikz_position p_endPos,
                      const kv_store& p_options = { },
                      const kv_store& p_basicOptions
                      = OPT::LW_SUPPORT_LINE | OPT::DRAW( COLOR_TEXT ) | OPT::ARR_TIP_LATEX );

    void place_selected_arrow( picture& p_pic, tikz_position p_startPos, tikz_position p_endPos,
                               color p_lineColor, color p_fillColor, double p_angle = 0,
                               const kv_store& p_options = { } );

} // namespace TIKZ

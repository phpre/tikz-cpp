#pragma once

#include <cstdio>
#include <string>
#include "tikz.h"

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

        std::string m_inner = EMPTY_STR;

        static inline std::string shape_string( shape_t p_shape ) {
            switch( p_shape ) {
            default:
            case ST_CIRCLE: return "circle";
            case ST_RECTANGLE: return "rectangle, rounded corners=.85pt";
            case ST_TRIANGLE_DOWN:
                return "regular polygon, regular polygon sides=3, shape border rotate=180, rounded corners=.85pt";
            case ST_TRIANGLE_UP:
                return "regular polygon,regular polygon sides=3, rounded corners=.85pt";
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
            res.m_inner      = p_inner.to_string( );

            return res;
        }

        std::string to_string( ) const;
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

        void print_coordinates( FILE* p_out, u32 p_sizeX, u32 p_sizeY, s32 p_startX = 0,
                                s32 p_startY = 0, const std::string& p_extraOptions = EMPTY_STR,
                                u32 p_startIndent = 1, u32 p_indent = 4 ) const;

        void print_vertex_on_coordinates( FILE* p_out, const vertex& p_vertex, s32 p_posX = 0,
                                          s32                p_posY         = 0,
                                          const std::string& p_extraOptions = EMPTY_STR,
                                          u32 p_startIndent = 1, u32 p_indent = 4 ) const;

        void print_vertices_on_coordinates( FILE* p_out, const vertex& p_vertex, u32 p_sizeX,
                                            u32 p_sizeY, s32 p_startX = 0, s32 p_startY = 0,
                                            const std::string& p_extraOptions = EMPTY_STR,
                                            u32 p_startIndent = 1, u32 p_indent = 4 ) const;

        void print_diagonal_on_coordinates( FILE* p_out, const vertex& p_vertex, u32 p_length,
                                            s32 p_startX = 0, s32 p_startY = 0,
                                            const std::string& p_extraOptions = EMPTY_STR,
                                            u32 p_startIndent = 1, u32 p_indent = 4 ) const;

        void print_new_vertex( FILE* p_out, const vertex& p_vertex, s32 p_posX = 0, s32 p_posY = 0,
                               const std::string& p_extraOptions = EMPTY_STR, u32 p_startIndent = 1,
                               u32 p_indent = 4 ) const;

        void print_vertices( FILE* p_out, const vertex& p_vertex, u32 p_sizeX, u32 p_sizeY,
                             s32 p_startX = 0, s32 p_startY = 0,
                             const std::string& p_extraOptions = EMPTY_STR, u32 p_startIndent = 1,
                             u32 p_indent = 4 ) const;

        void print_diagonal( FILE* p_out, const vertex& p_vertex, u32 p_length, s32 p_startX = 0,
                             s32 p_startY = 0, const std::string& p_extraOptions = EMPTY_STR,
                             u32 p_startIndent = 1, u32 p_indent = 4 ) const;
    };

    void print_vertex( FILE* p_out, const std::string& p_position, const vertex& p_vertex,
                       const std::string& p_name         = EMPTY_STR,
                       const std::string& p_extraOptions = EMPTY_STR, u32 p_startIndent = 1,
                       u32 p_indent = 4 );

    void print_vertex( FILE* p_out, tikz_point p_position, const vertex& p_vertex,
                       const std::string& p_name         = EMPTY_STR,
                       const std::string& p_extraOptions = EMPTY_STR, u32 p_startIndent = 1,
                       u32 p_indent = 4 );

    void print_arrow( FILE* p_out, const std::string& p_startPos, const std::string& p_endPos,
                      const std::string& p_lineWidth = LW_SUPPORT_LINE, color p_color = COLOR_TEXT,
                      u32 p_startIndent = 1, u32 p_indent = 4 );
    void print_arrow( FILE* p_out, tikz_point p_startPos, tikz_point p_endPos,
                      const std::string& p_lineWidth = LW_SUPPORT_LINE, color p_color = COLOR_TEXT,
                      u32 p_startIndent = 1, u32 p_indent = 4 );

    void print_selected_arrow( FILE* p_out, const std::string& p_startPos,
                               const std::string& p_endPos, color p_lineColor, color p_fillColor,
                               double p_angle = 0, u32 p_startIndent = 1, u32 p_indent = 4 );

    void print_selected_arrow( FILE* p_out, tikz_point p_startPos, tikz_point p_endPos,
                               color p_lineColor, color p_fillColor, double p_angle = 0,
                               u32 p_startIndent = 1, u32 p_indent = 4 );

} // namespace TIKZ

#include <cstdio>
#include <string>

#include "tikz.h"
#include "tikz_graph.h"

std::string vertex::to_string( ) const {
    std::string res{ };

    res += vertex::shape_string( m_shape ) + ", ";
    res += "inner sep = " + std::to_string( m_innerSep ) + "pt, ";
    res += "outer sep = " + std::to_string( m_outerSep ) + "pt, ";

    if( m_vertexType & vertex::VT_FILL ) { res += "fill = " + m_fillColor.to_string( ) + ", "; }
    if( m_vertexType & vertex::VT_DRAW ) {
        res += "draw = " + m_drawColor.to_string( ) + ", ";
        res += "line width = " + std::to_string( m_lineWidth ) + "pt, ";
    }
    if( m_inner != EMPTY_STR ) {
        res += "path picture = { \\node[";
        res += m_inner;
        res += "] {}; }, ";
    }

    return res;
}

void vertex_grid::print_coordinates( FILE* p_out, u32 p_sizeX, u32 p_sizeY, s32 p_startX,
                                     s32 p_startY, const std::string& p_extraOptions,
                                     u32 p_startIndent, u32 p_indent ) const {
    for( u32 x = 0; x < p_sizeX; ++x ) {
        for( u32 y = 0; y < p_sizeY; ++y ) {
            print_coordinate( p_out, this->point_for_pos( p_startX + x, p_startY + y ),
                              this->label_for_pos( p_startX + x, p_startY + y ), p_extraOptions,
                              p_startIndent, p_indent );
        }
    }
}

void vertex_grid::print_vertices_on_coordinates( FILE* p_out, const vertex& p_vertex, u32 p_sizeX,
                                                 u32 p_sizeY, s32 p_startX, s32 p_startY,
                                                 const std::string& p_extraOptions,
                                                 u32 p_startIndent, u32 p_indent ) const {
    for( u32 x = 0; x < p_sizeX; ++x ) {
        for( u32 y = 0; y < p_sizeY; ++y ) {
            print_vertex( p_out, this->label_for_pos( p_startX + x, p_startY + y ), p_vertex,
                          EMPTY_STR, p_extraOptions, p_startIndent, p_indent );
        }
    }
}

void vertex_grid::print_vertex_on_coordinates( FILE* p_out, const vertex& p_vertex, s32 p_posX,
                                               s32 p_posY, const std::string& p_extraOptions,
                                               u32 p_startIndent, u32 p_indent ) const {
    print_vertex( p_out, this->label_for_pos( p_posX, p_posY ), p_vertex, EMPTY_STR, p_extraOptions,
                  p_startIndent, p_indent );
}

void vertex_grid::print_diagonal_on_coordinates( FILE* p_out, const vertex& p_vertex, u32 p_length,
                                                 s32 p_startX, s32 p_startY,
                                                 const std::string& p_extraOptions,
                                                 u32 p_startIndent, u32 p_indent ) const {
    for( u32 i = 0; i < p_length; ++i ) {
        print_vertex( p_out, this->label_for_pos( p_startX + i, p_startY + i ), p_vertex, EMPTY_STR,
                      p_extraOptions, p_startIndent, p_indent );
    }
}

void vertex_grid::print_new_vertex( FILE* p_out, const vertex& p_vertex, s32 p_posX, s32 p_posY,
                                    const std::string& p_extraOptions, u32 p_startIndent,
                                    u32 p_indent ) const {
    print_vertex( p_out, this->point_for_pos( p_posX, p_posY ), p_vertex,
                  this->label_for_pos( p_posX, p_posY ), p_extraOptions, p_startIndent, p_indent );
}

void vertex_grid::print_vertices( FILE* p_out, const vertex& p_vertex, u32 p_sizeX, u32 p_sizeY,
                                  s32 p_startX, s32 p_startY, const std::string& p_extraOptions,
                                  u32 p_startIndent, u32 p_indent ) const {
    for( u32 x = 0; x < p_sizeX; ++x ) {
        for( u32 y = 0; y < p_sizeY; ++y ) {
            print_vertex( p_out, this->point_for_pos( p_startX + x, p_startY + y ), p_vertex,
                          this->label_for_pos( p_startX + x, p_startY + y ), p_extraOptions,
                          p_startIndent, p_indent );
        }
    }
}

void vertex_grid::print_diagonal( FILE* p_out, const vertex& p_vertex, u32 p_length, s32 p_startX,
                                  s32 p_startY, const std::string& p_extraOptions,
                                  u32 p_startIndent, u32 p_indent ) const {
    for( u32 i = 0; i < p_length; ++i ) {
        print_vertex( p_out, this->point_for_pos( p_startX + i, p_startY + i ), p_vertex,
                      this->label_for_pos( p_startX + i, p_startY + i ), p_extraOptions,
                      p_startIndent, p_indent );
    }
}

void print_vertex( FILE* p_out, const std::string& p_position, const vertex& p_vertex,
                   const std::string& p_name, const std::string& p_extraOptions, u32 p_startIndent,
                   u32 p_indent ) {
    print_node( p_out, p_position, EMPTY_STR, p_vertex.to_string( ) + p_extraOptions, p_name,
                p_startIndent, p_indent );
}

void print_vertex( FILE* p_out, tikz_point p_position, const vertex& p_vertex,
                   const std::string& p_name, const std::string& p_extraOptions, u32 p_startIndent,
                   u32 p_indent ) {
    print_node( p_out, p_position, EMPTY_STR, p_vertex.to_string( ) + p_extraOptions, p_name,
                p_startIndent, p_indent );
}

void print_selected_arrow( FILE* p_out, const std::string& p_startPos, const std::string& p_endPos,
                           color p_lineColor, color p_fillColor, double p_angle, u32 p_startIndent,
                           u32 p_indent ) {
    INDENT_PRINT( p_startIndent )(
        p_out,
        "\\draw[rotate = %5.3lf, %s, double = %s, double distance=.75pt, fill = %s, rounded corners=0.2pt]\n",
        p_angle, p_fillColor.c_str( ), p_lineColor.c_str( ), p_fillColor.c_str( ) );
    INDENT_PRINT( 1 + p_startIndent )(
        p_out, "($(%s)+(-.5pt,0pt)$) -- ($(%s)+(-6.5pt,3.5pt)$) -- ($(%s)+(-6.5pt,1.25pt)$)\n",
        p_endPos.c_str( ), p_endPos.c_str( ), p_endPos.c_str( ) );
    INDENT_PRINT( 1 + p_startIndent )(
        p_out,
        "-- ($(%s)+(2.75pt,1.25pt)$) arc [start angle=25, end angle=333.5, radius=3pt] -- ($(%s)+(2.75pt,-1.25pt)$)\n",
        p_startPos.c_str( ), p_startPos.c_str( ) );
    INDENT_PRINT( 1 + p_startIndent )(
        p_out, "-- ($(%s)+(-6.5pt,-1.25pt)$) -- ($(%s)+(-6.5pt,-3.5pt)$) -- cycle;\n",
        p_endPos.c_str( ), p_endPos.c_str( ) );
}

void print_selected_arrow( FILE* p_out, tikz_point p_startPos, const tikz_point p_endPos,
                           color p_lineColor, color p_fillColor, double p_angle, u32 p_startIndent,
                           u32 p_indent ) {
    char buffer1[ 30 ] = { 0 };
    char buffer2[ 30 ] = { 0 };
    snprintf( buffer1, 29, "%5.3lf, %5.3lf", p_startPos.m_x, p_startPos.m_y );
    snprintf( buffer2, 29, "%5.3lf, %5.3lf", p_endPos.m_x, p_endPos.m_y );

    print_selected_arrow( p_out, buffer1, buffer2, p_lineColor, p_fillColor, p_angle, p_startIndent,
                          p_indent );
}

void print_arrow( FILE* p_out, const std::string& p_startPos, const std::string& p_endPos,
                  const std::string& p_lineWidth, color p_color, u32 p_startIndent, u32 p_indent ) {
    print_connection( p_out, p_startPos, p_endPos, LINE_CONN,
                      p_lineWidth + ", " + p_color.to_string( ) + ", " + ARR_TIP_LATEX,
                      p_startIndent, p_indent );
}

void print_arrow( FILE* p_out, tikz_point p_startPos, tikz_point p_endPos,
                  const std::string& p_lineWidth, color p_color, u32 p_startIndent, u32 p_indent ) {
    print_connection( p_out, p_startPos, p_endPos, LINE_CONN,
                      p_lineWidth + ", " + p_color.to_string( ) + ", " + ARR_TIP_LATEX,
                      p_startIndent, p_indent );
}

#include "tikz.h"

const std::string CROSS_STYLE
    = "\\tikzset{cross/.style={cross out, draw, minimum size=2*(#1-\\pgflinewidth), inner sep=0pt, outer sep=0pt}}\n";

color color::mix( color p_other, u32 p_amount ) const {
    return color{ this->m_name + color::MIX_SYMBOL + std::to_string( p_amount ) + color::MIX_SYMBOL
                  + p_other.m_name };
}

color color::lighten( u32 p_amount ) const {
    return this->mix( COLOR_WHITE, p_amount );
}

color color::darken( u32 p_amount ) const {
    return this->mix( COLOR_BLACK, p_amount );
}

FILE* open_or_abort( const std::string& p_path ) {
    FILE* out = fopen( p_path.c_str( ), "w" );
    if( !out ) {
        fprintf( stderr, "IO error" );
        return nullptr;
    }
    return out;
}

FILE* new_document( const std::string& p_path, const std::string& p_fontPath,
                    const std::string& p_colorPath, const std::string& p_macroPath,
                    const std::string& p_packages, const std::string& p_tikzLibraries,
                    const std::string& p_extra ) {
    FILE* doc = open_or_abort( p_path );
    fprintf( doc, "\\documentclass[multi=page]{standalone}\n"
                  "\\usepackage{algorithm2e,mathtools,tikz}\n"
                  "\\usetikzlibrary{calc,fit}\n"
                  "\\usetikzlibrary{shapes,shapes.geometric}\n"
                  "\\usetikzlibrary{decorations,decorations.pathmorphing,decorations.markings}\n"
                  "\\usetikzlibrary{decorations.pathreplacing,decorations.shapes}\n" );

    if( p_packages != EMPTY_STR ) { fprintf( doc, "\\usepackage{%s}\n", p_packages.c_str( ) ); }

    if( p_macroPath != EMPTY_STR ) { fprintf( doc, "\\input{%s}\n", p_macroPath.c_str( ) ); }
    if( p_fontPath != EMPTY_STR ) { fprintf( doc, "\\input{%s}\n", p_fontPath.c_str( ) ); }
    if( p_colorPath != EMPTY_STR ) { fprintf( doc, "\\input{%s}\n", p_colorPath.c_str( ) ); }

    if( p_tikzLibraries != EMPTY_STR ) {
        fprintf( doc, "\\usetikzlibrary{%s}\n", p_tikzLibraries.c_str( ) );
    }

    if( p_extra != EMPTY_STR ) { fprintf( doc, "%s\n", p_extra.c_str( ) ); }

    // some default extra styles
    fprintf( doc, "%s\n", CROSS_STYLE.c_str( ) );

    fprintf( doc, "\\begin{document}\n" );
    fprintf( doc, "\\pdfvariable suppressoptionalinfo \\numexpr\n"
                  "0\n"
                  "+   1   %% PTEX.FullBanner\n"
                  "+   2   %% PTEX.FileName\n"
                  "+   4   %% PTEX.PageNumber\n"
                  "+   8   %% PTEX.InfoDict\n"
                  "+  16   %% Creator\n"
                  "+  32   %% CreationDate\n"
                  "+  64   %% ModDate\n"
                  "+ 128   %% Producer\n"
                  "+ 256   %% Trapped\n"
                  "+ 512   %% ID\n"
                  "\\relax\n" );

    return doc;
}

void finish_document( FILE* p_out ) {
    fprintf( p_out, "\\end{document}\n" );
    fclose( p_out );
}

void initialize_tikzpicture( FILE* p_out, const std::string& p_options ) {
    fprintf( p_out,
             "\\begin{page}\n"
             "\\begin{tikzpicture}[%s]\n",
             p_options.c_str( ) );
}
void finish_tikzpicture( FILE* p_out ) {
    fprintf( p_out, "\\end{tikzpicture}\n"
                    "\\end{page}\n" );
}

void indent( FILE* p_out, u32 p_indentLevel, u32 p_indent ) {
    char buffer[ 10 ] = { 0 };
    snprintf( buffer, 9, "%%%lu s", p_indentLevel * p_indent );
    fprintf( p_out, buffer, "" );
}

void print_coordinate( FILE* p_out, const std::string& p_position, const std::string& p_name,
                       const std::string& p_options, u32 p_startIndent, u32 p_indent ) {
    INDENT_PRINT( p_startIndent )( p_out, "\\coordinate[%s] (%s) at (%s);\n", p_options.c_str( ),
                                   p_name.c_str( ), p_position.c_str( ) );
}

void print_coordinate( FILE* p_out, tikz_point p_position, const std::string& p_name,
                       const std::string& p_options, u32 p_startIndent, u32 p_indent ) {
    char buffer[ 30 ] = { 0 };
    snprintf( buffer, 29, "%5.3lf, %5.3lf", p_position.m_x, p_position.m_y );
    print_coordinate( p_out, std::string{ buffer }, p_name, p_options, p_startIndent, p_indent );
}

void print_node( FILE* p_out, const std::string& p_position, const std::string& p_content,
                 const std::string& p_options, const std::string& p_name, u32 p_startIndent,
                 u32 p_indent ) {
    if( p_name == EMPTY_STR ) {
        INDENT_PRINT( p_startIndent )( p_out, "\\node[%s] at (%s) {%s};\n", p_options.c_str( ),
                                       p_position.c_str( ), p_content.c_str( ) );
    } else {
        INDENT_PRINT( p_startIndent )( p_out, "\\node[%s] (%s) at (%s) {%s};\n", p_options.c_str( ),
                                       p_name.c_str( ), p_position.c_str( ), p_content.c_str( ) );
    }
}

void print_node( FILE* p_out, tikz_point p_position, const std::string& p_content,
                 const std::string& p_options, const std::string& p_name, u32 p_startIndent,
                 u32 p_indent ) {
    char buffer[ 30 ] = { 0 };
    snprintf( buffer, 29, "%5.3lf, %5.3lf", p_position.m_x, p_position.m_y );
    print_node( p_out, std::string{ buffer }, p_content, p_options, p_name, p_startIndent,
                p_indent );
}

void print_connection( FILE* p_out, const std::string& p_topLeft, const std::string& p_bottomRight,
                       const std::string& p_connection, const std::string& p_options,
                       u32 p_startIndent, u32 p_indent ) {
    INDENT_PRINT( p_startIndent )( p_out, "\\draw[%s] (%s) %s (%s);\n", p_options.c_str( ),
                                   p_topLeft.c_str( ), p_connection.c_str( ),
                                   p_bottomRight.c_str( ) );
}

void print_connection( FILE* p_out, tikz_point p_topLeft, tikz_point p_bottomRight,
                       const std::string& p_connection, const std::string& p_options,
                       u32 p_startIndent, u32 p_indent ) {
    char buffer1[ 30 ] = { 0 };
    char buffer2[ 30 ] = { 0 };
    snprintf( buffer1, 29, "%5.3lf, %5.3lf", p_topLeft.m_x, p_topLeft.m_y );
    snprintf( buffer2, 29, "%5.3lf, %5.3lf", p_bottomRight.m_x, p_bottomRight.m_y );
    print_connection( p_out, buffer1, buffer2, p_connection, p_options, p_startIndent, p_indent );
}

void print_text( FILE* p_out, const std::string& p_text, tikz_point p_position,
                 const std::string& p_options, color p_textColor, u32 p_startIndent,
                 u32 p_indent ) {
    INDENT_PRINT( p_startIndent )(
        p_out, "\\node[inner sep = 0pt, text = %s, %s ] at (%lf, %lf) { %s };\n",
        p_textColor.c_str( ), p_options.c_str( ), p_position.m_x, p_position.m_y, p_text.c_str( ) );
}

void print_cross( FILE* p_out, tikz_point p_position, color p_color, u32 p_startIndent,
                  u32 p_indent ) {
    INDENT_PRINT( p_startIndent )( p_out,
                                   "\\node[cross=5, line width=2.5pt, %s] at (%lf, %lf) {};\n",
                                   p_color.c_str( ), p_position.m_x, p_position.m_y );
}

void print_debug_point( FILE* p_out, tikz_point p_position, u32 p_startIndent, u32 p_indent ) {
    INDENT_PRINT( p_startIndent )(
        p_out, "\\node[fill, circle, red, inner sep = 1pt] at (%5.3lf, %5.3lf) {};\n",
        p_position.m_x, p_position.m_y );
}

std::pair<tikz_point, tikz_point> translate_right( std::pair<tikz_point, tikz_point> p_pos,
                                                   double                            p_amount ) {
    return { tikz_point{ p_pos.first.m_x + p_amount, p_pos.first.m_y },
             tikz_point{ p_pos.second.m_x + p_amount, p_pos.second.m_y } };
}

std::pair<tikz_point, tikz_point> translate_down( std::pair<tikz_point, tikz_point> p_pos,
                                                  double                            p_amount ) {
    return { tikz_point{ p_pos.first.m_x, p_amount + p_pos.first.m_y },
             tikz_point{ p_pos.second.m_x, p_amount + p_pos.second.m_y } };
}

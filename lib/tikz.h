#pragma once

#include <algorithm>
#include <cstdio>
#include <map>
#include <string>

#include "defines.h"
#include "tikz_color.h"
#include "tikz_point.h"

namespace TIKZ {
    const extern std::string CROSS_STYLE;

    constexpr std::string LINE_CONN     = " -- ";
    constexpr std::string RECT_CONN     = " rectangle ";
    constexpr std::string CIRC_CONN     = " circle ";
    constexpr std::string ARR_TIP_LATEX = "-latex";

    constexpr std::string LW_VERY_THIN_OUTLINE  = "line width = .5pt";
    constexpr std::string LW_THIN_OUTLINE       = "line width = .75pt";
    constexpr std::string LW_OUTLINE            = "line width = 1pt";
    constexpr std::string LW_SUPPORT_LINE       = "line width = .5pt";
    constexpr std::string LW_THICK_SUPPORT_LINE = "line width = .75pt";
    constexpr std::string LW_LINE               = "line width = 1.25pt";

    inline std::string math_mode( const std::string& p_string ) {
        return "\\(" + p_string + "\\)";
    }

    inline std::string text_typewriter( const std::string& p_string ) {
        return "\\texttt{" + p_string + "}";
    }

    constexpr std::string VSIZE_CORRECTION       = "\\vphantom{Ag|}";
    constexpr std::string WILDCARD_SYMBOL_FILLED = "\\wcsymbolf";
    constexpr std::string WILDCARD_SYMBOL        = "\\wcsymbol";

    inline std::string textsize_small( const std::string& p_string ) {
        return "{\\small " + p_string + "}";
    }
    inline std::string textsize_footnotesize( const std::string& p_string ) {
        return "{\\footnotesize " + p_string + "}";
    }
    inline std::string textsize_tiny( const std::string& p_string ) {
        return "{\\tiny " + p_string + "}";
    }
    inline std::string textsize_large( const std::string& p_string ) {
        return "{\\large " + p_string + "}";
    }

    FILE* open_or_abort( const std::string& p_path );

    FILE* new_document( const std::string& p_path, const std::string& p_fontPath = EMPTY_STR,
                        const std::string& p_colorPath     = EMPTY_STR,
                        const std::string& p_macroPath     = EMPTY_STR,
                        const std::string& p_packages      = EMPTY_STR,
                        const std::string& p_tikzLibraries = EMPTY_STR,
                        const std::string& p_extra         = EMPTY_STR );

    void finish_document( FILE* p_out );

    void initialize_tikzpicture( FILE* p_out, const std::string& p_options = EMPTY_STR );
    void finish_tikzpicture( FILE* p_out );
    void indent( FILE* p_out, u32 p_indentLevel = 1, u32 p_indent = 4 );

#define INDENT_PRINT( p_level )         \
    indent( p_out, p_level, p_indent ); \
    fprintf

    void print_coordinate( FILE* p_out, const std::string& p_position, const std::string& p_name,
                           const std::string& p_extraOptions = EMPTY_STR, u32 p_startIndent = 1,
                           u32 p_indent = 4 );

    void print_coordinate( FILE* p_out, tikz_point p_position, const std::string& p_name,
                           const std::string& p_extraOptions = EMPTY_STR, u32 p_startIndent = 1,
                           u32 p_indent = 4 );

    void print_node( FILE* p_out, const std::string& p_position,
                     const std::string& p_content = EMPTY_STR,
                     const std::string& p_options = EMPTY_STR,
                     const std::string& p_name = EMPTY_STR, u32 p_startIndent = 1,
                     u32 p_indent = 4 );

    void print_node( FILE* p_out, tikz_point p_position, const std::string& p_content = EMPTY_STR,
                     const std::string& p_options = EMPTY_STR,
                     const std::string& p_name = EMPTY_STR, u32 p_startIndent = 1,
                     u32 p_indent = 4 );

    void print_connection( FILE* p_out, const std::string& p_topLeft,
                           const std::string& p_bottomRight,
                           const std::string& p_connection = LINE_CONN,
                           const std::string& p_options = EMPTY_STR, u32 p_startIndent = 1,
                           u32 p_indent = 4 );
    void print_connection( FILE* p_out, tikz_point p_topLeft, tikz_point p_bottomRight,
                           const std::string& p_connection = LINE_CONN,
                           const std::string& p_options = EMPTY_STR, u32 p_startIndent = 1,
                           u32 p_indent = 4 );

    void print_text( FILE* p_out, const std::string& p_text, tikz_point p_position,
                     const std::string& p_options = EMPTY_STR, color p_textColor = COLOR_TEXT,
                     u32 p_startIndent = 1, u32 p_indent = 4 );

    void print_cross( FILE* p_out, tikz_point p_position, color p_color = COLOR_TEXT,
                      u32 p_startIndent = 1, u32 p_indent = 4 );

    void print_debug_point( FILE* p_out, tikz_point p_position, u32 p_startIndent = 1,
                            u32 p_indent = 4 );

} // namespace TIKZ

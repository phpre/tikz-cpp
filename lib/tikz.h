#pragma once

#include <algorithm>
#include <cstdio>
#include <deque>
#include <map>
#include <string>

#include "defines.h"
#include "tikz_color.h"
#include "tikz_point.h"

namespace TIKZ {
    typedef std::deque<std::pair<u64, std::string>> render_t;

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

    void print_coordinate( FILE* p_out, const std::string& p_position, const std::string& p_name,
                           const std::string& p_extraOptions = EMPTY_STR, u64 p_startIndent = 1,
                           u64 p_indent = 4 );

    void print_coordinate( FILE* p_out, tikz_point p_position, const std::string& p_name,
                           const std::string& p_extraOptions = EMPTY_STR, u64 p_startIndent = 1,
                           u64 p_indent = 4 );

    void print_node( FILE* p_out, const std::string& p_position,
                     const std::string& p_content = EMPTY_STR,
                     const std::string& p_options = EMPTY_STR,
                     const std::string& p_name = EMPTY_STR, u64 p_startIndent = 1,
                     u64 p_indent = 4 );

    void print_node( FILE* p_out, tikz_point p_position, const std::string& p_content = EMPTY_STR,
                     const std::string& p_options = EMPTY_STR,
                     const std::string& p_name = EMPTY_STR, u64 p_startIndent = 1,
                     u64 p_indent = 4 );

    void print_connection( FILE* p_out, const std::string& p_topLeft,
                           const std::string& p_bottomRight,
                           const std::string& p_connection = LINE_CONN,
                           const std::string& p_options = EMPTY_STR, u64 p_startIndent = 1,
                           u64 p_indent = 4 );
    void print_connection( FILE* p_out, tikz_point p_topLeft, tikz_point p_bottomRight,
                           const std::string& p_connection = LINE_CONN,
                           const std::string& p_options = EMPTY_STR, u64 p_startIndent = 1,
                           u64 p_indent = 4 );

    void print_debug_point( FILE* p_out, tikz_point p_position, u64 p_startIndent = 1,
                            u64 p_indent = 4 );

} // namespace TIKZ

#pragma once
#include <string>
#include "defines.h"
#include "tikz_color.h"
#include "tikz_point.h"

namespace TIKZ {
    const extern std::string CROSS_STYLE;

    void print_width_macro( FILE* p_out, double p_width, const std::string& p_text,
                            u64 p_startIndent, u64 p_indent );

#define INDENT_PRINT( p_level )         \
    indent( p_out, p_level, p_indent ); \
    fprintf

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
    void indent( FILE* p_out, u64 p_indentLevel = 1, u64 p_indent = 4 );

    void print_text( FILE* p_out, const std::string& p_text, tikz_point p_position,
                     const std::string& p_options = EMPTY_STR, color p_textColor = COLOR_TEXT,
                     u64 p_startIndent = 1, u64 p_indent = 4 );

    void print_cross( FILE* p_out, tikz_point p_position, color p_color = COLOR_TEXT,
                      u64 p_startIndent = 1, u64 p_indent = 4 );
} // namespace TIKZ

#pragma once
#include <string>
#include "breakpoint.h"
#include "defines.h"
#include "tikz_color.h"
#include "tikz_point.h"

namespace TIKZ {
    struct stylized_string;

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

    void print_single_character( FILE* p_out, const stylized_string& p_S, u64 p_pos,
                                 std::pair<std::string, std::string> p_render, tikz_point p_center,
                                 std::string p_extraOptions, u64 p_startIndent, u64 p_indent );
    void print_wildcard( FILE* p_out, const stylized_string& p_S, u64 p_pos, tikz_point p_center,
                         u64 p_startIndent, u64 p_indent );
    void print_string_inner( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                             u64 p_startIndent, u64 p_indent );

    void print_separator( FILE* p_out, tikz_point p_PtopLeft, tikz_point p_TtopLeft, color p_color,
                          u64 p_startIndent = 1, u64 p_indent = 4 );

    void print_matched_string_pair( FILE* p_out, const stylized_string& p_P, tikz_point p_PtopLeft,
                                    const stylized_string& p_T, tikz_point p_TtopLeft,
                                    color p_bgColor, bool p_compress = false, u64 p_startIndent = 1,
                                    u64 p_indent = 4 );

    std::pair<tikz_point, tikz_point>
    print_alignment( FILE* p_out, const stylized_string& p_P, tikz_point p_PtopLeft,
                     const stylized_string& p_T, tikz_point p_TtopLeft,
                     const breakpoint_repn& p_brpnt, bool p_printBreakpoints = true,
                     bool p_printExtraStringParts = false, bool p_compress = false,
                     u64 p_startIndent = 1, u64 p_indent = 4 );

    std::string to_stringT( const breakpoint& p_bp, const std::string& p_wildcard );

    std::string to_stringP( const breakpoint& p_bp, const std::string& p_wildcard );

    void print_string( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                       u64 p_startIndent = 1, u64 p_indent = 4 );

    void print_string_vertical( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                                u64 p_startIndent = 1, u64 p_indent = 4 );

} // namespace TIKZ

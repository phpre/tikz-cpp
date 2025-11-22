#include "old.h"
#include "tikz.h"
#include "tikz_document.h"
#include "tikz_string.h"

namespace TIKZ {
    const std::string CROSS_STYLE
        = "\\tikzset{cross/.style={cross out, draw, minimum size=2*(#1-\\pgflinewidth), inner sep=0pt, outer sep=0pt}}\n";

    void print_width_macro( FILE* p_out, double p_width, const std::string& p_text,
                            u64 p_startIndent, u64 p_indent ) {
        if( p_text == EMPTY_STR ) {
            INDENT_PRINT( p_startIndent )(
                p_out, "\\pgfmathsetmacro\\twd{ %5.3lf * 1cm / width(\" \") }\n", p_width );
        } else {
            INDENT_PRINT( p_startIndent )(
                p_out, "\\pgfmathsetmacro\\twd{ %5.3lf * 1cm / width(\"%s\") }\n", p_width,
                p_text.c_str( ) );
        }
    }

    void print_text( FILE* p_out, const std::string& p_text, tikz_point p_position,
                     const std::string& p_options, color p_textColor, u64 p_startIndent,
                     u64 p_indent ) {
        INDENT_PRINT( p_startIndent )(
            p_out, "\\node[inner sep = 0pt, text = %s, %s ] at (%lf, %lf) { %s };\n",
            p_textColor.c_str( ), p_options.c_str( ), p_position.m_x, p_position.m_y,
            p_text.c_str( ) );
    }

    FILE* open_or_abort( const std::string& p_path ) {
        return document::open_or_abort( p_path );
    }

    void indent( FILE* p_out, u64 p_indentLevel, u64 p_indent ) {
        document::indent( p_out, p_indentLevel, p_indent );
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

    void print_cross( FILE* p_out, tikz_point p_position, color p_color, u64 p_startIndent,
                      u64 p_indent ) {
        INDENT_PRINT( p_startIndent )( p_out,
                                       "\\node[cross=5, line width=2.5pt, %s] at (%lf, %lf) {};\n",
                                       p_color.c_str( ), p_position.m_x, p_position.m_y );
    }

    FILE* new_document( const std::string& p_path, const std::string& p_fontPath,
                        const std::string& p_colorPath, const std::string& p_macroPath,
                        const std::string& p_packages, const std::string& p_tikzLibraries,
                        const std::string& p_extra ) {
        FILE* doc = open_or_abort( p_path );
        fprintf( doc,
                 "\\documentclass[multi=page]{standalone}\n"
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

    void print_coordinate( FILE* p_out, const std::string& p_position, const std::string& p_name,
                           const std::string& p_options, u64 p_startIndent, u64 p_indent ) {
        INDENT_PRINT( p_startIndent )( p_out, "\\coordinate[%s] (%s) at (%s);\n",
                                       p_options.c_str( ), p_name.c_str( ), p_position.c_str( ) );
    }

    void print_coordinate( FILE* p_out, tikz_point p_position, const std::string& p_name,
                           const std::string& p_options, u64 p_startIndent, u64 p_indent ) {
        char buffer[ 30 ] = { 0 };
        snprintf( buffer, 29, "%5.3lf, %5.3lf", p_position.m_x, p_position.m_y );
        print_coordinate( p_out, std::string{ buffer }, p_name, p_options, p_startIndent,
                          p_indent );
    }

    void print_node( FILE* p_out, const std::string& p_position, const std::string& p_content,
                     const std::string& p_options, const std::string& p_name, u64 p_startIndent,
                     u64 p_indent ) {
        if( p_name == EMPTY_STR ) {
            INDENT_PRINT( p_startIndent )( p_out, "\\node[%s] at (%s) {%s};\n", p_options.c_str( ),
                                           p_position.c_str( ), p_content.c_str( ) );
        } else {
            INDENT_PRINT( p_startIndent )( p_out, "\\node[%s] (%s) at (%s) {%s};\n",
                                           p_options.c_str( ), p_name.c_str( ), p_position.c_str( ),
                                           p_content.c_str( ) );
        }
    }

    void print_node( FILE* p_out, tikz_point p_position, const std::string& p_content,
                     const std::string& p_options, const std::string& p_name, u64 p_startIndent,
                     u64 p_indent ) {
        char buffer[ 30 ] = { 0 };
        snprintf( buffer, 29, "%5.3lf, %5.3lf", p_position.m_x, p_position.m_y );
        print_node( p_out, std::string{ buffer }, p_content, p_options, p_name, p_startIndent,
                    p_indent );
    }

    void print_debug_point( FILE* p_out, tikz_point p_position, u64 p_startIndent, u64 p_indent ) {
        INDENT_PRINT( p_startIndent )(
            p_out, "\\node[fill, circle, red, inner sep = 1pt] at (%5.3lf, %5.3lf) {};\n",
            p_position.m_x, p_position.m_y );
    }

    void print_single_character( FILE* p_out, const stylized_string& p_S, u64 p_pos,
                                 std::pair<std::string, std::string> p_render, tikz_point p_center,
                                 std::string p_extraOptions, u64 p_startIndent, u64 p_indent ) {
        print_width_macro( p_out, CHAR_WIDTH, p_render.first, p_startIndent, p_indent );

        if( p_S.m_annotation.count( p_pos )
            && p_S.m_annotation.at( p_pos ).m_textColor.is_non_empty( ) ) {
            p_extraOptions += ", color = " + p_S.m_annotation.at( p_pos ).m_textColor.to_string( );
        }
        print_node( p_out, p_center, p_render.second, p_extraOptions, EMPTY_STR, p_startIndent,
                    p_indent );
    }

    void print_wildcard( FILE* p_out, const stylized_string& p_S, u64 p_pos, tikz_point p_center,
                         u64 p_startIndent, u64 p_indent ) {
        double cor1 = p_pos ? .005 : 0, cor2 = ( p_pos + 1 < p_S.length( ) ) ? .005 : 0;
        auto   col = p_S.m_color;
        col.replace_if_non_empty( p_S.annotation_at_pos( p_pos ).m_textColor );

        auto bgcol = p_S.m_fillColor;
        bgcol.replace_if_non_empty( p_S.annotation_at_pos( p_pos ).m_bgColor );

        INDENT_PRINT( p_startIndent )(
            p_out,
            "\\fill[%s, rounded corners = 1pt, %s] "
            "(%5.3lf + %5.3lf, %5.3lf - %5.3lf) rectangle (%5.3lf - %5.3lf, %5.3lf + %5.3lf);\n",
            LW_OUTLINE.c_str( ), col.c_str( ), p_center.m_x - CHAR_WIDTH / 2, cor1,
            p_center.m_y + CHAR_HEIGHT / 2, cor1, p_center.m_x + CHAR_WIDTH / 2, cor2,
            p_center.m_y - CHAR_HEIGHT / 2, cor2 );

        const auto& wc = p_S.m_annotation.at( p_pos );
        // print wildcard symbol
        if( wc.m_showSymbol ) {
            print_node( p_out, p_center, text_typewriter( wc.m_wildcardSymbol ), bgcol.c_str( ),
                        EMPTY_STR, p_startIndent, p_indent );
        } else {
            // wildcard lozenge
            print_node( p_out, p_center, math_mode( WILDCARD_SYMBOL_FILLED ), bgcol.c_str( ),
                        EMPTY_STR, p_startIndent, p_indent );
        }
        // print wildcard index
        if( wc.m_showId ) {
            auto text  = std::string{ "$" } + std::to_string( wc.m_wildcardId ) + "$";
            auto ftext = textsize_tiny( text );

            print_width_macro( p_out, CHAR_WIDTH / 2.0, text, p_startIndent, p_indent );
            print_node(
                p_out,
                tikz_point{ p_center.m_x + CHAR_WIDTH / 3.5, p_center.m_y - CHAR_HEIGHT / 3.5 },
                ftext, bgcol.c_str( ), EMPTY_STR, p_startIndent, p_indent );
        }
    }

    void print_string_inner( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                             u64 p_startIndent, u64 p_indent ) {
        auto        posy = p_StopLeft.m_y - CHAR_HEIGHT / 2.0;
        std::string extraopt
            = p_S.m_color.to_string( ) + ", inner sep = 2pt, xscale = {min(1, \\twd)}";

        if( p_S.m_labelAlign == AN_BEGIN ) {
            extraopt += ", anchor = west";
        } else if( p_S.m_labelAlign == AN_END ) {
            extraopt += ", anchor = east";
        }

        u64 pos = p_S.m_fragment.closed_begin( );
        while( pos < p_S.m_fragment.open_end( ) ) {
            // get next fragment to render
            auto next   = p_S.next( pos );
            auto render = p_S.render_fragment( next );

            auto posx = p_StopLeft.m_x + ( pos - p_S.m_fragment.closed_begin( ) ) * CHAR_WIDTH
                        + ( next.second.length( ) * CHAR_WIDTH ) / 2.0;

            if( p_S.m_labelAlign == AN_BEGIN ) {
                posx = p_StopLeft.m_x + ( pos - p_S.m_fragment.closed_begin( ) ) * CHAR_WIDTH;
            } else if( p_S.m_labelAlign == AN_END ) {
                posx = p_StopLeft.m_x + ( pos - p_S.m_fragment.closed_begin( ) ) * CHAR_WIDTH
                       + ( next.second.length( ) * CHAR_WIDTH );
            }

            switch( next.first ) {
            case stylized_string::fragment_t::FT_CHARACTER: {
                print_single_character( p_out, p_S, pos, render, tikz_point{ posx, posy }, extraopt,
                                        p_startIndent, p_indent );
                break;
            }
            case stylized_string::fragment_t::FT_WILDCARD: {
                print_wildcard( p_out, p_S, pos, tikz_point{ posx, posy }, p_startIndent,
                                p_indent );
            }
            default: {
                print_width_macro( p_out, next.second.length( ) * CHAR_WIDTH, render.first,
                                   p_startIndent, p_indent );
                print_node( p_out, tikz_point{ posx, posy }, render.second, extraopt, EMPTY_STR,
                            p_startIndent, p_indent );
                break;
            }
            }
            pos = next.second.open_end( );
        }
    }

    void print_string( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                       u64 p_startIndent, u64 p_indent ) {
        if( !p_S.length( ) ) { return; }

        double cor3 = -0.035;
        INDENT_PRINT( p_startIndent )(
            p_out,
            "\\fill[rounded corners = 2pt, %s] "
            "(%5.3lf + %5.3lf, %5.3lf - %5.3lf) rectangle"
            "(%5.3lf - %5.3lf, %5.3lf + %5.3lf);\n",
            p_S.m_fillColor.c_str( ), p_StopLeft.m_x, cor3, p_StopLeft.m_y, cor3,
            p_StopLeft.m_x + p_S.length( ) * CHAR_WIDTH, cor3, p_StopLeft.m_y - CHAR_HEIGHT, cor3 );

        // for each character, print a small rectangle
        for( u64 i = 0; i < p_S.length( ); ++i ) {
            double cor1 = i ? .01 : 0, cor2 = ( i + 1 < p_S.length( ) ) ? .01 : 0;
            auto   col = p_S.m_color;
            col.replace_if_non_empty( p_S.annotation_at_pos( i ).m_textColor );

            INDENT_PRINT( p_startIndent )(
                p_out,
                "\\draw[%s, rounded corners = 1pt, %s] "
                "(%5.3lf + %5.3lf, %5.3lf) rectangle (%5.3lf - %5.3lf, %5.3lf);\n",
                LW_OUTLINE.c_str( ), col.c_str( ), p_StopLeft.m_x + i * CHAR_WIDTH, cor1,
                p_StopLeft.m_y, p_StopLeft.m_x + ( i + 1 ) * CHAR_WIDTH, cor2,
                p_StopLeft.m_y - CHAR_HEIGHT );
        }
        // fill draw white box with label
        cor3 = 0.015;
        INDENT_PRINT( p_startIndent )(
            p_out,
            "\\fill[rounded corners = 1pt, %s] "
            "(%5.3lf + %5.3lf, %5.3lf - %5.3lf) rectangle"
            "(%5.3lf - %5.3lf, %5.3lf + %5.3lf);\n",
            p_S.m_fillColor.c_str( ), p_StopLeft.m_x, cor3, p_StopLeft.m_y, cor3,
            p_StopLeft.m_x + p_S.length( ) * CHAR_WIDTH, cor3, p_StopLeft.m_y - CHAR_HEIGHT, cor3 );

        for( u64 i = 0; i < p_S.length( ); ++i ) {
            double cor1 = i ? .04 : .05, cor2 = ( i + 1 < p_S.length( ) ) ? .04 : .05;
            auto   ann = p_S.annotation_at_pos( i );
            if( ann.m_bgColor.is_non_empty( ) ) {
                auto bgcol = ann.m_bgColor;
                INDENT_PRINT( p_startIndent )(
                    p_out,
                    "\\fill[rounded corners = 2pt,  %s] "
                    "(%5.3lf + %5.3lf, %5.3lf) rectangle (%5.3lf - %5.3lf, %5.3lf);\n",
                    bgcol.c_str( ), p_StopLeft.m_x + i * CHAR_WIDTH, cor1, p_StopLeft.m_y - .05,
                    p_StopLeft.m_x + ( i + 1 ) * CHAR_WIDTH, cor2,
                    p_StopLeft.m_y - CHAR_HEIGHT + .05 );
            }
        }

        print_string_inner( p_out, p_S, p_StopLeft, p_startIndent, p_indent );

        double cor4 = .02;
        // add little dots to highlight parts between characters
        for( u64 i = 1; i < p_S.length( ); ++i ) {
            INDENT_PRINT( p_startIndent )(
                p_out,
                "\\node[fill, circle, inner sep =0.1pt, %s] at (%5.3lf, %5.3lf - %5.3lf) {};\n",
                p_S.m_color.c_str( ), p_StopLeft.m_x + i * CHAR_WIDTH, p_StopLeft.m_y, cor4 );
            INDENT_PRINT( p_startIndent )(
                p_out,
                "\\node[fill, circle, inner sep =0.1pt, %s] at (%5.3lf, %5.3lf + %5.3lf) {};\n",
                p_S.m_color.c_str( ), p_StopLeft.m_x + i * CHAR_WIDTH, p_StopLeft.m_y - CHAR_HEIGHT,
                cor4 );
        }
    }

    void print_string_inner_vertical( FILE* p_out, const stylized_string& p_S,
                                      tikz_point p_StopLeft, u64 p_startIndent, u64 p_indent ) {
        auto        posx = p_StopLeft.m_x + ( CHAR_WIDTH ) / 2.0;
        std::string extraopt
            = p_S.m_color.to_string( ) + ", inner sep = 2pt, xscale = {min(1, \\twd)}";
        std::string extraopt_rotate
            = p_S.m_color.to_string( ) + ", inner sep = 2pt, yscale = {min(1, \\twd)}, rotate = 90";

        if( p_S.m_labelAlign == AN_BEGIN ) {
            extraopt += ", anchor = north";
            extraopt_rotate += ", anchor = north";
        } else if( p_S.m_labelAlign == AN_END ) {
            extraopt += ", anchor = south";
            extraopt_rotate += ", anchor = south";
        }

        u64 pos = p_S.m_fragment.closed_begin( );
        while( pos < p_S.m_fragment.open_end( ) ) {
            // get next fragment to render
            auto next   = p_S.next( pos );
            auto render = p_S.render_fragment( next );

            auto posy = p_StopLeft.m_y - ( pos - p_S.m_fragment.closed_begin( ) ) * CHAR_HEIGHT
                        - ( next.second.length( ) * CHAR_HEIGHT ) / 2.0;

            if( p_S.m_labelAlign == AN_BEGIN ) {
                posy = p_StopLeft.m_y - ( pos - p_S.m_fragment.closed_begin( ) ) * CHAR_HEIGHT;
            } else if( p_S.m_labelAlign == AN_END ) {
                posy = p_StopLeft.m_y - ( pos - p_S.m_fragment.closed_begin( ) ) * CHAR_HEIGHT
                       - ( next.second.length( ) * CHAR_HEIGHT );
            }

            switch( next.first ) {
            case stylized_string::fragment_t::FT_CHARACTER: {
                print_single_character( p_out, p_S, pos, render, tikz_point{ posx, posy }, extraopt,
                                        p_startIndent, p_indent );
                break;
            }
            case stylized_string::fragment_t::FT_WILDCARD: {
                print_wildcard( p_out, p_S, pos, tikz_point{ posx, posy }, p_startIndent,
                                p_indent );
            }
            default: {
                print_width_macro(
                    p_out, p_S.rotatable( ) ? next.second.length( ) * CHAR_WIDTH : CHAR_WIDTH,
                    render.first, p_startIndent, p_indent );
                print_node( p_out, tikz_point{ posx, posy }, render.second,
                            p_S.rotatable( ) ? extraopt_rotate : extraopt, EMPTY_STR, p_startIndent,
                            p_indent );
                break;
            }
            }
            pos = next.second.open_end( );
        }
    }
    void print_string_vertical( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                                u64 p_startIndent, u64 p_indent ) {
        if( !p_S.length( ) ) { return; }
        double cor3 = -0.035;
        INDENT_PRINT( p_startIndent )( p_out,
                                       "\\fill[rounded corners=2pt, %s] "
                                       "(%5.3lf + %5.3lf, %5.3lf - %5.3lf) rectangle"
                                       "(%5.3lf - %5.3lf, %5.3lf + %5.3lf);\n",
                                       p_S.m_fillColor.c_str( ), p_StopLeft.m_x, cor3,
                                       p_StopLeft.m_y, cor3, p_StopLeft.m_x + CHAR_WIDTH, cor3,
                                       p_StopLeft.m_y - p_S.length( ) * CHAR_HEIGHT, cor3 );

        // for each character, print a small rectangle
        for( u64 i = 0; i < p_S.length( ); ++i ) {
            double cor1 = i ? .01 : 0, cor2 = ( i + 1 < p_S.length( ) ) ? .01 : 0;
            auto   col = p_S.m_color;
            col.replace_if_non_empty( p_S.annotation_at_pos( i ).m_textColor );

            INDENT_PRINT( p_startIndent )(
                p_out,
                "\\draw[%s, rounded corners = 1pt, %s] "
                "(%5.3lf, %5.3lf - %5.3lf) rectangle (%5.3lf, %5.3lf + %5.3lf);\n",
                LW_OUTLINE.c_str( ), col.c_str( ), p_StopLeft.m_x, p_StopLeft.m_y - i * CHAR_HEIGHT,
                cor1, p_StopLeft.m_x + CHAR_WIDTH, p_StopLeft.m_y - ( i + 1 ) * CHAR_HEIGHT, cor2 );
        }
        // fill draw white box with label
        cor3 = 0.015;
        INDENT_PRINT( p_startIndent )( p_out,
                                       "\\fill[rounded corners=1pt, %s] "
                                       "(%5.3lf + %5.3lf, %5.3lf - %5.3lf) rectangle"
                                       "(%5.3lf - %5.3lf, %5.3lf + %5.3lf);\n",
                                       p_S.m_fillColor.c_str( ), p_StopLeft.m_x, cor3,
                                       p_StopLeft.m_y, cor3, p_StopLeft.m_x + CHAR_WIDTH, cor3,
                                       p_StopLeft.m_y - p_S.length( ) * CHAR_HEIGHT, cor3 );

        for( u64 i = 0; i < p_S.length( ); ++i ) {
            double cor1 = i ? .04 : 0.05, cor2 = ( i + 1 < p_S.length( ) ) ? .04 : .05;
            auto   ann = p_S.annotation_at_pos( i );
            if( ann.m_bgColor.is_non_empty( ) ) {
                auto bgcol = ann.m_bgColor;
                INDENT_PRINT( p_startIndent )(
                    p_out,
                    "\\draw[rounded corners = 2pt, %s] "
                    "(%5.3lf, %5.3lf - %5.3lf) rectangle (%5.3lf, %5.3lf + %5.3lf);\n",
                    bgcol.c_str( ), p_StopLeft.m_x + .05, p_StopLeft.m_y - i * CHAR_HEIGHT, cor1,
                    p_StopLeft.m_x + CHAR_WIDTH - .05, p_StopLeft.m_y - ( i + 1 ) * CHAR_HEIGHT,
                    cor2 );
            }
        }

        print_string_inner_vertical( p_out, p_S, p_StopLeft, p_startIndent, p_indent );

        double cor4 = .02;
        // add little dots to highlight parts between characters
        for( u64 i = 1; i < p_S.length( ); ++i ) {
            INDENT_PRINT( p_startIndent )(
                p_out,
                "\\node[fill, circle, inner sep = 0.1pt, %s] at (%5.3lf + %5.3lf, %5.3lf) {};\n",
                p_S.m_color.c_str( ), p_StopLeft.m_x, cor4, p_StopLeft.m_y - i * CHAR_HEIGHT );
            INDENT_PRINT( p_startIndent )(
                p_out,
                "\\node[fill, circle, inner sep = 0.1pt, %s] at (%5.3lf - %5.3lf, %5.3lf) {};\n",
                p_S.m_color.c_str( ), p_StopLeft.m_x + CHAR_WIDTH, cor4,
                p_StopLeft.m_y - i * CHAR_HEIGHT );
        }
    }

    void print_separator( FILE* p_out, tikz_point p_PtopLeft, tikz_point p_TtopLeft, color p_color,
                          u64 p_startIndent, u64 p_indent ) {
        // make sure T is top, P is bottom
        if( p_PtopLeft.m_y > p_TtopLeft.m_y ) { std::swap( p_PtopLeft, p_TtopLeft ); }

        INDENT_PRINT( p_startIndent )(
            p_out,
            "\\node[rotate = 45, outer sep = 0pt, inner sep = 1pt, fill = %s] "
            "at (%5.3lf, %5.3lf + %5.3lf) {};\n",
            p_color.c_str( ), p_TtopLeft.m_x, p_TtopLeft.m_y, 1.5 * CHAR_GLOW );
        INDENT_PRINT( p_startIndent )(
            p_out,
            "\\node[rotate = 45, outer sep = 0pt, inner sep = 1pt, fill = %s] "
            "at (%5.3lf, %5.3lf - %5.3lf) {};\n",
            p_color.c_str( ), p_PtopLeft.m_x, p_PtopLeft.m_y - CHAR_HEIGHT, 1.5 * CHAR_GLOW );

        INDENT_PRINT( p_startIndent )( p_out, "\\draw[%s, rounded corners=2pt, %s]\n",
                                       LW_LINE.c_str( ), p_color.c_str( ) );
        INDENT_PRINT( 1 + p_startIndent )( p_out,
                                           "(%5.3lf, %5.3lf + %5.3lf) -- (%5.3lf, %5.3lf) "
                                           "-- (%5.3lf, %5.3lf) -- (%5.3lf, %5.3lf - %5.3lf);\n",
                                           p_TtopLeft.m_x, p_TtopLeft.m_y, 1.5 * CHAR_GLOW,
                                           p_TtopLeft.m_x, p_TtopLeft.m_y - CHAR_HEIGHT,
                                           p_PtopLeft.m_x, p_PtopLeft.m_y, p_PtopLeft.m_x,
                                           p_PtopLeft.m_y - CHAR_HEIGHT, 1.5 * CHAR_GLOW );
    }

   void print_matched_string_pair( FILE* p_out, const stylized_string& p_P, tikz_point p_PtopLeft,
                                    const stylized_string& p_T, tikz_point p_TtopLeft,
                                    color p_bgColor, bool p_compress, u64 p_startIndent,
                                    u64 p_indent ) {
        // print glow bubble
        if( !p_compress ) {
            INDENT_PRINT( p_startIndent )(
                p_out, "\\draw[%s, rounded corners = 2pt, %s, fill = %s]\n",
                LW_THIN_OUTLINE.c_str( ), p_bgColor.c_str( ), p_bgColor.to_bg( ).c_str( ) );
        } else {
            INDENT_PRINT( p_startIndent )(
                p_out, "\\draw[%s, rounded corners = 2pt, %s, fill = %s]\n",
                LW_VERY_THIN_OUTLINE.c_str( ), p_bgColor.c_str( ), p_bgColor.to_bg( ).c_str( ) );
        }

        auto glowx = p_compress ? 0 : CHAR_GLOW;
        auto glowy = p_compress ? -CHAR_GLOW : CHAR_GLOW;

        INDENT_PRINT( 1 + p_startIndent )( p_out,
                                           "(%5.3lf - %5.3lf, %5.3lf + %5.3lf) "
                                           "-- (%5.3lf - %5.3lf, %5.3lf - %5.3lf)\n",
                                           p_PtopLeft.m_x, glowx, p_PtopLeft.m_y,
                                           0 * glowy, // bot-mid left
                                           p_TtopLeft.m_x, glowx, p_TtopLeft.m_y - CHAR_HEIGHT,
                                           0 * glowy // top-mid left
        );

        // top line
        if( !p_compress || p_T.length( ) ) {
            INDENT_PRINT( 1 + p_startIndent )(
                p_out,
                "-- (%5.3lf - %5.3lf, %5.3lf + %5.3lf) "
                "-- (%5.3lf + %5.3lf, %5.3lf + %5.3lf) "
                "-- (%5.3lf + %5.3lf, %5.3lf - %5.3lf)\n",
                p_TtopLeft.m_x, glowx, p_TtopLeft.m_y, glowy, // top left
                p_TtopLeft.m_x + p_T.length( ) * CHAR_WIDTH, glowx, p_TtopLeft.m_y,
                glowy, // top
                       // right
                p_TtopLeft.m_x + p_T.length( ) * CHAR_WIDTH, glowx, p_TtopLeft.m_y - CHAR_HEIGHT,
                0 * glowy // top-mid right
            );
        }

        if( !p_compress || p_P.length( ) ) {
            INDENT_PRINT( 1 + p_startIndent )(
                p_out,
                "-- (%5.3lf + %5.3lf, %5.3lf + %5.3lf) "
                "-- (%5.3lf + %5.3lf, %5.3lf - %5.3lf) "
                "-- (%5.3lf - %5.3lf, %5.3lf - %5.3lf)\n",
                p_PtopLeft.m_x + p_P.length( ) * CHAR_WIDTH, glowx, p_PtopLeft.m_y,
                0 * glowy, // bot-mid right
                p_PtopLeft.m_x + p_P.length( ) * CHAR_WIDTH, glowx, p_PtopLeft.m_y - CHAR_HEIGHT,
                glowy,                                                     // bot right
                p_PtopLeft.m_x, glowx, p_PtopLeft.m_y - CHAR_HEIGHT, glowy // bot left
            );
        }

        INDENT_PRINT( 1 + p_startIndent )( p_out, "-- cycle;\n" );

        if( p_P.length( ) == p_T.length( ) ) {
            // print "matching edges"
            INDENT_PRINT( p_startIndent )( p_out, "\\draw[%s, %s]\n", LW_SUPPORT_LINE.c_str( ),
                                           p_bgColor.deemphasize( ).c_str( ) );
            for( u64 i = 0; i < p_P.length( ); ++i ) {
                INDENT_PRINT( 1 + p_startIndent )(
                    p_out, "(%5.3lf, %5.3lf) -- (%5.3lf, %5.3lf)\n",
                    p_TtopLeft.m_x + ( i + .5 ) * CHAR_WIDTH, p_TtopLeft.m_y - CHAR_HEIGHT,
                    p_PtopLeft.m_x + ( i + .5 ) * CHAR_WIDTH, p_PtopLeft.m_y );
            }
            INDENT_PRINT( p_startIndent )( p_out, ";\n" );
            for( u64 i = 0; i < p_P.length( ); ++i ) {
                // if characters are printed, check that they actually match
                bool match = p_P.has_wildcard( i + p_P.m_fragment.closed_begin( ) )
                             || p_T.has_wildcard( i + p_T.m_fragment.closed_begin( ) );
                match = match
                        || ( p_P[ i + p_P.m_fragment.closed_begin( ) ]
                             == p_T[ i + p_T.m_fragment.closed_begin( ) ] );

                if( !match ) {
                    // if they don't, draw a big cross symbolizing the replacement
                    print_cross(
                        p_out,
                        tikz_point{ ( p_TtopLeft.m_x + ( i + .5 ) * CHAR_WIDTH + p_PtopLeft.m_x
                                      + ( i + .5 ) * CHAR_WIDTH )
                                        / 2.0,
                                    p_PtopLeft.m_y
                                        + ( p_TtopLeft.m_y - CHAR_HEIGHT - p_PtopLeft.m_y ) / 2.0 },
                        p_bgColor.deemphasize_weak( ), p_startIndent, p_indent );
                }
            }
        }

        if( !p_compress && p_P.length( ) ) {
            print_string( p_out, p_P, p_PtopLeft, p_startIndent, p_indent );
        }
        if( !p_compress && p_T.length( ) ) {
            print_string( p_out, p_T, p_TtopLeft, p_startIndent, p_indent );
        }
    }

    std::pair<tikz_point, tikz_point> print_alignment(
        FILE* p_out, const stylized_string& p_P, tikz_point p_PtopLeft, const stylized_string& p_T,
        tikz_point p_TtopLeft, const breakpoint_repn& p_brpnt, bool p_printBreakpoints,
        bool p_printExtraStringParts, bool p_compress, u64 p_startIndent, u64 p_indent ) {
        double pxpos = p_PtopLeft.m_x, txpos = p_TtopLeft.m_x;
        double sepWidth = 3 * CHAR_GLOW, extraGlow = 3.5 * CHAR_GLOW, smallGlow = 2 * CHAR_GLOW;

        stylized_string pnew{ p_P.m_name, p_P.m_fragment,
                              p_P.m_displayStyle | str_displ_t::SHOW_CHARACTERS };
        stylized_string tnew{ p_T.m_name, p_T.m_fragment,
                              p_T.m_displayStyle | str_displ_t::SHOW_CHARACTERS };

        for( u64 i = p_P.m_fragment.closed_begin( ); i < p_P.m_fragment.open_end( ); ++i ) {
            if( p_P.has_wildcard( i ) ) {
                pnew.annotation_at_pos( i ) = p_P.annotation_at_pos( i );
            }
        }
        for( u64 i = p_T.m_fragment.closed_begin( ); i < p_T.m_fragment.open_end( ); ++i ) {
            if( p_T.has_wildcard( i ) ) {
                tnew.annotation_at_pos( i ) = p_T.annotation_at_pos( i );
            }
        }

        if( p_compress ) {
            sepWidth = extraGlow = smallGlow = 0;
            p_printBreakpoints               = false;
        }
        double pxinit = pxpos, txinit = txpos;

        if( p_printExtraStringParts ) {
            auto fragP = fragmentco{ p_P.m_fragment.closed_begin( ), p_brpnt.front( ).m_posP };
            auto fragT = fragmentco{ p_T.m_fragment.closed_begin( ), p_brpnt.front( ).m_posT };
            if( fragP.length( ) ) {
                if( !p_compress ) {
                    print_string( p_out, p_P.slice( fragP ), tikz_point{ pxpos, p_PtopLeft.m_y },
                                  p_startIndent, p_indent );
                }
                pxpos += CHAR_WIDTH * fragP.length( ) + smallGlow;
            } else {
                pxpos += CHAR_WIDTH * fragT.length( ) + smallGlow;
                pxinit = pxpos;
            }
            if( fragT.length( ) ) {
                if( !p_compress ) {
                    print_string( p_out, p_T.slice( fragT ), tikz_point{ txpos, p_TtopLeft.m_y },
                                  p_startIndent, p_indent );
                }
                txpos += CHAR_WIDTH * fragT.length( ) + smallGlow;
            } else {
                txpos += CHAR_WIDTH * fragP.length( ) + smallGlow;
                txinit = txpos;
            }
        }

        double last = pxpos, lastt = txpos;
        u64    shiftP = 0, shiftT = 0;

        if( p_printBreakpoints ) {
            // initial dummy breakpoint
            print_separator( p_out, tikz_point{ pxpos, p_PtopLeft.m_y },
                             tikz_point{ txpos, p_TtopLeft.m_y }, SEP_COL.deemphasize( ),
                             p_startIndent, p_indent );
            print_text(
                p_out, to_stringP( p_brpnt.front( ), WILDCARD ),
                tikz_point{ pxpos - 3 * CHAR_GLOW, p_PtopLeft.m_y - CHAR_HEIGHT - 3 * CHAR_GLOW },
                "anchor = north west", SEP_COL, p_startIndent, p_indent );
            print_text( p_out, to_stringT( p_brpnt.front( ), WILDCARD ),
                        tikz_point{ txpos - 3 * CHAR_GLOW, p_TtopLeft.m_y + 3 * CHAR_GLOW },
                        "anchor = south west", SEP_COL, p_startIndent, p_indent );
            pxpos += sepWidth;
            txpos += sepWidth;
        }

        for( u64 i = 1; i < p_brpnt.size( ); ++i ) {
            auto bp = p_brpnt[ i ], prevbp = p_brpnt[ i - 1 ];
            auto fragP = fragmentco{ prevbp.m_posP + shiftP, bp.m_posP };
            auto fragT = fragmentco{ prevbp.m_posT + shiftT, bp.m_posT };

            if( fragT.length( ) > 0 && fragP.length( ) > 0 ) {
                // matched fragment
                print_matched_string_pair( p_out, p_P.slice( fragP ),
                                           tikz_point{ pxpos, p_PtopLeft.m_y }, p_T.slice( fragT ),
                                           tikz_point{ txpos, p_TtopLeft.m_y }, MAT_COL, p_compress,
                                           p_startIndent, p_indent );
                pxpos += CHAR_WIDTH * fragP.length( ) + extraGlow;
                txpos += CHAR_WIDTH * fragT.length( ) + extraGlow;
            }

            // print next breakpoint
            if( p_printBreakpoints ) {
                auto posx = pxpos, posxt = txpos;
                auto extray = 0.0, extrayt = 0.0;

                if( pxpos < last + .5 ) {
                    extray = .35;
                    last += .1;
                } else if( pxpos < last + .9 ) {
                    posx = last + .75;
                    last = posx;
                } else {
                    last = posx;
                }
                if( txpos < lastt + .5 ) {
                    extrayt = .35;
                    lastt += .1;
                } else if( txpos < lastt + .9 ) {
                    posxt = lastt + .75;
                    lastt = posxt;
                } else {
                    lastt = posxt;
                }

                std::string anchort = "anchor = south", anchorp = "anchor = north";
                if( bp.m_posT == p_brpnt.back( ).m_posT ) {
                    anchort = "anchor = south east";
                    posxt += sepWidth;
                } else if( bp.m_posT == p_brpnt.front( ).m_posT ) {
                    anchort = "anchor = south west";
                    posxt -= sepWidth;
                } else {
                    posxt -= CHAR_GLOW;
                }
                if( bp.m_posP == p_brpnt.back( ).m_posP ) {
                    anchorp = "anchor = north east";
                    posx += sepWidth;
                } else if( bp.m_posP == p_brpnt.front( ).m_posP ) {
                    anchorp = "anchor = north west";
                    posx -= sepWidth;
                } else {
                    posx -= CHAR_GLOW;
                }

                auto col = color_for_bp( bp );
                pxpos    = pxpos - extraGlow + sepWidth;
                txpos    = txpos - extraGlow + sepWidth;
                print_separator( p_out, { pxpos, p_PtopLeft.m_y }, { txpos, p_TtopLeft.m_y },
                                 col.deemphasize( ), p_startIndent, p_indent );
                pxpos += sepWidth;
                txpos += sepWidth;
                print_text( p_out, to_stringP( p_brpnt[ i ], WILDCARD ),
                            tikz_point{ posx, p_PtopLeft.m_y - CHAR_HEIGHT - sepWidth - extray },
                            anchorp, col, p_startIndent, p_indent );
                print_text( p_out, to_stringT( p_brpnt[ i ], WILDCARD ),
                            tikz_point{ posxt, p_TtopLeft.m_y + sepWidth + extrayt }, anchort, col,
                            p_startIndent, p_indent );
            }

            if( bp.is_dummy( ) ) {
                shiftP = 0;
                shiftT = 0;
            } else {
                if( bp.m_charT && bp.m_charP ) { // substitution
                    shiftP = 1;
                    shiftT = 1;

                    pnew[ bp.m_posP ] = std::string{ bp.m_charP };
                    tnew[ bp.m_posT ] = std::string{ bp.m_charT };
                    if( !pnew.has_wildcard( bp.m_posP ) ) {
                        pnew.highlight_position( bp.m_posP, color_for_bp( bp ) );
                    }
                    if( !tnew.has_wildcard( bp.m_posT ) ) {
                        tnew.highlight_position( bp.m_posT, color_for_bp( bp ) );
                    }
                } else if( bp.m_charT ) { // insertion
                    shiftT = 1;
                    shiftP = 0;

                    tnew[ bp.m_posT ] = std::string{ bp.m_charT };
                    tnew.highlight_position( bp.m_posT, color_for_bp( bp ) );
                } else if( bp.m_charP ) { // deletion
                    shiftP = 1;
                    shiftT = 0;

                    pnew[ bp.m_posP ] = std::string{ bp.m_charP };
                    pnew.highlight_position( bp.m_posP, color_for_bp( bp ) );
                }
                print_matched_string_pair(
                    p_out,
                    p_P.slice( { bp.m_posP, bp.m_posP + shiftP } )
                        .replace_data( str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS,
                                       std::string{ bp.m_charP }, bp.m_posP ),
                    tikz_point{ pxpos, p_PtopLeft.m_y },
                    p_T.slice( { bp.m_posT, bp.m_posT + shiftT } )
                        .replace_data( str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS,
                                       std::string{ bp.m_charT }, bp.m_posT ),
                    tikz_point{ txpos, p_TtopLeft.m_y }, color_for_bp( bp ), p_compress,
                    p_startIndent, p_indent );
                pxpos += shiftP * CHAR_WIDTH + extraGlow;
                txpos += shiftT * CHAR_WIDTH + extraGlow;
            }
        }

        if( !p_compress && p_printExtraStringParts ) {
            auto fragP = fragmentco{ p_brpnt.back( ).m_posP, p_P.m_fragment.open_end( ) };
            auto fragT = fragmentco{ p_brpnt.back( ).m_posT, p_T.m_fragment.open_end( ) };
            auto corr  = smallGlow - extraGlow;
            if( p_printBreakpoints ) { corr = smallGlow - sepWidth; }
            if( fragP.length( ) ) {
                pxpos += corr;
                if( !p_compress ) {
                    print_string( p_out, p_P.slice( fragP ), tikz_point{ pxpos, p_PtopLeft.m_y },
                                  p_startIndent, p_indent );
                }
                pxpos += CHAR_WIDTH * fragP.length( );
            } else {
                pxpos += CHAR_WIDTH * fragT.length( ) + corr;
            }
            if( fragT.length( ) ) {
                txpos += corr;
                if( !p_compress ) {
                    print_string( p_out, p_T.slice( fragT ), tikz_point{ txpos, p_TtopLeft.m_y },
                                  p_startIndent, p_indent );
                }
                txpos += CHAR_WIDTH * fragT.length( );
            } else {
                txpos += CHAR_WIDTH * fragP.length( ) + corr;
            }
        }

        if( p_compress ) {
            print_string( p_out, pnew, tikz_point{ pxinit, p_PtopLeft.m_y }, p_startIndent,
                          p_indent );
            print_string( p_out, tnew, tikz_point{ txinit, p_TtopLeft.m_y }, p_startIndent,
                          p_indent );
        }

        return { tikz_point{ pxpos, p_PtopLeft.m_y }, tikz_point{ txpos, p_TtopLeft.m_y } };
    }

} // namespace TIKZ

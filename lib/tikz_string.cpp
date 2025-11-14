#include "tikz.h"
#include "tikz_string.h"
#include "wildcard.h"

void print_width_macro( FILE* p_out, double p_width, const std::string& p_text, u32 p_startIndent,
                        u32 p_indent ) {
    INDENT_PRINT( p_startIndent )( p_out,
                                   "\\pgfmathsetmacro\\twd{ %5.3lf * 1cm / width(\"%s\") }\n",
                                   p_width, p_text.c_str( ) );
}

void print_string_inner( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                         u32 p_startIndent, u32 p_indent ) {
    auto        posx     = p_StopLeft.m_x + ( p_S.length( ) * CHAR_WIDTH ) / 2.0;
    auto        posy     = p_StopLeft.m_y - CHAR_HEIGHT / 2.0;
    std::string extraopt = p_S.m_color.to_string( ) + ", inner sep = 2pt, xscale = {min(1, \\twd)}";

    if( p_S.print_single_characters( ) ) {
        for( u32 pos = p_S.m_fragment.closed_begin( ); pos < p_S.m_fragment.open_end( ); ++pos ) {
            auto text = p_S.render_pos( pos );
            if( text == EMPTY_STR || text == " " ) { continue; }
            auto ftext = p_S.resize_rendering( text );

            auto posxi = p_StopLeft.m_x + ( pos - p_S.m_fragment.closed_begin( ) ) * CHAR_WIDTH
                         + CHAR_WIDTH / 2.0;

            print_width_macro( p_out, CHAR_WIDTH, text, p_startIndent, p_indent );

            std::string exo = extraopt;
            if( p_S.m_annotation.count( pos )
                && p_S.m_annotation.at( pos ).m_textColor.is_non_empty( ) ) {
                exo += ", color = " + p_S.m_annotation.at( pos ).m_textColor.to_string( );
            }
            print_node( p_out, tikz_point{ posxi, posy }, ftext, exo, EMPTY_STR, p_startIndent,
                        p_indent );
        }
    } else {
        if( p_S.m_labelAlign == AN_BEGIN ) {
            extraopt += ", anchor = west";
            posx = p_StopLeft.m_x;
        } else if( p_S.m_labelAlign == AN_END ) {
            extraopt += ", anchor = east";
            posx = p_StopLeft.m_x + ( p_S.length( ) * CHAR_WIDTH );
        }

        auto text  = p_S.render_complete( );
        auto ftext = p_S.resize_rendering( text );
        print_width_macro( p_out, p_S.length( ) * CHAR_WIDTH, text, p_startIndent, p_indent );
        print_node( p_out, tikz_point{ posx, posy }, ftext, extraopt, EMPTY_STR, p_startIndent,
                    p_indent );
    }
}

void print_string_inner_vertical( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                                  u32 p_startIndent, u32 p_indent ) {
    auto        posx     = p_StopLeft.m_x + ( CHAR_WIDTH ) / 2.0;
    auto        posy     = p_StopLeft.m_y - ( p_S.length( ) * CHAR_HEIGHT ) / 2.0;
    std::string extraopt = p_S.m_color.to_string( ) + ", inner sep = 2pt, xscale = {min(1, \\twd)}";
    std::string extraopt_rotate
        = p_S.m_color.to_string( ) + ", inner sep = 2pt, yscale = {min(1, \\twd)}, rotate = 90";

    if( p_S.print_single_characters( ) ) {
        for( u32 pos = p_S.m_fragment.closed_begin( ); pos < p_S.m_fragment.open_end( ); ++pos ) {
            auto text  = p_S.render_pos( pos );
            auto ftext = p_S.resize_rendering( text );

            auto posyi = p_StopLeft.m_y - ( pos - p_S.m_fragment.closed_begin( ) ) * CHAR_HEIGHT
                         - CHAR_HEIGHT / 2.0;

            print_width_macro( p_out, CHAR_WIDTH, text, p_startIndent, p_indent );

            std::string exo = p_S.rotatable( ) ? extraopt_rotate : extraopt;
            if( p_S.m_annotation.count( pos )
                && p_S.m_annotation.at( pos ).m_textColor.is_non_empty( ) ) {
                exo += ", color = " + p_S.m_annotation.at( pos ).m_textColor.to_string( );
            }
            print_node( p_out, tikz_point{ posx, posyi }, ftext, exo, EMPTY_STR, p_startIndent,
                        p_indent );
        }
    } else {
        if( p_S.m_labelAlign == AN_BEGIN ) {
            extraopt += ", anchor = north";
            extraopt_rotate += ", anchor = north";
            posy = p_StopLeft.m_y;
        } else if( p_S.m_labelAlign == AN_END ) {
            extraopt += ", anchor = south";
            extraopt_rotate += ", anchor = south";
            posy = p_StopLeft.m_y - ( p_S.length( ) * CHAR_HEIGHT );
        }

        auto text  = p_S.render_complete( );
        auto ftext = p_S.resize_rendering( text );
        print_width_macro( p_out, p_S.rotatable( ) ? p_S.length( ) * CHAR_WIDTH : CHAR_WIDTH, text,
                           p_startIndent, p_indent );
        print_node( p_out, tikz_point{ posx, posy }, ftext,
                    p_S.rotatable( ) ? extraopt_rotate : extraopt, EMPTY_STR, p_startIndent,
                    p_indent );
    }
}

// Prints a string S
void print_string( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                   u32 p_startIndent, u32 p_indent ) {
    if( !p_S.length( ) ) { return; }

    double cor3 = -0.035;
    INDENT_PRINT( p_startIndent )(
        p_out,
        "\\fill[rounded corners = 2pt, %s] "
        "(%5.3lf + %5.3lf, %5.3lf - %5.3lf) rectangle"
        "(%5.3lf - %5.3lf, %5.3lf + %5.3lf);\n",
        p_S.m_color.to_flavor_bg( ).c_str( ), p_StopLeft.m_x, cor3, p_StopLeft.m_y, cor3,
        p_StopLeft.m_x + p_S.length( ) * CHAR_WIDTH, cor3, p_StopLeft.m_y - CHAR_HEIGHT, cor3 );

    // for each character, print a small rectangle
    for( u32 i = 0; i < p_S.length( ); ++i ) {
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
        p_S.m_color.to_flavor_bg( ).c_str( ), p_StopLeft.m_x, cor3, p_StopLeft.m_y, cor3,
        p_StopLeft.m_x + p_S.length( ) * CHAR_WIDTH, cor3, p_StopLeft.m_y - CHAR_HEIGHT, cor3 );

    for( u32 i = 0; i < p_S.length( ); ++i ) {
        double cor1 = i ? .04 : .05, cor2 = ( i + 1 < p_S.length( ) ) ? .04 : .05;
        auto   ann = p_S.annotation_at_pos( i );
        if( ann.m_bgColor.is_non_empty( ) ) {
            auto bgcol = ann.m_bgColor;
            INDENT_PRINT( p_startIndent )(
                p_out,
                "\\fill[rounded corners = 2pt,  %s] "
                "(%5.3lf + %5.3lf, %5.3lf) rectangle (%5.3lf - %5.3lf, %5.3lf);\n",
                bgcol.c_str( ), p_StopLeft.m_x + i * CHAR_WIDTH, cor1, p_StopLeft.m_y - .05,
                p_StopLeft.m_x + ( i + 1 ) * CHAR_WIDTH, cor2, p_StopLeft.m_y - CHAR_HEIGHT + .05 );
        }
    }

    print_string_inner( p_out, p_S, p_StopLeft, p_startIndent, p_indent );

    double cor4 = .02;
    // add little dots to highlight parts between characters
    for( u32 i = 1; i < p_S.length( ); ++i ) {
        INDENT_PRINT( p_startIndent )(
            p_out, "\\node[fill, circle, inner sep =0.1pt, %s] at (%5.3lf, %5.3lf - %5.3lf) {};\n",
            p_S.m_color.c_str( ), p_StopLeft.m_x + i * CHAR_WIDTH, p_StopLeft.m_y, cor4 );
        INDENT_PRINT( p_startIndent )(
            p_out, "\\node[fill, circle, inner sep =0.1pt, %s] at (%5.3lf, %5.3lf + %5.3lf) {};\n",
            p_S.m_color.c_str( ), p_StopLeft.m_x + i * CHAR_WIDTH, p_StopLeft.m_y - CHAR_HEIGHT,
            cor4 );
    }
}

// Prints a string S vertically
void print_string_vertical( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft,
                            u32 p_startIndent, u32 p_indent ) {
    if( !p_S.length( ) ) { return; }
    double cor3 = -0.035;
    INDENT_PRINT( p_startIndent )( p_out,
                                   "\\fill[rounded corners=2pt, %s] "
                                   "(%5.3lf + %5.3lf, %5.3lf - %5.3lf) rectangle"
                                   "(%5.3lf - %5.3lf, %5.3lf + %5.3lf);\n",
                                   p_S.m_color.to_flavor_bg( ).c_str( ), p_StopLeft.m_x, cor3,
                                   p_StopLeft.m_y, cor3, p_StopLeft.m_x + CHAR_WIDTH, cor3,
                                   p_StopLeft.m_y - p_S.length( ) * CHAR_HEIGHT, cor3 );

    // for each character, print a small rectangle
    for( u32 i = 0; i < p_S.length( ); ++i ) {
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
                                   p_S.m_color.to_flavor_bg( ).c_str( ), p_StopLeft.m_x, cor3,
                                   p_StopLeft.m_y, cor3, p_StopLeft.m_x + CHAR_WIDTH, cor3,
                                   p_StopLeft.m_y - p_S.length( ) * CHAR_HEIGHT, cor3 );

    for( u32 i = 0; i < p_S.length( ); ++i ) {
        double cor1 = i ? .04 : 0.05, cor2 = ( i + 1 < p_S.length( ) ) ? .04 : .05;
        auto   ann = p_S.annotation_at_pos( i );
        if( ann.m_bgColor.is_non_empty( ) ) {
            auto bgcol = ann.m_bgColor;
            INDENT_PRINT( p_startIndent )(
                p_out,
                "\\draw[rounded corners = 2pt, %s] "
                "(%5.3lf, %5.3lf - %5.3lf) rectangle (%5.3lf, %5.3lf + %5.3lf);\n",
                bgcol.c_str( ), p_StopLeft.m_x + .05, p_StopLeft.m_y - i * CHAR_HEIGHT, cor1,
                p_StopLeft.m_x + CHAR_WIDTH - .05, p_StopLeft.m_y - ( i + 1 ) * CHAR_HEIGHT, cor2 );
        }
    }

    print_string_inner_vertical( p_out, p_S, p_StopLeft, p_startIndent, p_indent );

    double cor4 = .02;
    // add little dots to highlight parts between characters
    for( u32 i = 1; i < p_S.length( ); ++i ) {
        INDENT_PRINT( p_startIndent )(
            p_out, "\\node[fill, circle, inner sep = 0.1pt, %s] at (%5.3lf + %5.3lf, %5.3lf) {};\n",
            p_S.m_color.c_str( ), p_StopLeft.m_x, cor4, p_StopLeft.m_y - i * CHAR_HEIGHT );
        INDENT_PRINT( p_startIndent )(
            p_out, "\\node[fill, circle, inner sep = 0.1pt, %s] at (%5.3lf - %5.3lf, %5.3lf) {};\n",
            p_S.m_color.c_str( ), p_StopLeft.m_x + CHAR_WIDTH, cor4,
            p_StopLeft.m_y - i * CHAR_HEIGHT );
    }
}

void print_separator( FILE* p_out, tikz_point p_PtopLeft, tikz_point p_TtopLeft, color p_color,
                      u32 p_startIndent, u32 p_indent ) {
    // make sure T is top, P is bottom
    if( p_PtopLeft.m_y > p_TtopLeft.m_y ) { std::swap( p_PtopLeft, p_TtopLeft ); }

    INDENT_PRINT( p_startIndent )(
        p_out,
        "\\node[rotate = 45, outer sep = 0pt, inner sep = 1pt, fill = %s] at (%5.3lf, %5.3lf + %5.3lf) {};\n",
        p_color.c_str( ), p_TtopLeft.m_x, p_TtopLeft.m_y, 1.5 * CHAR_GLOW );
    INDENT_PRINT( p_startIndent )(
        p_out,
        "\\node[rotate = 45, outer sep = 0pt, inner sep = 1pt, fill = %s] at (%5.3lf, %5.3lf - %5.3lf) {};\n",
        p_color.c_str( ), p_PtopLeft.m_x, p_PtopLeft.m_y - CHAR_HEIGHT, 1.5 * CHAR_GLOW );

    INDENT_PRINT( p_startIndent )( p_out, "\\draw[%s, rounded corners=2pt, %s]\n", LW_LINE.c_str( ),
                                   p_color.c_str( ) );
    INDENT_PRINT( 1 + p_startIndent )(
        p_out,
        "(%5.3lf, %5.3lf + %5.3lf) -- (%5.3lf, %5.3lf) -- (%5.3lf, %5.3lf) -- (%5.3lf, %5.3lf - %5.3lf);\n",
        p_TtopLeft.m_x, p_TtopLeft.m_y, 1.5 * CHAR_GLOW, p_TtopLeft.m_x,
        p_TtopLeft.m_y - CHAR_HEIGHT, p_PtopLeft.m_x, p_PtopLeft.m_y, p_PtopLeft.m_x,
        p_PtopLeft.m_y - CHAR_HEIGHT, 1.5 * CHAR_GLOW );
}

void print_matched_string_pair( FILE* p_out, const stylized_string& p_P, tikz_point p_PtopLeft,
                                const stylized_string& p_T, tikz_point p_TtopLeft, color p_bgColor,
                                u32 p_startIndent, u32 p_indent ) {
    // print glow bubble
    INDENT_PRINT( p_startIndent )( p_out, "\\draw[%s, rounded corners = 2pt, %s, fill = %s]\n",
                                   LW_THIN_OUTLINE.c_str( ), p_bgColor.c_str( ),
                                   p_bgColor.to_bg( ).c_str( ) );
    // top line
    INDENT_PRINT( 1 + p_startIndent )(
        p_out,
        "(%5.3lf - %5.3lf, %5.3lf + %5.3lf) -- (%5.3lf + %5.3lf, %5.3lf + %5.3lf) -- (%5.3lf + %5.3lf, %5.3lf - %5.3lf)\n",
        p_TtopLeft.m_x, CHAR_GLOW, p_TtopLeft.m_y, CHAR_GLOW, // top left
        p_TtopLeft.m_x + p_T.length( ) * CHAR_WIDTH, CHAR_GLOW, p_TtopLeft.m_y,
        CHAR_GLOW, // top
                   // right
        p_TtopLeft.m_x + p_T.length( ) * CHAR_WIDTH, CHAR_GLOW, p_TtopLeft.m_y - CHAR_HEIGHT,
        0 * CHAR_GLOW // top-mid right
    );
    INDENT_PRINT( 1 + p_startIndent )(
        p_out,
        "-- (%5.3lf + %5.3lf, %5.3lf + %5.3lf) -- (%5.3lf + %5.3lf, %5.3lf - %5.3lf) -- (%5.3lf - %5.3lf, %5.3lf - %5.3lf)\n",
        p_PtopLeft.m_x + p_P.length( ) * CHAR_WIDTH, CHAR_GLOW, p_PtopLeft.m_y,
        0 * CHAR_GLOW, // bot-mid right
        p_PtopLeft.m_x + p_P.length( ) * CHAR_WIDTH, CHAR_GLOW, p_PtopLeft.m_y - CHAR_HEIGHT,
        CHAR_GLOW,                                                         // bot right
        p_PtopLeft.m_x, CHAR_GLOW, p_PtopLeft.m_y - CHAR_HEIGHT, CHAR_GLOW // bot left
    );
    INDENT_PRINT( 1 + p_startIndent )(
        p_out,
        "-- (%5.3lf - %5.3lf, %5.3lf + %5.3lf) -- (%5.3lf - %5.3lf, %5.3lf - %5.3lf) -- cycle;\n",
        p_PtopLeft.m_x, CHAR_GLOW, p_PtopLeft.m_y,
        0 * CHAR_GLOW, // bot-mid left
        p_TtopLeft.m_x, CHAR_GLOW, p_TtopLeft.m_y - CHAR_HEIGHT,
        0 * CHAR_GLOW // top-mid left
    );

    if( p_P.length( ) == p_T.length( ) ) {
        // print "matching edges"
        INDENT_PRINT( p_startIndent )( p_out, "\\draw[%s, %s]\n", LW_SUPPORT_LINE.c_str( ),
                                       p_bgColor.deemphasize( ).c_str( ) );
        for( u32 i = 0; i < p_P.length( ); ++i ) {
            INDENT_PRINT( 1 + p_startIndent )(
                p_out, "(%5.3lf, %5.3lf) -- (%5.3lf, %5.3lf)\n",
                p_TtopLeft.m_x + ( i + .5 ) * CHAR_WIDTH, p_TtopLeft.m_y - CHAR_HEIGHT,
                p_PtopLeft.m_x + ( i + .5 ) * CHAR_WIDTH, p_PtopLeft.m_y );
        }
        INDENT_PRINT( p_startIndent )( p_out, ";\n" );
        for( u32 i = 0; i < p_P.length( ); ++i ) {
            // if characters are printed, check that they actually match
            auto p = p_P[ i ];
            auto t = p_T[ i ];

            if( p != EMPTY_STR && t != EMPTY_STR && p != t ) {
                // if they don't, draw a big cross symbolizing the replacement
                print_cross( p_out,
                             tikz_point{ ( p_TtopLeft.m_x + ( i + .5 ) * CHAR_WIDTH + p_PtopLeft.m_x
                                           + ( i + .5 ) * CHAR_WIDTH )
                                             / 2.0,
                                         ( p_TtopLeft.m_y - CHAR_HEIGHT - p_PtopLeft.m_y ) / 2.0 },
                             p_bgColor.deemphasize_weak( ), p_startIndent, p_indent );
            }
        }
    }

    if( p_P.length( ) ) { print_string( p_out, p_P, p_PtopLeft, p_startIndent, p_indent ); }
    if( p_T.length( ) ) { print_string( p_out, p_T, p_TtopLeft, p_startIndent, p_indent ); }
}

std::string char_or_empty( char p_char ) {
    if( !p_char ) { return "\\varepsilon"; }
    return text_typewriter( std::string{ p_char } );
}

std::string to_stringT( const breakpoint& p_bp ) {
    return textsize_footnotesize( math_mode( std::string{ "(" } + std::to_string( p_bp.m_posT )
                                             + ", " + char_or_empty( p_bp.m_charT ) + ")" ) );
}

std::string to_stringP( const breakpoint& p_bp ) {
    return textsize_footnotesize( math_mode( std::string{ "(" } + std::to_string( p_bp.m_posP )
                                             + ", " + char_or_empty( p_bp.m_charP ) + ")" ) );
}

color color_for_bp( const breakpoint& p_bp ) {
    if( p_bp.m_charP && p_bp.m_charT ) {
        // substitution
        return SUB_COL;
    } else if( p_bp.m_charP ) {
        // deletion
        return DEL_COL;
    } else if( p_bp.m_charT ) {
        // insertion
        return INS_COL;
    } else {
        // dummy breakpoint
        return SEP_COL;
    }
}

std::pair<tikz_point, tikz_point>
print_alignment( FILE* p_out, const stylized_string& p_P, tikz_point p_PtopLeft,
                 const stylized_string& p_T, tikz_point p_TtopLeft, const breakpoint_repn& p_brpnt,
                 bool p_printBreakpoints, u32 p_startIndent, u32 p_indent ) {
    double pxpos = p_PtopLeft.m_x, txpos = p_TtopLeft.m_x;
    double last = pxpos, lastt = txpos;
    double sepWidth = 3 * CHAR_GLOW, extraGlow = 3.5 * CHAR_GLOW;
    u32    shiftP = 0, shiftT = 0;

    if( p_printBreakpoints ) {
        // initial dummy breakpoint
        print_separator( p_out, p_PtopLeft, p_TtopLeft, SEP_COL.deemphasize( ), p_startIndent,
                         p_indent );
        print_text( p_out, to_stringP( p_brpnt.front( ) ),
                    tikz_point{ p_PtopLeft.m_x - 3 * CHAR_GLOW,
                                p_PtopLeft.m_y - CHAR_HEIGHT - 3 * CHAR_GLOW },
                    "anchor = north west", SEP_COL, p_startIndent, p_indent );
        print_text( p_out, to_stringT( p_brpnt.front( ) ),
                    tikz_point{ p_TtopLeft.m_x - 3 * CHAR_GLOW, p_TtopLeft.m_y + 3 * CHAR_GLOW },
                    "anchor = south west", SEP_COL, p_startIndent, p_indent );
        pxpos += sepWidth;
        txpos += sepWidth;
    }

    for( u32 i = 1; i < p_brpnt.size( ); ++i ) {
        auto bp = p_brpnt[ i ], prevbp = p_brpnt[ i - 1 ];
        auto fragP = fragmentco{ prevbp.m_posP + shiftP, bp.m_posP };
        auto fragT = fragmentco{ prevbp.m_posT + shiftT, bp.m_posT };

        if( fragT.length( ) > 0 && fragP.length( ) > 0 ) {
            // matched fragment
            print_matched_string_pair(
                p_out, p_P.slice( fragP ), tikz_point{ pxpos, p_PtopLeft.m_y }, p_T.slice( fragT ),
                tikz_point{ txpos, p_TtopLeft.m_y }, MAT_COL, p_startIndent, p_indent );
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
            print_text( p_out, to_stringP( p_brpnt[ i ] ),
                        tikz_point{ posx, p_PtopLeft.m_y - CHAR_HEIGHT - sepWidth - extray },
                        anchorp, col, p_startIndent, p_indent );
            print_text( p_out, to_stringT( p_brpnt[ i ] ),
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
            } else if( bp.m_charT ) { // insertion
                shiftT = 1;
                shiftP = 0;
            } else if( bp.m_charP ) { // deletion
                shiftP = 1;
                shiftT = 0;
            }
            print_matched_string_pair(
                p_out, p_P.slice( { bp.m_posP, bp.m_posP + shiftP }, bp.m_charP ),
                tikz_point{ pxpos, p_PtopLeft.m_y },
                p_T.slice( { bp.m_posT, bp.m_posT + shiftT }, bp.m_charT ),
                tikz_point{ txpos, p_TtopLeft.m_y }, color_for_bp( bp ), p_startIndent, p_indent );
            pxpos += shiftP * CHAR_WIDTH + extraGlow;
            txpos += shiftT * CHAR_WIDTH + extraGlow;
        }
    }

    return { tikz_point{ pxpos, p_PtopLeft.m_y }, tikz_point{ txpos, p_TtopLeft.m_y } };
}

void print_alignment_graph_label( FILE* p_out, const stylized_string& p_P,
                                  const stylized_string& p_T, tikz_point p_gridTopLeft,
                                  u32 p_startIndent, u32 p_indent ) {
    print_string( p_out, p_T, p_gridTopLeft + tikz_point{ .0, CHAR_HEIGHT + .25 }, p_startIndent,
                  p_indent );
    print_string_vertical( p_out, p_P, p_gridTopLeft + tikz_point{ -CHAR_WIDTH - .25, .0 },
                           p_startIndent, p_indent );
}

vertex_grid print_alignment_graph( FILE* p_out, const std::string& p_P, fragmentco p_fragP,
                                   const std::string& p_T, fragmentco p_fragT,
                                   tikz_point p_gridCellSize, tikz_point p_gridTopLeft,
                                   color p_color, u32 p_startIndent, u32 p_indent ) {
    vertex_grid vg{ p_gridTopLeft, p_gridCellSize };
    vg.print_vertices( p_out, vertex::unselected_vertex( p_color ), p_fragT.length( ) + 1,
                       p_fragP.length( ) + 1, p_fragT.closed_begin( ), p_fragP.closed_begin( ),
                       EMPTY_STR, p_startIndent, p_indent );

    for( u32 x = p_fragT.closed_begin( ); x <= p_fragT.open_end( ); ++x ) {
        for( u32 y = p_fragP.closed_begin( ); y <= p_fragP.open_end( ); ++y ) {
            if( x > p_fragT.closed_begin( ) ) {
                print_arrow( p_out, vg.label_for_pos( x - 1, y ), vg.label_for_pos( x, y ),
                             LW_SUPPORT_LINE, INS_COL.deemphasize_strong( ) );
            }
            if( y > p_fragP.closed_begin( ) ) {
                print_arrow( p_out, vg.label_for_pos( x, y - 1 ), vg.label_for_pos( x, y ),
                             LW_SUPPORT_LINE, DEL_COL.deemphasize_strong( ) );
            }
            if( x > p_fragT.closed_begin( ) && y > p_fragP.closed_begin( ) ) {
                if( p_P[ y - 1 ] == p_T[ x - 1 ] ) {
                    print_arrow( p_out, vg.label_for_pos( x - 1, y - 1 ), vg.label_for_pos( x, y ),
                                 LW_THICK_SUPPORT_LINE, MAT_COL.deemphasize( ) );
                } else {
                    print_arrow( p_out, vg.label_for_pos( x - 1, y - 1 ), vg.label_for_pos( x, y ),
                                 LW_SUPPORT_LINE, SUB_COL.deemphasize_strong( ) );
                }
            }
        }
    }
    return vg;
}

vertex_grid print_alignment_graph_simple( FILE* p_out, fragmentco p_fragP, fragmentco p_fragT,
                                          tikz_point p_gridCellSize, tikz_point p_gridTopLeft,
                                          color p_color, u32 p_startIndent, u32 p_indent ) {
    vertex_grid vg{ p_gridTopLeft, p_gridCellSize };
    vg.print_vertices( p_out, vertex::unselected_vertex( p_color ), p_fragT.length( ) + 1,
                       p_fragP.length( ) + 1, p_fragT.closed_begin( ), p_fragP.closed_begin( ),
                       EMPTY_STR, p_startIndent, p_indent );

    for( u32 x = p_fragT.closed_begin( ); x <= p_fragT.open_end( ); ++x ) {
        for( u32 y = p_fragP.closed_begin( ); y <= p_fragP.open_end( ); ++y ) {
            if( x > p_fragT.closed_begin( ) ) {
                print_arrow( p_out, vg.label_for_pos( x - 1, y ), vg.label_for_pos( x, y ),
                             LW_SUPPORT_LINE, p_color.deemphasize( ) );
            }
            if( y > p_fragP.closed_begin( ) ) {
                print_arrow( p_out, vg.label_for_pos( x, y - 1 ), vg.label_for_pos( x, y ),
                             LW_SUPPORT_LINE, p_color.deemphasize( ) );
            }
            if( x > p_fragT.closed_begin( ) && y > p_fragP.closed_begin( ) ) {
                print_arrow( p_out, vg.label_for_pos( x - 1, y - 1 ), vg.label_for_pos( x, y ),
                             LW_SUPPORT_LINE, p_color.deemphasize( ) );
            }
        }
    }
    return vg;
}

void print_alignment_on_coordinates( FILE* p_out, const vertex_grid& p_vg,
                                     const breakpoint_repn& p_brpnt, u32 p_startIndent,
                                     u32 p_indent ) {
    for( u32 i = p_brpnt.size( ) - 1; i; --i ) {
        auto curpos  = p_brpnt[ i ].position( );
        auto prev    = p_brpnt[ i - 1 ];
        auto prevpos = prev.position( );

        // draw "selected arrow" from prev to curr.
        auto preva
            = point{ prevpos.first + ( !!prev.m_charP ), prevpos.second + ( !!prev.m_charT ) };

        if( preva != curpos ) {
            if( true /* print single step of matching edge */ ) {
                auto pp = point{ curpos.first - 1, curpos.second - 1 };
                while( pp != preva ) {
                    print_selected_arrow( p_out, p_vg.label_for_pos( pp.second, pp.first ),
                                          p_vg.label_for_pos( curpos.second, curpos.first ),
                                          MAT_COL, MAT_COL.to_bg( ), -45.0, p_startIndent,
                                          p_indent );
                    curpos = pp;
                    pp     = point{ curpos.first - 1, curpos.second - 1 };
                }
            }
            print_selected_arrow( p_out, p_vg.label_for_pos( preva.second, preva.first ),
                                  p_vg.label_for_pos( curpos.second, curpos.first ), MAT_COL,
                                  MAT_COL.to_bg( ), -45.0, p_startIndent, p_indent );
        }

        if( prev.m_charT && prev.m_charP ) {
            // substitution
            print_selected_arrow( p_out, p_vg.label_for_pos( prevpos.second, prevpos.first ),
                                  p_vg.label_for_pos( preva.second, preva.first ), SUB_COL,
                                  SUB_COL.to_bg( ), -45.0, p_startIndent, p_indent );
        } else if( prev.m_charP ) {
            // deletion
            print_selected_arrow( p_out, p_vg.label_for_pos( prevpos.second, prevpos.first ),
                                  p_vg.label_for_pos( preva.second, preva.first ), DEL_COL,
                                  DEL_COL.to_bg( ), -90.0, p_startIndent, p_indent );
        } else if( prev.m_charT ) {
            // insertion
            print_selected_arrow( p_out, p_vg.label_for_pos( prevpos.second, prevpos.first ),
                                  p_vg.label_for_pos( preva.second, preva.first ), INS_COL,
                                  INS_COL.to_bg( ), -0.0, p_startIndent, p_indent );
        }
    }
}

void print_graph_slice_on_coordinates( FILE* p_out, const vertex_grid& p_vg,
                                       const graph_slice& p_gs, color p_color, color p_portalColor,
                                       color p_innerPortalColor, color p_topColor,
                                       color p_bottomColor, u32 p_startIndent, u32 p_indent ) {
    double vtxsz = 1.25;
    double inner = .5;
    if( p_color.is_non_empty( ) ) {
        vtxsz       = 1.75;
        inner       = .72;
        double glow = 0 * CHAR_GLOW;
        double corX = .5 * CHAR_GLOW;

        if( !p_gs.m_topFrontier.empty( ) || !p_gs.m_bottomFrontier.empty( ) ) {
            // print glow bubble
            INDENT_PRINT( p_startIndent )(
                p_out, "\\draw[%s, rounded corners = 1.25pt, %s, fill = %s]\n", LW_OUTLINE.c_str( ),
                p_color.c_str( ), p_color.to_bg( ).c_str( ) );
            // top line
            for( u32 i = 0; i < p_gs.m_topFrontier.size( ); ++i ) {
                const auto& pt   = p_gs.m_topFrontier[ i ];
                auto        tp   = p_vg.point_for_pos( pt.first, pt.second );
                double      corx = corX;
                if( !i ) { corx = -corX; }
                if( p_gs.width( ) ) { corx = 0.0; }

                double gy = glow;
                if( p_topColor.is_non_empty( )
                    && pt.second == p_gs.m_topFrontier.front( ).second ) {
                    gy = 0.0;
                }
                INDENT_PRINT( 1 + p_startIndent )( p_out, "(%5.3lf + %5.3lf, %5.3lf + %5.3lf) --\n",
                                                   tp.m_x, corx, tp.m_y, gy );
            }

            // bottom line
            for( u32 i = 0; i < p_gs.m_bottomFrontier.size( ); ++i ) {
                const auto& pt   = p_gs.m_bottomFrontier[ i ];
                auto        tp   = p_vg.point_for_pos( pt.first, pt.second );
                double      corx = -corX;
                if( !i ) { corx = corX; }
                if( p_gs.width( ) ) { corx = 0.0; }

                double gy = glow;
                if( p_bottomColor.is_non_empty( )
                    && pt.second == p_gs.m_bottomFrontier.front( ).second ) {
                    gy = 0.0;
                }
                INDENT_PRINT( 1 + p_startIndent )( p_out, "(%5.3lf + %5.3lf, %5.3lf - %5.3lf) --\n",
                                                   tp.m_x, corx, tp.m_y, gy );
            }
            INDENT_PRINT( 1 + p_startIndent )( p_out, "cycle;\n" );
        }
    }

    if( p_portalColor.is_non_empty( ) ) {
        auto portal_inner = vertex::unselected_vertex( COLOR_FILL_WHITE, vtxsz - .7 );
        auto portal
            = vertex::marked_vertex( p_portalColor, vtxsz, .75, vertex::ST_CIRCLE, portal_inner );
        for( const auto& pt : p_gs.m_leftPortals ) {
            p_vg.print_vertex_on_coordinates( p_out, portal, pt.first, pt.second );
        }
        for( const auto& pt : p_gs.m_rightPortals ) {
            p_vg.print_vertex_on_coordinates( p_out, portal, pt.first, pt.second );
        }
    }

    if( p_innerPortalColor.is_non_empty( ) ) {
        auto portal_inner = vertex::unselected_vertex( COLOR_FILL_WHITE, vtxsz - .7 );
        auto portal = vertex::marked_vertex( p_innerPortalColor, vtxsz, .75, vertex::ST_CIRCLE,
                                             portal_inner );
        for( const auto& pt : p_gs.m_innerPortals ) {
            p_vg.print_vertex_on_coordinates( p_out, portal, pt.first, pt.second );
        }
    }

    if( p_topColor.is_non_empty( ) ) {
        auto portal_inner
            = vertex::unselected_vertex( COLOR_FILL_WHITE, inner, .75, vertex::ST_TRIANGLE_UP );
        auto portal
            = vertex::marked_vertex( p_topColor, vtxsz, .75, vertex::ST_CIRCLE, portal_inner );
        auto [ x, y ] = p_gs.m_topFrontier.front( );
        u32 xm        = x;
        for( const auto& pt : p_gs.m_topFrontier ) {
            if( pt.second == y ) {
                xm = pt.first;
            } else {
                break;
            }
        }
        for( auto i = x; i <= xm; ++i ) { p_vg.print_vertex_on_coordinates( p_out, portal, i, y ); }
    }

    if( p_bottomColor.is_non_empty( ) ) {
        auto portal_inner
            = vertex::unselected_vertex( COLOR_FILL_WHITE, inner, .75, vertex::ST_TRIANGLE_DOWN );
        auto portal
            = vertex::marked_vertex( p_bottomColor, vtxsz, .75, vertex::ST_CIRCLE, portal_inner );
        auto [ x, y ] = p_gs.m_bottomFrontier.front( );
        u32 xm        = x;
        for( const auto& pt : p_gs.m_bottomFrontier ) {
            if( pt.second == y ) {
                xm = pt.first;
            } else {
                break;
            }
        }
        for( auto i = xm; i <= x; ++i ) { p_vg.print_vertex_on_coordinates( p_out, portal, i, y ); }
    }
}

void print_graph_slices_stylized( FILE* p_out, const std::deque<graph_slice>& p_gs,
                                  breakpoint_repn p_alignment, color p_portalVertexColor,
                                  color p_innerPortalColor, color p_topVertexColor,
                                  color p_bottomVertexColor, color p_pureColor, color p_impureColor,
                                  color p_leftColor, color p_rightColor, double p_sliceSpacing,
                                  u32 p_startIndent, u32 p_indent ) {
    fragmentco frag{ p_gs.front( ).fragment_p( ).closed_begin( ),
                     p_gs.back( ).fragment_p( ).open_end( ) };
    auto       align = breakpoint_slice( p_alignment, frag );

    for( u32 i = 0; i < p_gs.size( ); ++i ) {
        const auto& slice = p_gs[ i ];
        auto        fP    = slice.fragment_p( );
        auto        fPo   = slice.original_fragment_p( );
        vertex_grid vg{ tikz_point{ p_sliceSpacing * i, 0.0 },
                        tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT } };
        vg.print_coordinates( p_out, slice.width( ) + 1, slice.height( ) + 1,
                              slice.top_left( ).first, slice.top_left( ).second, EMPTY_STR,
                              p_startIndent, p_indent );

        auto bgcol = p_impureColor;
        if( !!( slice.m_flags & gs_flag_t::GSF_PURE ) ) { bgcol = p_pureColor; }
        if( !!( slice.m_flags & gs_flag_t::GSF_TOP ) ) { bgcol = p_leftColor; }
        if( !!( slice.m_flags & gs_flag_t::GSF_BOT ) ) { bgcol = p_rightColor; }

        print_graph_slice_on_coordinates(
            p_out, vg, slice, bgcol, p_portalVertexColor, p_innerPortalColor,
            fP.closed_begin( ) == frag.closed_begin( ) ? p_topVertexColor : COLOR_NONE,
            fP.closed_end( ) == frag.closed_end( ) ? p_bottomVertexColor : COLOR_NONE,
            p_startIndent, p_indent );

        auto brpnt = breakpoint_slice( align, fPo );
        print_alignment_on_coordinates( p_out, vg, brpnt, p_startIndent, p_indent );
    }
}

void print_graph_slices( FILE* p_out, const std::deque<graph_slice>& p_gs,
                         breakpoint_repn p_alignment, const std::string& p_P,
                         const std::string& p_T, color p_portalVertexColor,
                         color p_innerPortalColor, color p_topVertexColor,
                         color p_bottomVertexColor, double p_sliceSpacing, u32 p_startIndent,
                         u32 p_indent ) {
    fragmentco frag{ p_gs.front( ).fragment_p( ).closed_begin( ),
                     p_gs.back( ).fragment_p( ).open_end( ) };
    auto       align = breakpoint_slice( p_alignment, frag );

    for( u32 i = 0; i < p_gs.size( ); ++i ) {
        const auto& slice = p_gs[ i ];
        auto        fP    = slice.fragment_p( );
        auto        fPo   = slice.original_fragment_p( );

        vertex_grid vg;
        for( u32 j = 1; j < slice.m_topFrontier.size( ); ++j ) {

            auto fp2
                = fragmentco{ slice.m_topFrontier[ j - 1 ].second,
                              slice.m_bottomFrontier[ slice.m_bottomFrontier.size( ) - j ].second };
            auto ft2
                = fragmentco{ slice.m_topFrontier[ j - 1 ].first, slice.m_topFrontier[ j ].first };

            vg = print_alignment_graph( p_out, p_P, fp2, p_T, ft2,
                                        tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT },
                                        tikz_point{ p_sliceSpacing * i, 0.0 },
                                        COLOR_BLACK.deemphasize( ), p_startIndent, p_indent );
        }

        print_graph_slice_on_coordinates(
            p_out, vg, slice, COLOR_NONE, p_portalVertexColor, p_innerPortalColor,
            fP.closed_begin( ) == frag.closed_begin( ) ? p_topVertexColor : COLOR_NONE,
            fP.closed_end( ) == frag.closed_end( ) ? p_bottomVertexColor : COLOR_NONE,
            p_startIndent, p_indent );

        auto brpnt = breakpoint_slice( align, fPo );
        print_alignment_on_coordinates( p_out, vg, brpnt, p_startIndent, p_indent );
    }
}

void print_graph_slices_t_labels( FILE* p_out, const std::deque<graph_slice>& p_gs,
                                  const stylized_string& p_Tname, const stylized_string& p_Pname,
                                  bool p_labelEqualParts, double p_labelDis, double p_sliceSpacing,
                                  u32 p_startIndent, u32 p_indent ) {
    fragmentco frag{ p_gs.front( ).fragment_p( ).closed_begin( ),
                     p_gs.back( ).fragment_p( ).open_end( ) };
    for( u32 i = 0; i < p_gs.size( ); ++i ) {
        const auto& slice = p_gs[ i ];
        auto        fT    = slice.fragment_t( );
        auto        fPo   = slice.original_fragment_p( );

        auto posT  = tikz_point{ p_sliceSpacing * i + CHAR_WIDTH * fT.closed_begin( ),
                                frag.closed_begin( ) * -CHAR_HEIGHT + CHAR_HEIGHT + p_labelDis };
        auto posTB = tikz_point{ p_sliceSpacing * i + CHAR_WIDTH * fT.closed_begin( ),
                                 frag.open_end( ) * -CHAR_HEIGHT - p_labelDis };

        auto strT = p_Tname.slice( fT );
        print_string( p_out, strT, posT, p_startIndent, p_indent );

        if( p_labelEqualParts && !!( slice.m_flags & gs_flag_t::GSF_PURE ) ) {
            strT = p_Pname.slice( fPo );
        } else {
            strT.m_color = strT.m_color.deemphasize( );
        }
        print_string( p_out, strT, posTB, p_startIndent, p_indent );
    }
}

void print_graph_slices_p_labels( FILE* p_out, const std::deque<graph_slice>& p_gs, u32 p_d,
                                  bool p_guideLines, double p_labelDis, double p_sliceSpacing,
                                  u32 p_startIndent, u32 p_indent ) {

    fragmentco frag{ p_gs.front( ).fragment_p( ).closed_begin( ),
                     p_gs.back( ).fragment_p( ).open_end( ) };
    fragmentco fragT{ p_gs.front( ).fragment_t( ).closed_begin( ),
                      p_gs.back( ).fragment_t( ).open_end( ) };
    for( s32 i = p_gs.size( ) - 1; i >= 0; --i ) {
        const auto& slice = p_gs[ i ];
        auto        fP    = slice.fragment_p( );
        auto        fPo   = slice.original_fragment_p( );
        auto        fT    = slice.fragment_t( );

        auto        posP = tikz_point{ fragT.closed_begin( ) * CHAR_WIDTH - CHAR_WIDTH - p_labelDis
                                    - CHAR_WIDTH * i / 6.25,
                                fP.closed_begin( ) * -CHAR_HEIGHT };
        std::string label;
        if( fPo.length( ) == 1 ) {
            label = std::string{ "P_{" } + std::to_string( p_d ) + ","
                    + std::to_string( fPo.closed_begin( ) ) + "}";
        } else {
            label = std::string{ "P\\fragmentco{p^{<" } + std::to_string( p_d ) + "}_"
                    + std::to_string( fPo.closed_begin( ) ) + "}{ p^{>" + std::to_string( p_d )
                    + "}_" + std::to_string( fPo.open_end( ) ) + "}";
        }
        stylized_string strP = stylized_string{ label, str_displ_t::SDT_NAME }.slice( fP );
        if( fPo.length( ) > 1 ) {
            strP.m_displayStyle = str_displ_t::SDT_NAME_ROTATE;
        } else {
            strP.m_labelAlign = AN_END;
            strP.highlight_position( i - fP.closed_begin( ), SEP_COL.deemphasize_weak( ) );
        }
        if( i ) { strP.m_color = strP.m_color.deemphasize( ); }
        print_string_vertical( p_out, strP, posP, p_startIndent, p_indent );

        if( p_guideLines ) {
            // guide lines
            vertex_grid vg{ tikz_point{ p_sliceSpacing * i, 0.0 },
                            tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT } };
            if( fP.closed_begin( ) && i + 0U != frag.closed_end( ) ) {
                posP = tikz_point{ fragT.open_end( ) * CHAR_WIDTH
                                       + ( p_gs.size( ) - 1 ) * p_sliceSpacing + p_labelDis
                                       + CHAR_WIDTH * ( p_gs.size( ) - 1 - i ) / 6.25,
                                   fP.closed_begin( ) * -CHAR_HEIGHT };
                print_connection(
                    p_out,
                    vg.point_for_pos( fT.open_end( ), fP.closed_begin( ) ) + tikz_point{ .2, 0.0 },
                    posP + tikz_point{ -.2, 0.0 }, LINE_CONN,
                    LW_SUPPORT_LINE + ", "
                        + ( i % 5 ? COLOR_TEXT.deemphasize_strong( ) : COLOR_TEXT.deemphasize( ) )
                              .to_string( ),
                    p_startIndent, p_indent );
            }
            if( i && fP.open_end( ) != frag.open_end( ) ) {
                posP = tikz_point{ fragT.closed_begin( ) * CHAR_WIDTH - p_labelDis
                                       - CHAR_WIDTH * i / 6.25,
                                   fP.open_end( ) * -CHAR_HEIGHT };

                print_connection(
                    p_out,
                    vg.point_for_pos( fT.closed_begin( ), fP.open_end( ) ) + tikz_point{ -.2, 0.0 },
                    posP + tikz_point{ .2, 0.0 }, LINE_CONN,
                    LW_SUPPORT_LINE + ", "
                        + ( i % 5 ? COLOR_TEXT.deemphasize_strong( ) : COLOR_TEXT.deemphasize( ) )
                              .to_string( ),
                    p_startIndent, p_indent );
            }
        }
    }

    for( u32 i = 0; i < p_gs.size( ); ++i ) {
        const auto& slice = p_gs[ i ];
        auto        fP    = slice.fragment_p( );
        auto        fPo   = slice.original_fragment_p( );
        auto        posP  = tikz_point{ fragT.open_end( ) * CHAR_WIDTH + p_labelDis
                                    + ( p_gs.size( ) - 1 ) * p_sliceSpacing
                                    + CHAR_WIDTH * ( p_gs.size( ) - 1 - i ) / 6.25,
                                fP.closed_begin( ) * -CHAR_HEIGHT };
        std::string label;
        if( fPo.length( ) == 1 ) {
            label = std::string{ "P_{" } + std::to_string( p_d ) + ","
                    + std::to_string( fPo.closed_begin( ) ) + "}";
        } else {
            label = std::string{ "P" };
        }
        stylized_string strP = stylized_string{ label, str_displ_t::SDT_NAME }.slice( fP );
        if( fPo.length( ) > 1 ) {
            strP.m_displayStyle = str_displ_t::SDT_FRAGMENT;
        } else {
            strP.m_labelAlign = AN_BEGIN;
            strP.highlight_position( i - fP.closed_begin( ), SEP_COL.deemphasize( ) );
        }

        if( i != frag.closed_end( ) ) {
            strP.m_color = strP.m_color.deemphasize_strong( );
        } else {
            strP.m_color = strP.m_color.deemphasize( );
        }
        print_string_vertical( p_out, strP, posP, p_startIndent, p_indent );
    }
}

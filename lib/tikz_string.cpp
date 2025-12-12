#include <format>
#include "tikz_default_paths.h"
#include "tikz_option.h"
#include "tikz_string.h"
#include "tikz_util.h"

namespace TIKZ {
    void place_single_character( picture& p_pic, const stylized_string& p_S, u64 p_pos,
                                 std::pair<std::string, std::string> p_render,
                                 tikz_position p_center, const kv_store& p_extraOptions ) {

        p_pic.add_command(
            std::make_shared<math_command>( width_macro( CHAR_WIDTH, p_render.first ) ) );

        if( p_S.m_annotation.count( p_pos )
            && p_S.m_annotation.at( p_pos ).m_textColor.is_non_empty( ) ) {
            p_pic.place_node( p_center, p_render.second,
                              p_extraOptions
                                  | OPT::TEXT_COLOR( p_S.m_annotation.at( p_pos ).m_textColor ) );
        } else {
            p_pic.place_node( p_center, p_render.second, p_extraOptions );
        }
    }

    void place_wildcard( picture& p_pic, const stylized_string& p_S, u64 p_pos,
                         tikz_point p_center ) {
        double cor1 = p_pos ? .005 : 0, cor2 = ( p_pos + 1 < p_S.length( ) ) ? .005 : 0;
        auto   col = p_S.m_color;
        col.replace_if_non_empty( p_S.annotation_at_pos( p_pos ).m_textColor );

        auto bgcol = p_S.m_fillColor;
        bgcol.replace_if_non_empty( p_S.annotation_at_pos( p_pos ).m_bgColor );

        p_pic.place_rectangle( tikz_point{ p_center.m_x - CHAR_WIDTH / 2 + cor1,
                                           p_center.m_y + CHAR_HEIGHT / 2 - cor1 },
                               tikz_point{ p_center.m_x + CHAR_WIDTH / 2 - cor2,
                                           p_center.m_y - CHAR_HEIGHT / 2 + cor2 },
                               OPT::LW_OUTLINE | OPT::ROUNDED_CORNERS( "1pt" ) | OPT::FILL( col ) );

        const auto& wc = p_S.m_annotation.at( p_pos );
        if( wc.render_as_former_wc( ) ) {
            auto text = wc.m_symbol;
            p_pic.add_command( std::make_shared<math_command>( width_macro( CHAR_WIDTH, text ) ) );
            p_pic.place_node( p_center, text_typewriter( VSIZE_CORRECTION + text ),
                              OPT::TEXT_COLOR( bgcol ) );
        } else {
            // print wildcard symbol
            if( wc.show_symbol_if_wc( ) ) {
                p_pic.place_node( p_center, text_typewriter( wc.m_wildcardSymbol ),
                                  OPT::TEXT_COLOR( bgcol ) );
            } else {
                // wildcard lozenge
                p_pic.place_node( p_center, math_mode( WILDCARD_SYMBOL_FILLED ),
                                  OPT::TEXT_COLOR( bgcol ) );
            }
            // print wildcard index
            if( wc.show_id_if_wc( ) ) {
                auto text  = math_mode( std::to_string( wc.m_wildcardId ) );
                auto ftext = textsize_tiny( text );

                p_pic.add_command(
                    std::make_shared<math_command>( width_macro( CHAR_WIDTH / 2.0, text ) ) );
                p_pic.place_node(
                    tikz_point{ p_center.m_x + CHAR_WIDTH / 3.5, p_center.m_y - CHAR_HEIGHT / 3.5 },
                    ftext, OPT::TEXT_COLOR( bgcol ) );
            }
        }
    }

    void place_string_inner( picture& p_pic, const stylized_string& p_S, tikz_point p_StopLeft,
                             const kv_store& p_options ) {
        auto posy = p_StopLeft.m_y - CHAR_HEIGHT / 2.0;

        kv_store extraopt
            = OPT::TEXT_COLOR( p_S.m_color ) | OPT::INNER_SEP( "2pt" ) | XSCALE_TO_WIDTH;
        if( p_S.m_labelAlign == AN_BEGIN ) {
            extraopt = extraopt | OPT::ANCHOR_WEST;
        } else if( p_S.m_labelAlign == AN_END ) {
            extraopt = extraopt | OPT::ANCHOR_EAST;
        }
        extraopt = extraopt | p_options;

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
                place_single_character( p_pic, p_S, pos, render, tikz_point{ posx, posy },
                                        extraopt );
                break;
            }
            case stylized_string::fragment_t::FT_WILDCARD: {
                place_wildcard( p_pic, p_S, pos, tikz_point{ posx, posy } );
                break;
            }
            default: {
                p_pic.add_command( std::make_shared<math_command>(
                    width_macro( next.second.length( ) * CHAR_WIDTH, render.first ) ) );
                p_pic.place_node( tikz_point{ posx, posy }, render.second, extraopt );
                break;
            }
            }
            pos = next.second.open_end( );
        }
    }

    void place_string_inner_vertical( picture& p_pic, const stylized_string& p_S,
                                      tikz_point p_StopLeft, const kv_store& p_options ) {
        auto     posx = p_StopLeft.m_x + ( CHAR_WIDTH ) / 2.0;
        kv_store extraopt
            = OPT::TEXT_COLOR( p_S.m_color ) | OPT::INNER_SEP( "2pt" ) | XSCALE_TO_WIDTH;
        kv_store extraopt_rotate = OPT::TEXT_COLOR( p_S.m_color ) | OPT::INNER_SEP( "2pt" )
                                   | YSCALE_TO_WIDTH | OPT::ROTATE( "90" );

        if( p_S.m_labelAlign == AN_BEGIN ) {
            extraopt        = extraopt | OPT::ANCHOR_NORTH;
            extraopt_rotate = extraopt_rotate | OPT::ANCHOR_NORTH;
        } else if( p_S.m_labelAlign == AN_END ) {
            extraopt        = extraopt | OPT::ANCHOR_SOUTH;
            extraopt_rotate = extraopt_rotate | OPT::ANCHOR_SOUTH;
        }
        extraopt        = extraopt | p_options;
        extraopt_rotate = extraopt_rotate | p_options;

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
                place_single_character( p_pic, p_S, pos, render, tikz_point{ posx, posy },
                                        extraopt );
                break;
            }
            case stylized_string::fragment_t::FT_WILDCARD: {
                place_wildcard( p_pic, p_S, pos, tikz_point{ posx, posy } );
                break;
            }
            default: {
                p_pic.add_command( std::make_shared<math_command>(
                    width_macro( p_S.rotatable( ) ? next.second.length( ) * CHAR_WIDTH : CHAR_WIDTH,
                                 render.first ) ) );
                p_pic.place_node( tikz_point{ posx, posy }, render.second,
                                  p_S.rotatable( ) ? extraopt_rotate : extraopt );
                break;
            }
            }
            pos = next.second.open_end( );
        }
    }

    void place_string( picture& p_pic, const stylized_string& p_S, tikz_point p_StopLeft,
                       const kv_store& p_options, bool p_openBegin, bool p_openEnd ) {
        if( !p_S.length( ) ) { return; }
        double cor3 = -0.035;
        p_pic.place_rectangle(
            tikz_point{ p_StopLeft.m_x + ( !p_openBegin ) * cor3, p_StopLeft.m_y - cor3 },
            tikz_point{ p_StopLeft.m_x + p_S.length( ) * CHAR_WIDTH - ( !p_openEnd ) * cor3,
                        p_StopLeft.m_y - CHAR_HEIGHT + cor3 },
            OPT::ROUNDED_CORNERS( "2pt" ) | OPT::FILL( p_S.m_fillColor ) | p_S.m_outlineOptions );
        // for each character, print a small rectangle
        for( u64 i = 0; i < p_S.length( ); ++i ) {
            double cor1 = i ? .01 : 0, cor2 = ( i + 1 < p_S.length( ) ) ? .01 : 0;
            auto   col = p_S.m_color;
            col.replace_if_non_empty( p_S.annotation_at_pos( i ).m_textColor );
            p_pic.place_rectangle(
                tikz_point{ p_StopLeft.m_x + i * CHAR_WIDTH + cor1, p_StopLeft.m_y },
                tikz_point{ p_StopLeft.m_x + ( i + 1 ) * CHAR_WIDTH - cor2,
                            p_StopLeft.m_y - CHAR_HEIGHT },
                OPT::LW_OUTLINE | OPT::DRAW( col ) | OPT::ROUNDED_CORNERS( "1pt" ) | p_options );
        }
        // fill draw white box with label
        cor3 = 0.015;
        p_pic.place_rectangle(
            tikz_point{ p_StopLeft.m_x + ( !p_openBegin ) * cor3, p_StopLeft.m_y - cor3 },
            tikz_point{ p_StopLeft.m_x + p_S.length( ) * CHAR_WIDTH - ( !p_openEnd ) * cor3,
                        p_StopLeft.m_y - CHAR_HEIGHT + cor3 },
            OPT::ROUNDED_CORNERS( "1pt" ) | OPT::FILL( p_S.m_fillColor ) | p_S.m_fillOptions );
        for( u64 i = 0; i < p_S.length( ); ++i ) {
            double cor1 = i ? .04 : .05, cor2 = ( i + 1 < p_S.length( ) ) ? .04 : .05;
            auto   ann = p_S.annotation_at_pos( i );
            if( ann.m_bgColor.is_non_empty( ) ) {
                auto bgcol = ann.m_bgColor;
                p_pic.place_rectangle(
                    tikz_point{ p_StopLeft.m_x + i * CHAR_WIDTH + cor1, p_StopLeft.m_y - .05 },
                    tikz_point{ p_StopLeft.m_x + ( i + 1 ) * CHAR_WIDTH - cor2,
                                p_StopLeft.m_y - CHAR_HEIGHT + .05 },
                    OPT::ROUNDED_CORNERS( "2pt" ) | OPT::FILL( bgcol ) );
            }
        }
        place_string_inner( p_pic, p_S, p_StopLeft, p_options );
        double cor4 = .02;
        auto smno = OPT::FILL( p_S.m_color ) | OPT::CIRCLE | OPT::INNER_SEP( "0.1pt" ) | p_options;
        // add little dots to highlight parts between characters
        for( u64 i = !p_openBegin; i < p_S.length( ); ++i ) {
            p_pic.place_node( tikz_point{ p_StopLeft.m_x + i * CHAR_WIDTH, p_StopLeft.m_y - cor4 },
                              EMPTY_STR, smno );
            p_pic.place_node(
                tikz_point{ p_StopLeft.m_x + i * CHAR_WIDTH, p_StopLeft.m_y - CHAR_HEIGHT + cor4 },
                EMPTY_STR, smno );
        }
    }

    void place_string_vertical( picture& p_pic, const stylized_string& p_S, tikz_point p_StopLeft,
                                const kv_store& p_options, bool p_openBegin, bool p_openEnd ) {
        if( !p_S.length( ) ) { return; }
        double cor3 = -0.035;
        p_pic.place_rectangle(
            tikz_point{ p_StopLeft.m_x + cor3, p_StopLeft.m_y - ( !p_openBegin ) * cor3 },
            tikz_point{ p_StopLeft.m_x + CHAR_WIDTH - cor3,
                        p_StopLeft.m_y - p_S.length( ) * CHAR_HEIGHT + ( !p_openEnd ) * cor3 },
            OPT::ROUNDED_CORNERS( "2pt" ) | OPT::FILL( p_S.m_fillColor ) | p_S.m_outlineOptions );
        // for each character, print a small rectangle
        for( u64 i = 0; i < p_S.length( ); ++i ) {
            double cor1 = i ? .01 : 0, cor2 = ( i + 1 < p_S.length( ) ) ? .01 : 0;
            auto   col = p_S.m_color;
            col.replace_if_non_empty( p_S.annotation_at_pos( i ).m_textColor );
            p_pic.place_rectangle(
                tikz_point{ p_StopLeft.m_x, p_StopLeft.m_y - i * CHAR_HEIGHT - cor1 },
                tikz_point{ p_StopLeft.m_x + CHAR_WIDTH,
                            p_StopLeft.m_y - ( i + 1 ) * CHAR_HEIGHT + cor2 },
                OPT::LW_OUTLINE | OPT::DRAW( col ) | OPT::ROUNDED_CORNERS( "1pt" ) | p_options );
        }
        // fill draw white box with label
        cor3 = 0.015;
        p_pic.place_rectangle(
            tikz_point{ p_StopLeft.m_x + cor3, p_StopLeft.m_y - ( !p_openBegin ) * cor3 },
            tikz_point{ p_StopLeft.m_x + CHAR_WIDTH - cor3,
                        p_StopLeft.m_y - p_S.length( ) * CHAR_HEIGHT + ( !p_openEnd ) * cor3 },
            OPT::ROUNDED_CORNERS( "1pt" ) | OPT::FILL( p_S.m_fillColor ) | p_S.m_fillOptions );

        for( u64 i = 0; i < p_S.length( ); ++i ) {
            double cor1 = i ? .04 : 0.05, cor2 = ( i + 1 < p_S.length( ) ) ? .04 : .05;
            auto   ann = p_S.annotation_at_pos( i );
            if( ann.m_bgColor.is_non_empty( ) ) {
                auto bgcol = ann.m_bgColor;
                p_pic.place_rectangle(
                    tikz_point{ p_StopLeft.m_x + .05, p_StopLeft.m_y - i * CHAR_HEIGHT - cor1 },
                    tikz_point{ p_StopLeft.m_x + CHAR_WIDTH - .05,
                                p_StopLeft.m_y - ( i + 1 ) * CHAR_HEIGHT + cor2 },
                    OPT::ROUNDED_CORNERS( "2pt" ) | OPT::FILL( bgcol ) );
            }
        }

        place_string_inner_vertical( p_pic, p_S, p_StopLeft, p_options );

        double cor4 = .02;
        auto smno = OPT::FILL( p_S.m_color ) | OPT::CIRCLE | OPT::INNER_SEP( "0.1pt" ) | p_options;
        // add little dots to highlight parts between characters
        for( u64 i = !p_openBegin; i < p_S.length( ); ++i ) {
            p_pic.place_node( tikz_point{ p_StopLeft.m_x + cor4, p_StopLeft.m_y - i * CHAR_HEIGHT },
                              EMPTY_STR, smno );
            p_pic.place_node(
                tikz_point{ p_StopLeft.m_x + CHAR_WIDTH - cor4, p_StopLeft.m_y - i * CHAR_HEIGHT },
                EMPTY_STR, smno );
        }
    }

    void place_separator( picture& p_pic, tikz_point p_PtopLeft, tikz_point p_TtopLeft,
                          color p_color ) {
        // make sure T is top, P is bottom
        if( p_PtopLeft.m_y > p_TtopLeft.m_y ) { std::swap( p_PtopLeft, p_TtopLeft ); }

        auto opt = OPT::ROTATE( "45" ) | OPT::OUTER_SEP( "0pt" ) | OPT::INNER_SEP( "1pt" )
                   | OPT::FILL( p_color );
        p_pic.place_node( tikz_point{ p_TtopLeft.m_x, p_TtopLeft.m_y + 1.5 * CHAR_GLOW }, EMPTY_STR,
                          opt );
        p_pic.place_node(
            tikz_point{ p_PtopLeft.m_x, p_PtopLeft.m_y - CHAR_HEIGHT - 1.5 * CHAR_GLOW }, EMPTY_STR,
            opt );

        p_pic.place_simple_path(
            { tikz_point{ p_TtopLeft.m_x, p_TtopLeft.m_y + 1.5 * CHAR_GLOW },
              tikz_point{ p_TtopLeft.m_x, p_TtopLeft.m_y - CHAR_HEIGHT },
              tikz_point{ p_PtopLeft.m_x, p_PtopLeft.m_y },
              tikz_point{ p_PtopLeft.m_x, p_PtopLeft.m_y - CHAR_HEIGHT - 1.5 * CHAR_GLOW } },
            OPT::DRAW( p_color ) | OPT::LW_LINE | OPT::ROUNDED_CORNERS( "2pt" ) );
    }

    void place_matched_string_pair( picture& p_pic, const stylized_string& p_P,
                                    tikz_point p_PtopLeft, const stylized_string& p_T,
                                    tikz_point p_TtopLeft, color p_color, bool p_compress ) {
        // print glow bubble
        auto opt = OPT::DRAW( p_color ) | OPT::FILL( p_color.to_bg( ) )
                   | OPT::ROUNDED_CORNERS( "2pt" )
                   | ( p_compress ? OPT::LW_VERY_THIN_OUTLINE : OPT::LW_THIN_OUTLINE );
        auto glowx = p_compress ? 0 : CHAR_GLOW;
        auto glowy = p_compress ? -CHAR_GLOW : CHAR_GLOW;

        std::deque<tikz_position> pts{
            tikz_point{ p_PtopLeft.m_x - glowx, p_PtopLeft.m_y + 0 * glowy },
            tikz_point{ p_TtopLeft.m_x - glowx, p_TtopLeft.m_y - CHAR_HEIGHT - 0 * glowy } };

        // top line
        if( !p_compress || p_T.length( ) ) {
            pts.push_back( tikz_point{ p_TtopLeft.m_x - glowx, p_TtopLeft.m_y + glowy } );
            pts.push_back( tikz_point{ p_TtopLeft.m_x + p_T.length( ) * CHAR_WIDTH + glowx,
                                       p_TtopLeft.m_y + glowy } );
            pts.push_back( tikz_point{ p_TtopLeft.m_x + p_T.length( ) * CHAR_WIDTH + glowx,
                                       p_TtopLeft.m_y - CHAR_HEIGHT - 0 * glowy } );
        }

        // bottom line
        if( !p_compress || p_P.length( ) ) {
            pts.push_back( tikz_point{ p_PtopLeft.m_x + p_P.length( ) * CHAR_WIDTH + glowx,
                                       p_PtopLeft.m_y + 0 * glowy } );
            pts.push_back( tikz_point{ p_PtopLeft.m_x + p_P.length( ) * CHAR_WIDTH + glowx,
                                       p_PtopLeft.m_y - CHAR_HEIGHT - glowy } );
            pts.push_back(
                tikz_point{ p_PtopLeft.m_x - glowx, p_PtopLeft.m_y - CHAR_HEIGHT - glowy } );
        }
        pts.push_back( tikz_position{ } );
        p_pic.place_simple_path( pts, opt );

        if( p_P.length( ) == p_T.length( ) ) {
            // print "matching edges"
            auto lopt = OPT::LW_SUPPORT_LINE | OPT::DRAW( p_color.deemphasize( ) );
            for( u64 i = 0; i < p_P.length( ); ++i ) {
                p_pic.place_line(
                    tikz_point{ p_TtopLeft.m_x + ( i + .5 ) * CHAR_WIDTH,
                                p_TtopLeft.m_y - CHAR_HEIGHT },
                    tikz_point{ p_PtopLeft.m_x + ( i + .5 ) * CHAR_WIDTH, p_PtopLeft.m_y }, lopt );
            }
            for( u64 i = 0; i < p_P.length( ); ++i ) {
                // if characters are printed, check that they actually match
                bool match = p_P.has_wildcard( i + p_P.m_fragment.closed_begin( ) )
                             || p_T.has_wildcard( i + p_T.m_fragment.closed_begin( ) );
                match = match
                        || ( p_P[ i + p_P.m_fragment.closed_begin( ) ]
                             == p_T[ i + p_T.m_fragment.closed_begin( ) ] );

                if( !match ) {
                    // if they don't, draw a big cross symbolizing the replacement
                    place_double_cross(
                        p_pic,
                        tikz_point{ ( p_TtopLeft.m_x + ( i + .5 ) * CHAR_WIDTH + p_PtopLeft.m_x
                                      + ( i + .5 ) * CHAR_WIDTH )
                                        / 2.0,
                                    p_PtopLeft.m_y
                                        + ( p_TtopLeft.m_y - CHAR_HEIGHT - p_PtopLeft.m_y ) / 2.0 },
                        ( CROSS_FILL ? OPT::DOUBLE( p_color ) | OPT::DRAW( p_color.to_bg( ) )
                                     : OPT::DOUBLE( p_color.to_bg( ) )
                                           | OPT::DRAW( p_color.deemphasize( ) ) ),
                        3 );
                }
            }
        }

        if( !p_compress && p_P.length( ) ) { place_string( p_pic, p_P, p_PtopLeft ); }
        if( !p_compress && p_T.length( ) ) { place_string( p_pic, p_T, p_TtopLeft ); }
    }

    void place_edit_cost( picture& p_pic, u64 p_cost, tikz_position p_position, color p_color,
                          color p_bgColor ) {
        place_circled_number( p_pic, p_position, p_cost, 4 * CHAR_WIDTH, p_color, p_bgColor );
    }

    std::string char_or_empty( char p_char, const std::string& p_wildcard ) {
        if( !p_char ) { return "\\varepsilon"; }
        char wc = ( p_wildcard == EMPTY_STR || !p_wildcard.length( ) ) ? 0 : p_wildcard[ 0 ];
        if( p_char == wc ) { return math_mode( WILDCARD_SYMBOL ); }
        return text_typewriter( std::string{ p_char } );
    }

    std::string to_stringT( const breakpoint& p_bp, const std::string& p_wildcard ) {
        return textsize_footnotesize( math_mode( std::string{ "(" } + std::to_string( p_bp.m_posT )
                                                 + ", " + char_or_empty( p_bp.m_charT, p_wildcard )
                                                 + ")" ) );
    }

    std::string to_stringP( const breakpoint& p_bp, const std::string& p_wildcard ) {
        return textsize_footnotesize( math_mode( std::string{ "(" } + std::to_string( p_bp.m_posP )
                                                 + ", " + char_or_empty( p_bp.m_charP, p_wildcard )
                                                 + ")" ) );
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
    place_alignment( picture& p_pic, const stylized_string& p_P, tikz_point p_PtopLeft,
                     const stylized_string& p_T, tikz_point p_TtopLeft,
                     const breakpoint_repn& p_brpnt, alignment_style_t p_style ) {
        double pxpos = p_PtopLeft.m_x, txpos = p_TtopLeft.m_x;
        double sepWidth = 3 * CHAR_GLOW, extraGlow = 3.5 * CHAR_GLOW, smallGlow = 2 * CHAR_GLOW;

        bool showMatchedCharacters = p_style & AT_SHOW_MATCHED_CHARACTERS;
        bool compress              = p_style & AT_COMPRESS;
        bool printBreakpoints      = p_style & AT_PRINT_BREAKPOINTS;
        bool printExtraStringParts = p_style & AT_PRINT_EXTRA_STRING_PARTS;
        bool costs                 = p_style & AT_SHOW_EDIT_COST;

        stylized_string pnew{ p_P.m_name, p_P.m_fragment,
                              p_P.m_displayStyle | str_displ_t::SHOW_CHARACTERS };
        stylized_string tnew{ p_T.m_name, p_T.m_fragment,
                              p_T.m_displayStyle | str_displ_t::SHOW_CHARACTERS };

        for( u64 i = p_P.m_fragment.closed_begin( ); i < p_P.m_fragment.open_end( ); ++i ) {
            if( p_P.has_wildcard( i ) || showMatchedCharacters ) {
                pnew.annotation_at_pos( i ) = p_P.annotation_at_pos( i );
            }
        }
        for( u64 i = p_T.m_fragment.closed_begin( ); i < p_T.m_fragment.open_end( ); ++i ) {
            if( p_T.has_wildcard( i ) || showMatchedCharacters ) {
                tnew.annotation_at_pos( i ) = p_T.annotation_at_pos( i );
            }
        }

        if( compress ) {
            sepWidth = extraGlow = smallGlow = 0;
            printBreakpoints                 = false;
        }
        double pxinit = pxpos, txinit = txpos;

        if( printExtraStringParts ) {
            auto fragP = fragmentco{ p_P.m_fragment.closed_begin( ), p_brpnt.front( ).m_posP };
            auto fragT = fragmentco{ p_T.m_fragment.closed_begin( ), p_brpnt.front( ).m_posT };
            if( fragP.length( ) ) {
                if( !compress ) {
                    place_string( p_pic, p_P.slice( fragP ), tikz_point{ pxpos, p_PtopLeft.m_y } );
                }
                pxpos += CHAR_WIDTH * fragP.length( ) + smallGlow;
            } else {
                pxpos += CHAR_WIDTH * fragT.length( ) + smallGlow;
                pxinit = pxpos;
            }
            if( fragT.length( ) ) {
                if( !compress ) {
                    place_string( p_pic, p_T.slice( fragT ), tikz_point{ txpos, p_TtopLeft.m_y } );
                }
                txpos += CHAR_WIDTH * fragT.length( ) + smallGlow;
            } else {
                txpos += CHAR_WIDTH * fragP.length( ) + smallGlow;
                txinit = txpos;
            }
        }

        double last = pxpos, lastt = txpos;
        u64    shiftP = 0, shiftT = 0;

        if( printBreakpoints ) {
            // initial dummy breakpoint
            place_separator( p_pic, tikz_point{ pxpos, p_PtopLeft.m_y },
                             tikz_point{ txpos, p_TtopLeft.m_y }, SEP_COL.deemphasize( ) );
            p_pic.place_text(
                to_stringP( p_brpnt.front( ), WILDCARD ),
                tikz_point{ pxpos - 3 * CHAR_GLOW, p_PtopLeft.m_y - CHAR_HEIGHT - 3 * CHAR_GLOW },
                OPT::INNER_SEP( "0pt" ) | OPT::ANCHOR( "north west" )
                    | OPT::TEXT_COLOR( SEP_COL ) );
            p_pic.place_text( to_stringT( p_brpnt.front( ), WILDCARD ),
                              tikz_point{ txpos - 3 * CHAR_GLOW, p_TtopLeft.m_y + 3 * CHAR_GLOW },
                              OPT::INNER_SEP( "0pt" ) | OPT::ANCHOR( "south west" )
                                  | OPT::TEXT_COLOR( SEP_COL ) );
            pxpos += sepWidth;
            txpos += sepWidth;
        }

        for( u64 i = 1; i < p_brpnt.size( ); ++i ) {
            auto bp = p_brpnt[ i ], prevbp = p_brpnt[ i - 1 ];
            auto fragP = fragmentco{ prevbp.m_posP + shiftP, bp.m_posP };
            auto fragT = fragmentco{ prevbp.m_posT + shiftT, bp.m_posT };

            if( fragT.length( ) > 0 && fragP.length( ) > 0 ) {
                // matched fragment
                place_matched_string_pair( p_pic, p_P.slice( fragP ),
                                           tikz_point{ pxpos, p_PtopLeft.m_y }, p_T.slice( fragT ),
                                           tikz_point{ txpos, p_TtopLeft.m_y }, MAT_COL, compress );
                pxpos += CHAR_WIDTH * fragP.length( ) + extraGlow;
                txpos += CHAR_WIDTH * fragT.length( ) + extraGlow;
            }

            // print next breakpoint
            if( printBreakpoints ) {
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

                auto anchort = OPT::ANCHOR_SOUTH, anchorp = OPT::ANCHOR_NORTH;
                if( bp.m_posT == p_brpnt.back( ).m_posT ) {
                    anchort = OPT::ANCHOR( "south east" );
                    posxt += sepWidth;
                } else if( bp.m_posT == p_brpnt.front( ).m_posT ) {
                    anchort = OPT::ANCHOR( "south west" );
                    posxt -= sepWidth;
                } else {
                    posxt -= CHAR_GLOW;
                }
                if( bp.m_posP == p_brpnt.back( ).m_posP ) {
                    anchorp = OPT::ANCHOR( "north east" );
                    posx += sepWidth;
                } else if( bp.m_posP == p_brpnt.front( ).m_posP ) {
                    anchorp = OPT::ANCHOR( "north west" );
                    posx -= sepWidth;
                } else {
                    posx -= CHAR_GLOW;
                }

                auto col = color_for_bp( bp );
                pxpos    = pxpos - extraGlow + sepWidth;
                txpos    = txpos - extraGlow + sepWidth;
                place_separator( p_pic, { pxpos, p_PtopLeft.m_y }, { txpos, p_TtopLeft.m_y },
                                 col.deemphasize( ) );
                pxpos += sepWidth;
                txpos += sepWidth;
                p_pic.place_text(
                    to_stringP( p_brpnt[ i ], WILDCARD ),
                    tikz_point{ posx, p_PtopLeft.m_y - CHAR_HEIGHT - sepWidth - extray },
                    OPT::INNER_SEP( "0pt" ) | anchorp | OPT::TEXT_COLOR( col ) );
                p_pic.place_text( to_stringT( p_brpnt[ i ], WILDCARD ),
                                  tikz_point{ posxt, p_TtopLeft.m_y + sepWidth + extrayt },
                                  OPT::INNER_SEP( "0pt" ) | anchort | OPT::TEXT_COLOR( col ) );
            }

            if( bp.is_dummy( ) ) {
                shiftP = 0;
                shiftT = 0;
            } else {
                tikz_point cost_pos{ 0.0, 0.0 };
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
                    cost_pos = tikz_point{
                        ( txpos + ( .5 ) * CHAR_WIDTH + pxpos + ( .5 ) * CHAR_WIDTH ) / 2.0,
                        p_PtopLeft.m_y + ( p_TtopLeft.m_y - CHAR_HEIGHT - p_PtopLeft.m_y ) / 2.0 };
                } else if( bp.m_charT ) { // insertion
                    shiftT = 1;
                    shiftP = 0;

                    tnew[ bp.m_posT ] = std::string{ bp.m_charT };
                    tnew.highlight_position( bp.m_posT, color_for_bp( bp ) );
                    cost_pos
                        = tikz_point{ txpos + ( .5 ) * CHAR_WIDTH, p_TtopLeft.m_y - CHAR_HEIGHT };
                } else if( bp.m_charP ) { // deletion
                    shiftP = 1;
                    shiftT = 0;

                    pnew[ bp.m_posP ] = std::string{ bp.m_charP };
                    pnew.highlight_position( bp.m_posP, color_for_bp( bp ) );
                    cost_pos = tikz_point{ pxpos + ( .5 ) * CHAR_WIDTH, p_PtopLeft.m_y };
                }
                place_matched_string_pair(
                    p_pic,
                    p_P.slice( { bp.m_posP, bp.m_posP + shiftP } )
                        .replace_data( str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS,
                                       std::string{ bp.m_charP }, bp.m_posP ),
                    tikz_point{ pxpos, p_PtopLeft.m_y },
                    p_T.slice( { bp.m_posT, bp.m_posT + shiftT } )
                        .replace_data( str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS,
                                       std::string{ bp.m_charT }, bp.m_posT ),
                    tikz_point{ txpos, p_TtopLeft.m_y }, color_for_bp( bp ), compress );
                if( costs ) {
                    p_pic.place_coordinate( cost_pos,
                                            std::format( "costp-{}-{}", bp.m_posP, bp.m_posT ) );
                }
                pxpos += shiftP * CHAR_WIDTH + extraGlow;
                txpos += shiftT * CHAR_WIDTH + extraGlow;
            }
        }

        if( !compress && printExtraStringParts ) {
            auto fragP = fragmentco{ p_brpnt.back( ).m_posP, p_P.m_fragment.open_end( ) };
            auto fragT = fragmentco{ p_brpnt.back( ).m_posT, p_T.m_fragment.open_end( ) };
            auto corr  = smallGlow - extraGlow;
            if( printBreakpoints ) { corr = smallGlow - sepWidth; }
            if( fragP.length( ) ) {
                pxpos += corr;
                if( !compress ) {
                    place_string( p_pic, p_P.slice( fragP ), tikz_point{ pxpos, p_PtopLeft.m_y } );
                }
                pxpos += CHAR_WIDTH * fragP.length( );
            } else {
                pxpos += CHAR_WIDTH * fragT.length( ) + corr;
            }
            if( fragT.length( ) ) {
                txpos += corr;
                if( !compress ) {
                    place_string( p_pic, p_T.slice( fragT ), tikz_point{ txpos, p_TtopLeft.m_y } );
                }
                txpos += CHAR_WIDTH * fragT.length( );
            } else {
                txpos += CHAR_WIDTH * fragP.length( ) + corr;
            }
        }

        if( compress ) {
            place_string( p_pic, pnew, tikz_point{ pxinit, p_PtopLeft.m_y } );
            place_string( p_pic, tnew, tikz_point{ txinit, p_TtopLeft.m_y } );
        }

        if( costs ) {
            for( const auto& bp : p_brpnt ) {
                if( !bp.m_charT && !bp.m_charP ) { continue; }
                place_edit_cost( p_pic, bp.m_cost,
                                 std::format( "costp-{}-{}", bp.m_posP, bp.m_posT ),
                                 color_for_bp( bp ), color_for_bp( bp ).to_bg( ) );
            }
        }

        return { tikz_point{ pxpos, p_PtopLeft.m_y }, tikz_point{ txpos, p_TtopLeft.m_y } };
    }

    void place_highlighted_occurrence( picture& p_pic, const stylized_string& p_P,
                                       tikz_point p_PtopLeft, const stylized_string& p_T,
                                       tikz_point                         p_TtopLeft,
                                       const std::deque<breakpoint_repn>& p_occs, u64 p_selectedOcc,
                                       occ_style_t p_occstyle ) {
        alignment_style_t alstyle = AT_OCCS_DEFAULT;
        std::deque<bool>  has_occ{ };
        for( u64 y = 0; y < p_occs.size( ); ++y ) {
            while( has_occ.size( ) < p_occs[ y ].front( ).m_posT ) { has_occ.push_back( false ); }
            has_occ.push_back( true );
            if( y != p_selectedOcc ) { continue; }

            place_alignment( p_pic, p_P, p_PtopLeft, p_T, p_TtopLeft, p_occs[ y ], alstyle );

            // place maru/batsu on top of each position so far
            if( p_occstyle == occ_style_t::ALL_POS ) {
                for( u64 i = 0; i <= p_occs[ y ].front( ).m_posT; ++i ) {
                    if( has_occ[ i ] ) {
                        place_maru( p_pic,
                                    p_TtopLeft
                                        + tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                    } else {
                        place_batsu( p_pic,
                                     p_TtopLeft
                                         + tikz_point{ ( i + .5 ) * CHAR_WIDTH, CHAR_HEIGHT / 2 } );
                    }
                }
            } else if( p_occstyle == occ_style_t::STARTING_POS ) {
                for( u64 i = 0; i <= y; ++i ) {
                    place_maru( p_pic,
                                p_TtopLeft
                                    + tikz_point{ ( p_occs[ i ].front( ).m_posT + .5 ) * CHAR_WIDTH,
                                                  CHAR_HEIGHT / 2 } );
                }
            } else if( p_occstyle == occ_style_t::OCC_FULL ) {
                auto opt = OPT::ROTATE( "45" ) | OPT::OUTER_SEP( "0pt" ) | OPT::INNER_SEP( "1pt" )
                           | OPT::FILL( COLOR_C1.deemphasize_weak( ) );

                for( u64 i = 0; i <= y; ++i ) {
                    auto tl = p_TtopLeft
                              + tikz_point{ ( p_occs[ i ].front( ).m_posT ) * CHAR_WIDTH,
                                            CHAR_HEIGHT / 4 * ( i + 1 ) };
                    auto tr = p_TtopLeft
                              + tikz_point{ ( p_occs[ i ].back( ).m_posT ) * CHAR_WIDTH,
                                            CHAR_HEIGHT / 4 * ( i + 1 ) };

                    p_pic.place_node( tl, EMPTY_STR, opt );
                    p_pic.place_node( tr, EMPTY_STR, opt );

                    p_pic.place_line( tl, tr,
                                      OPT::DRAW( COLOR_C1.deemphasize_weak( ) ) | OPT::LW_LINE );
                }
            }
        }
    }

} // namespace TIKZ

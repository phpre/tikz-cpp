#include "tikz.h"
using namespace TIKZ;
std::string OUTDIR_DEFAULT  = "../figs/";
std::string TEXDIR_DEFAULT  = "../tex/";
std::string FONT_FILENAME   = "font";
std::string COLOR_FILENAME  = "color";
std::string MACROS_FILENAME = "macros";

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

void add_string_demo_pictures( document& p_doc ) {
    std::deque<std::string> optname{ "rotatable",      "show positions",  "show characters",
                                     "show wildcards", "group positions", "use typewriter" };
    std::string             data = "hello world!";
    std::string             name = "S";

    WITH_PICTURE( p1, { }, p_doc ) {
        for( u64 j = 1, i2 = 0; j < str_displ_t::MAX; j <<= 1, ++i2 ) {
            p1.place_text(
                VSIZE_CORRECTION + optname[ i2 ],
                tikz_point{ ( data.length( ) + 1 + i2 ) * CHAR_WIDTH, CHAR_HEIGHT * ( .5 ) },
                OPT::ROTATE( "90" ) | OPT::ANCHOR_WEST );
        }
        for( u64 i = 0; i < str_displ_t::MAX; i += 2 ) {
            auto S = stylized_string{ data, name, i }.add_wildcards(
                std::deque<u64>{ 0, 1, 5 }, chr_displ_t::SHOW_ID_IF_WILDCARD );
            auto SI = S.color_invert( );
            place_string( p1, S, tikz_point{ .0, -CHAR_HEIGHT * 1.0 * i } );
            place_string(
                p1, SI,
                tikz_point{ ( data.length( ) + 1 ) * -CHAR_WIDTH, -CHAR_HEIGHT * 1.0 * i } );
            for( u64 j = 1, i2 = 0; j < str_displ_t::MAX; j <<= 1, ++i2 ) {
                tikz_point pos{ ( data.length( ) + 1 + i2 ) * CHAR_WIDTH,
                                -CHAR_HEIGHT * ( 1.0 * i + .5 ) };
                if( i & j ) {
                    place_maru( p1, pos );
                } else {
                    place_batsu( p1, pos );
                }
            }
        }
    }
    WITH_PICTURE( p2, { }, p_doc ) {
        for( u64 i = 0; i < str_displ_t::MAX / 2; ++i ) {
            auto S = stylized_string{ data, name, i }.add_wildcards(
                std::deque<u64>{ 0, 1, 5 }, chr_displ_t::SHOW_ID_IF_WILDCARD );
            auto SI = S.color_invert( );
            place_string_vertical( p2, S, tikz_point{ CHAR_WIDTH * 2.0 * i, .0 } );
            place_string_vertical(
                p2, SI,
                tikz_point{ CHAR_WIDTH * ( 2.0 * i ), ( data.length( ) + 1 ) * CHAR_WIDTH } );
        }
    }
}

void add_alignment_demo_pictures( document& p_doc ) {
    WITH_PICTURE( pic, { }, p_doc ) {
        place_circled_number( pic, tikz_point{ 0.0, 0.0 }, 5, 4 * CHAR_WIDTH, COLOR_C1,
                              COLOR_C1.to_bg( ) );

        place_selected_arrow( pic, tikz_point{ -.5, -.5 }, tikz_point{ .5, -.5 }, COLOR_C2,
                              COLOR_C2.to_bg( ), 0 );
        place_edit_cost( pic, 5, tikz_point{ 0.0, -.5 }, COLOR_C2, COLOR_C2.to_bg( ) );

        place_double_arrow( pic, tikz_point{ -.5, -1. }, tikz_point{ .5, -1. }, COLOR_C2,
                            COLOR_C3.to_bg( ) );
        place_edit_cost( pic, 5, tikz_point{ 0.0, -1. }, COLOR_C3, COLOR_C3.to_bg( ) );
    }
    WITH_PICTURE( p1, { }, p_doc ) {
        std::string T  = "abab" + WILDCARD + "abcaab" + WILDCARD + "a" + WILDCARD + "a";
        std::string P  = "babcabcaccabaab";
        auto        bp = compute_breakpoints( P, T, WILDCARD );

        auto T_NAME = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD }.add_wildcards(
            T, chr_displ_t::SHOW_ID_IF_WILDCARD );
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        place_alignment( p1, P_NAME, tikz_point{ 0.0, 0.0 }, T_NAME, tikz_point{ 0.0, 1.25 }, bp );

        place_alignment( p1, P_NAME, tikz_point{ 0.0, 3.5 }, T_NAME, tikz_point{ 0.0, 4.75 }, bp,
                         AT_COMPRESS );

        place_alignment( p1, P_NAME, tikz_point{ 0.0, 6.5 }, T_NAME, tikz_point{ 0.0, 7.75 }, bp,
                         AT_NONE );
    }
    WITH_PICTURE( p2, { }, p_doc ) {
        std::string T = "b" + WILDCARD + WILDCARD + WILDCARD + "a" + WILDCARD + "ab" + WILDCARD
                        + WILDCARD + "a" + WILDCARD + "a";
        std::string P = "bcaaa";

        auto T_NAME = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD }.add_wildcards(
            T, chr_displ_t::SHOW_ID_IF_WILDCARD );
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        auto bps = compute_occs( P, T, WILDCARD );

        for( u64 y = 0; y < bps.size( ); ++y ) {
            place_alignment( p2, P_NAME, tikz_point{ 0.0, -3.0 * y }, T_NAME,
                             tikz_point{ 0.0, -3.0 * y + 1 }, bps[ y ],
                             AT_PRINT_EXTRA_STRING_PARTS | AT_COMPRESS );

            place_alignment( p2, P_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y },
                             T_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y + 1 },
                             bps[ y ], AT_PRINT_BREAKPOINTS | AT_PRINT_EXTRA_STRING_PARTS );
        }
    }
    WITH_PICTURE( p3, { }, p_doc ) {
        std::string T = "aabaabcaabaabba";
        std::string P = "bcaaa";

        auto T_NAME = stylized_string{ T, "T", str_displ_t::FRAGMENT_WILDCARD };
        auto P_NAME = stylized_string{ P, "P", str_displ_t::FRAGMENT_WILDCARD };

        auto bps = compute_occs_with_edits( P, T, 2 );

        for( u64 y = 0; y < bps.size( ); ++y ) {
            place_alignment( p3, P_NAME, tikz_point{ 0.0, -3.0 * y }, T_NAME,
                             tikz_point{ 0.0, -3.0 * y + 1 }, bps[ y ],
                             AT_PRINT_EXTRA_STRING_PARTS | AT_COMPRESS );

            place_alignment( p3, P_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y },
                             T_NAME, tikz_point{ ( 2 + T.length( ) ) * CHAR_WIDTH, -3.0 * y + 1 },
                             bps[ y ], AT_PRINT_EXTRA_STRING_PARTS | AT_PRINT_BREAKPOINTS );
        }
    }
}

void add_puzzle_demo_picture( document& p_doc ) {
    WITH_PICTURE( pic, { }, p_doc ) {
        place_puzzle_piece( pic, tikz_point{ -1.0, 1 },
                            OPT::LW_OUTLINE | OPT::DRAW( COLOR_TEXT )
                                | OPT::ROUNDED_CORNERS( "6pt" )
                                | OPT::FILL( COLOR_TEXT.deemphasize_weak( ) ),
                            puzzle_bend_t::CURVED_OUT, puzzle_bend_t::CURVED_IN,
                            puzzle_bend_t::CURVED_OUT, puzzle_bend_t::CURVED_OUT );
        place_puzzle_piece( pic, tikz_point{ 1.0, 1 },
                            OPT::LW_OUTLINE | OPT::DRAW( COLOR_C1 ) | OPT::ROUNDED_CORNERS( "6pt" )
                                | OPT::PATTERN_LINES_45
                                | OPT::PATTERN_COLOR( COLOR_C1.deemphasize( ) ) );
        place_puzzle_piece( pic, tikz_point{ -1.0, -1 },
                            OPT::LW_OUTLINE | OPT::DRAW( COLOR_C3 ) | OPT::ROUNDED_CORNERS( "6pt" )
                                | OPT::PATTERN_LINES_135
                                | OPT::PATTERN_COLOR( COLOR_C3.deemphasize( ) ),
                            puzzle_bend_t::CURVED_IN, puzzle_bend_t::CURVED_IN,
                            puzzle_bend_t::CURVED_IN, puzzle_bend_t::CURVED_OUT );
        place_puzzle_piece( pic, tikz_point{ 1.0, -1 },
                            OPT::LW_OUTLINE | OPT::DRAW( COLOR_C5 ) | OPT::ROUNDED_CORNERS( "6pt" )
                                | OPT::PATTERN_HATCH_45
                                | OPT::PATTERN_COLOR( COLOR_C5.deemphasize( ) ),
                            puzzle_bend_t::STRAIGHT, puzzle_bend_t::STRAIGHT,
                            puzzle_bend_t::CURVED_OUT, puzzle_bend_t::CURVED_OUT );
    }
}

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures
//
// ---------------------------------------------------------------------------------------------

FILE_SIMPLE( g01, { add_string_demo_pictures( doc ); } )
FILE_SIMPLE( g02, { add_alignment_demo_pictures( doc ); } )
FILE_SIMPLE( g03, { add_puzzle_demo_picture( doc ); } )
FILE_SIMPLE( g04, {
    // std::string T = "acabb";
    // std::string P = "cabba";
    std::string T = "sarrebr";
    std::string P = "saarb";
    auto        T_NAME
        = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS }
              .add_wildcards( T, chr_displ_t::SHOW_ID_IF_WILDCARD );
    auto P_NAME
        = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };

    add_breakpoint_computation_pictures( doc, T, T_NAME, P, P_NAME,
                                         { { 1, T.size( ) }, { 2, 1 }, { 2, 2 }, { 2, 3 } } );
} )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } );
